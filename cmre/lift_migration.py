"""Explicit validated migration of accepted lift table declarations.

Only instruction/boundary/policy-preserving migrations with pinned old and new
source identities are admitted. The ledger is the publication commit point;
interrupted publication requires explicit reconciliation from retained artifacts.
This changes lift provenance, never C acceptance or integrated-source credit.
"""
import copy
import json
import os
import re
import sys
import uuid
from pathlib import Path
from cmre import coordination,difftest,env,image,ledger,lift,recover

def require(condition,message):
    if not condition:raise ValueError(message)

def digest_optional(path):return env.digest(path) if path.exists() else None

def paths_for(va):
    return {'source':env.LIFTED_DIR/f'fn_{va:08x}.S',
        'hints':env.HINTS_DIR/f'fn_{va:08x}.json','ledger':ledger.record_path(va)}

def encoded(value):return (json.dumps(value,indent=2,ensure_ascii=False)+'\n').encode()

def remove_fenced(path):
    """Deletion shares the same ownership check/publication gate as replacement."""
    lock_path=coordination._path()
    token=os.environ.get('CMRE_WRITER_TOKEN')
    inherited_path=os.environ.get('CMRE_WRITER_PATH')
    with coordination._gate(lock_path):
        if (not token or not inherited_path or Path(inherited_path).resolve()!=lock_path.resolve()
                or not coordination._live(lock_path,token)):
            raise coordination.BusyError('Inherited command lease expired before deletion')
        Path(path).unlink(missing_ok=True)

def reconcile(directory):
    """Restore the old baseline, or complete a committed new record; refuse drift."""
    with coordination.writer():
        directory=Path(directory).resolve()
        require(directory.is_relative_to(env.ANALYSIS.resolve()),'Journal outside analysis storage')
        state=json.loads((directory/'journal.json').read_text())
        require(state['schema']=='cmre-lift-migration/1','Wrong journal schema')
        require(state['target_sha256']==env.TARGET_SHA256,'Wrong target')
        paths=paths_for(state['va'])
        require(state['paths']=={k:env.rel(p) for k,p in paths.items()},'Journal target paths changed')
        require(not(env.C_DIR/f"fn_{state['va']:08x}.c").exists(),'C source appeared')
        before=state['before'];after=state['after']
        # Check every artifact and target before touching any target. A changed
        # accepted C/ledger, or unrelated edit, must never be overwritten.
        for key,path in paths.items():
            for phase,hashes in (('before',before),('after',after)):
                if hashes[key] is not None:
                    require(env.digest(directory/f'{phase}-{key}')==hashes[key],'Backup/artifact changed')
            require(digest_optional(path) in (before[key],after[key]),'Target changed outside this migration')
        committed=digest_optional(paths['ledger'])==after['ledger']
        chosen=after if committed else before
        phase='after' if committed else 'before'
        # The ledger is the commit point. Write it last on rollback as well.
        for key in ('source','hints','ledger'):
            if chosen[key] is None:remove_fenced(paths[key])
            else:coordination.atomic_bytes(paths[key],(directory/f'{phase}-{key}').read_bytes())
        state['state']='committed' if committed else 'rolled-back'
        coordination.atomic_json(directory/'journal.json',state)
        return state

def migrate(va,proposal,expected_old,expected_new,journal_root=None):
    """Validate unchanged instructions/policy and publish a journaled replacement."""
    with coordination.writer():
        require(type(va) is int and 0<=va<2**32,'Invalid function VA')
        for value in (expected_old,expected_new):require(re.fullmatch('[0-9a-f]{64}',value or '') is not None,'Expected source SHA256 required')
        paths=paths_for(va)
        original_bytes={key:path.read_bytes() if path.exists() else None for key,path in paths.items()}
        require(original_bytes['source'] is not None and original_bytes['ledger'] is not None,'Accepted lift required')
        record=json.loads(original_bytes['ledger'])
        require(not record.get('c') and not(env.C_DIR/f'fn_{va:08x}.c').exists(),'Migration excludes C/WIP source')
        require(record['input_sha256']==env.TARGET_SHA256 and env.digest(env.TARGET)==env.TARGET_SHA256,'Wrong original target')
        require(int(record['va'],16)==va and record['lifted']['source']==env.rel(paths['source']),'Accepted source path differs')
        require(env.digest_bytes(original_bytes['source'])==record['lifted']['source_sha256']==expected_old,'Accepted source changed')
        extent=record['extent'];start,end=int(extent['start'],16),int(extent['end'],16)
        require(start==va and extent['bytes']==end-start and env.digest_bytes(image.primary().read(start,end-start))==extent['sha256'],'Original extent changed')
        old_hint=json.loads(original_bytes['hints']) if original_bytes['hints'] is not None else {}
        proposal=Path(proposal)
        hint_bytes=proposal.read_bytes();new_hint=json.loads(hint_bytes)
        difftest.validate_hints(new_hint)
        require(bool(new_hint.get('lift_jump_tables')),'Explicit table declarations required')
        require(new_hint.get('auto') is not True,'Agent must own proposed hints')
        metadata={'notes','fixture_provenance','fixture_evidence','input_probe','input_probe_sha256','auto'}
        ignored=metadata|{'fixtures','lift_jump_tables'}
        require({k:v for k,v in old_hint.items() if k not in ignored}=={k:v for k,v in new_hint.items() if k not in ignored},'Exploration/policy inputs changed')
        old_fixtures=old_hint.get('fixtures',[]);new_fixtures=new_hint.get('fixtures',[])
        require(new_fixtures[:len(old_fixtures)]==old_fixtures,'Existing fixtures removed/reordered')
        identity=recover.execution_identity()
        old_options=difftest.Options.for_function(va)
        baseline=lift.discover(va)
        require(not baseline.problems,'Existing discovery is blocked')
        old_report=recover.test_lifted(va)
        require(old_report['status']=='pass' and old_report['source_sha256']==expected_old,'Existing lift no longer validates')
        journal_root=Path(journal_root or env.ANALYSIS/'cmre/lift-migrations').resolve()
        require(journal_root.is_relative_to(env.ANALYSIS.resolve()),'Journal outside analysis storage')
        directory=journal_root/uuid.uuid4().hex
        directory.mkdir(parents=True)
        proposed_hints=directory/'validation-hints';proposed_hints.mkdir()
        coordination.atomic_bytes(proposed_hints/paths['hints'].name,hint_bytes)
        old_dir=env.HINTS_DIR
        try:
            env.HINTS_DIR=proposed_hints
            proposed=lift.discover(va)
            require(not proposed.problems,'Proposed discovery is blocked')
            require({a:(i.raw,i.kind,i.target) for a,i in baseline.insns.items()}=={a:(i.raw,i.kind,i.target) for a,i in proposed.insns.items()},'Original instructions/edges changed')
            require((baseline.callees,baseline.imports,recover.extent(baseline))==(proposed.callees,proposed.imports,recover.extent(proposed)),'Original boundaries changed')
            require(recover.extent(proposed)==(start,end),'Accepted extent changed')
            new_options=difftest.Options.for_function(va)
            require({k:v for k,v in old_options.__dict__.items() if k!='hints'}=={k:v for k,v in new_options.__dict__.items() if k!='hints'},'Validation policy changed')
            first=recover.test_lifted(va)
            second=recover.test_lifted(va,fresh=True)
            require(first['status']==second['status']=='pass','Proposed lift failed validation')
            require(first['source_sha256']==second['source_sha256']==expected_new and expected_new!=expected_old,'Unexpected proposed source')
            require(first['key']==second['key'] and first['difftest']['result_digest']==second['difftest']['result_digest'],'Nondeterministic validation')
            require(first['execution_identity']==second['execution_identity']==identity==recover.execution_identity(),'Execution identity changed')
            require(new_options==difftest.Options.for_function(va),'Proposed hints changed during validation')
            generated=env.RUNS/f'fn_{va:08x}'/first['key']/paths['source'].name
            source_bytes=generated.read_bytes()
            require(env.digest_bytes(source_bytes)==expected_new,'Validated generated source changed')
            new_tier=recover._tier_record(first,generated)
            new_tier['source']=env.rel(paths['source'])
        finally:env.HINTS_DIR=old_dir
        require(all((path.read_bytes() if path.exists() else None)==original_bytes[key] for key,path in paths.items()),'Accepted inputs changed during validation')
        require(proposal.read_bytes()==hint_bytes and not(env.C_DIR/f'fn_{va:08x}.c').exists(),'Proposal/C source changed')
        new_record=copy.deepcopy(record)
        new_record['lifted']=new_tier
        new_record['hints']=env.rel(paths['hints'])
        new_record.setdefault('source_migrations',[]).append(dict(journal=env.rel(directory),old_sha256=expected_old,new_sha256=expected_new))
        proposed_bytes={'source':source_bytes,'hints':hint_bytes,'ledger':encoded(new_record)}
        state=dict(schema='cmre-lift-migration/1',state='prepared',va=va,target_sha256=env.TARGET_SHA256,
            paths={key:env.rel(path) for key,path in paths.items()},
            before={key:env.digest_bytes(value) if value is not None else None for key,value in original_bytes.items()},
            after={key:env.digest_bytes(value) for key,value in proposed_bytes.items()},
            execution_identity=identity,original_body_bytes=baseline.instruction_bytes,
            original_instruction_count=len(baseline.insns),accepted_extent_bytes=end-start,
            baseline_key=old_report['key'],baseline_digest=old_report['difftest']['result_digest'],
            validation_keys=[first['key'],second['key']],validation_digests=[first['difftest']['result_digest'],second['difftest']['result_digest']])
        for phase,values in (('before',original_bytes),('after',proposed_bytes)):
            for key,value in values.items():
                if value is not None:coordination.atomic_bytes(directory/f'{phase}-{key}',value)
        coordination.atomic_json(directory/'baseline-report.json',old_report)
        coordination.atomic_json(directory/'first-report.json',first)
        coordination.atomic_json(directory/'second-report.json',second)
        coordination.atomic_json(directory/'journal.json',state)
        for key in ('source','hints','ledger'):
            coordination.atomic_bytes(paths[key],proposed_bytes[key])
        state['state']='committed'
        coordination.atomic_json(directory/'journal.json',state)
        return directory,state


def _command(args):
    """Both actions acquire the normal writer lease through their API entry."""
    try:
        if args.action == 'apply':
            va = int(args.va.lower().removeprefix('fn_'), 16)
            directory, state = migrate(va, args.hints, args.expected_old_sha256,
                                       args.expected_new_sha256, args.journal_root)
        else:
            directory = Path(args.journal).resolve()
            state = reconcile(directory)
        print(json.dumps(dict(status=state['state'], journal=env.rel(directory),
                              va=f"0x{state['va']:08x}",
                              accepted_source_bytes_added=0), indent=2))
        return 0
    except (ValueError, OSError, coordination.BusyError) as error:
        print(json.dumps(dict(status='error', detail=str(error))), file=sys.stderr)
        return 2


def add_commands(sub):
    parser = sub.add_parser('lift-migration', help='explicit validated lift table-layout migration')
    actions = parser.add_subparsers(dest='action', required=True)
    apply = actions.add_parser('apply', help='validate pinned old/new lift and retain publication journal')
    apply.add_argument('va')
    apply.add_argument('--hints', required=True)
    apply.add_argument('--expected-old-sha256', required=True)
    apply.add_argument('--expected-new-sha256', required=True)
    apply.add_argument('--journal-root', help='journal directory beneath analysis storage')
    apply.set_defaults(func=_command)
    reconcile_parser = actions.add_parser('reconcile', help='recover an interrupted lift migration')
    reconcile_parser.add_argument('journal')
    reconcile_parser.set_defaults(func=_command)
