"""Closed finite-corpus proof for one stricter known-data forwarding transition.

No guest execution, acceptance, report rewriting or general comparison exception.
"""
from __future__ import annotations
import argparse
import ast
import copy
import math
from pathlib import Path
import re
from . import coordination, env, ledger, recover, session_evidence, validation as v

SCHEMA = 'cmre-known-data-compatibility/1'
OLD_HARNESS = 'fb73a0aa248762598c7fd97ba1e9037c984470e8946417c8d0dc01eb0f3986b5'
NEW_HARNESS = '4d54b827de6456a3faf8ae4b838c7ccefbfa949d62c4a62435d8895e684a8f65'
OLD_RECOVER = '1cd17c47d41302ea6f6af28d3e4a47de807d65b415e5dc00bd0981529eefd7a2'
NEW_RECOVER = 'd87d6f1f89f969fa6e84b1004e8902ae71afe65304896e6975847edb5e8f8fe8'
CHECKERS = ('known_data_compat.py', 'validation.py', 'validation_compat.py')

class Scope(set):
    known_data = True
    def __init__(self, origins):
        super().__init__(origins)
        self.origins = origins

def same_inputs(a, b):
    required = {'options', 'source', 'source_path', 'profile', 'optimizations', 'boundary'}
    return (isinstance(a, dict) and isinstance(b, dict) and required <= a.keys() and required <= b.keys()
            and {k:x for k,x in a.items() if k != 'accepted'} == {k:x for k,x in b.items() if k != 'accepted'})

def _ast(source, name):
    return next(n for n in ast.parse(source).body if isinstance(n, ast.FunctionDef) and n.name == name)

def _dump(node):
    return ast.dump(node, include_attributes=False)

def check_sources(old, new):
    """Exact hashes first, then independently confirm the numerical scope."""
    if env.digest_bytes(old.encode()) != OLD_RECOVER or env.digest_bytes(new.encode()) != NEW_RECOVER:
        raise ValueError('Unknown known-data recover source pair')
    for name in ('_data_spans', 'test_lifted', '_semantic_contract', '_indirect_calls', 'c_dependencies',
                 '_key', 'execution_identity', 'jump_contract', 'x87_contract'):
        if _dump(_ast(old,name)) != _dump(_ast(new,name)):
            raise ValueError('Guest dependency changed: '+name)
    candidate = copy.deepcopy(_ast(new,'test_c'))
    calls = [n for n in ast.walk(candidate) if isinstance(n,ast.Call) and ast.unparse(n.func)=='difftest.run']
    if len(calls)!=1 or [ast.unparse(k.value) for k in calls[0].keywords if k.arg=='data_spans'] != ['_data_spans(lifted)']:
        raise ValueError('Unknown C span forwarding')
    calls[0].keywords = [k for k in calls[0].keywords if k.arg!='data_spans']
    if _dump(candidate)!=_dump(_ast(old,'test_c')):
        raise ValueError('Additional C execution/comparison changes')
    for name,guard in [('c_profile',"not record.get('c') and (contract := _withdrawn_c_contract(record))"),
                       ('c_inputs',"not c and (contract := _withdrawn_c_contract(record))")]:
        found = [n for n in ast.walk(_ast(new,name)) if isinstance(n,ast.If) and '_withdrawn_c_contract' in ast.unparse(n.test)]
        if len(found)!=1 or _dump(found[0].test)!=_dump(ast.parse(guard,mode='eval').body):
            raise ValueError('Unknown active C policy selection')

def transition(before, after):
    if before.get('harness')!=OLD_HARNESS or after.get('harness')!=NEW_HARNESS:
        raise ValueError('Unknown known-data harness pair')
    ignored = {'harness','components','planner','dispatch_sources'}
    if {k:x for k,x in before.items() if k not in ignored}!={k:x for k,x in after.items() if k not in ignored}:
        raise ValueError('Environment, target, tools, policy or workspace drift')
    a,b=before['components'],after['components']
    if set(a)!=set(b) or {k for k in a if a[k]!=b[k]}!={'recover.py'} or a['recover.py']!=OLD_RECOVER or b['recover.py']!=NEW_RECOVER:
        raise ValueError('Additional guest module changes')
    for name,expected in b.items():
        if env.digest(Path(recover.__file__).with_name(name))!=expected:
            raise ValueError('Current guest module changed')
    old=before['dispatch_sources']['recover.py'];new=after['dispatch_sources']['recover.py']
    check_sources(old,new)
    if before['dispatch_sources']['difftest.py']!=after['dispatch_sources']['difftest.py']:
        raise ValueError('Classifier changed')

def execution(identity):
    result={k:x for k,x in identity.items() if k not in v.LOCAL_IDENTITY_FIELDS}
    result['headers']={env.rel(p):env.digest(p) for p in sorted((env.ROOT/'recovery/include').rglob('*')) if p.is_file()}
    return result

def report_pass(report, tier, opts):
    if report.get('status')!='pass' or report.get('tier')!=tier or report.get('problems'):
        raise ValueError('No intact passing report')
    builds=report.get('builds') if tier=='c' else {'assembler':{'status':'pass','difftest':report.get('difftest')}}
    if not isinstance(builds,dict) or (tier=='c' and set(builds)!=set(recover.C_OPT_LEVELS)):
        raise ValueError('Missing actual optimization builds')
    for build in builds.values():
        d=build.get('difftest')
        if build.get('status')!='pass' or not isinstance(d,dict) or type(d.get('mismatches')) is not list or d['mismatches']:
            raise ValueError('Missing/changed actual mismatch evidence')
        if (type(d.get('code_as_data_cases')) is not int or not 0<=d['code_as_data_cases']<=opts['cases']
                or (tier=='c' and d['code_as_data_cases']!=0)):
            raise ValueError('Missing/nonzero C CAD counter')
        if (type(d.get('inconclusive')) is not int or not 0<=d['inconclusive']<=opts['cases'] or
                (tier=='c' and d['inconclusive']>=opts['cases']/2)):
            raise ValueError('Invalid budget evidence')
        for name in ('cases','seed','budget'):
            if type(d.get(name)) is not int or d[name]!=opts[name]:raise ValueError('Changed '+name)
        if (d.get('inputs','v1')!=opts['inputs'] or not isinstance(d.get('outcomes'),dict)
                or any(type(n) is not int or n<0 for n in d['outcomes'].values()) or sum(d['outcomes'].values())!=opts['cases']):
            raise ValueError('Changed input policy/outcomes')
        if (not isinstance(d.get('result_digest'),str) or re.fullmatch('[0-9a-f]{64}',d['result_digest']) is None
                or type(d.get('coverage')) not in (int,float) or not math.isfinite(d['coverage'])
                or not opts['min_coverage']<=d['coverage']<=1):
            raise ValueError('Incomplete result/coverage')

def _safe_c_build(dependencies):
    files=dependencies.get('build',{}).get('files',{})
    if not files or not v._cacheable_build(files):raise ValueError('Uncertain compiler dependencies')
    for name,expected in files.items():
        p=env.ROOT/name
        if env.digest(p)!=expected:raise ValueError('Changed selected source/header')
        raw=re.sub(rb'\\\r?\n',b'',p.read_bytes())
        if re.search(rb'\b(?:asm|__asm|__asm__)\b|\.incbin\b|\.include\b',raw):
            raise ValueError('Assembler-side dependencies not covered')

def _ordered_inputs(options):
    """Sorted JSON keys cannot authenticate overlapping insertion-order writes."""
    hints=options['hints']
    for item in [hints]+list(hints.get('fixtures',[])):
        for name in ('arena_writes','globals'):
            addresses=sorted(int(key,16) for key in item.get(name,{}))
            if any(b<a+4 for a,b in zip(addresses,addresses[1:])):
                raise ValueError('Overlapping/aliased ordered input writes')

def _lift_source(va, report, source):
    from . import lift, build
    lifted=lift.discover(va)
    if lifted.problems:raise ValueError('Unknown current lift discovery')
    build._lift_return_sites(lifted)
    for text in report['lift']['byte_fallbacks']:
        instruction=lifted.insns[int(text,16)]
        if not instruction.as_bytes:
            if instruction.kind not in build.BYTE_SAFE or instruction.substitutions:raise ValueError('Unknown lift fallback')
            instruction.as_bytes=True
    if lift.render(lifted).encode()!=source.read_bytes():raise ValueError('Archived lift differs from current original-derived render')

def candidate(va, tier, current, before, index, prior):
    """Return an old-identity origin, or a named exclusion; never make a PASS."""
    try:
        if before.get('build_environment'):raise ValueError('Uncovered historic build environment')
        record=v.read(ledger.record_path(va));accepted=record.get(tier)
        if not accepted:raise ValueError('No active accepted tier')
        old=prior.get('results',{}).get(str(v.TierKey(va,tier)),{})
        if old.get('ok') is False and same_inputs(old.get('dependencies'),current):raise ValueError('Retained failure on these inputs')
        opts=v.effective_options(va,tier)
        _ordered_inputs(opts)
        if v.compact_options(opts)!=current['options']:raise ValueError('Plan options drift')
        ex=execution(before);source=env.ROOT/current['source_path'];text=source.read_bytes().decode('utf-8')
        if env.digest(source)!=current['source'] or accepted['source_sha256']!=current['source']:raise ValueError('Accepted source drift')
        if tier=='c':
            _safe_c_build(current)
            if recover.c_dependencies(source)!=current['build']:
                raise ValueError('Fresh current CPP/include lookup differs')
            matches=[r for r in index if int(r['va'],16)==va and r.get('accepted_source_sha256')==current['source']]
            if len(matches)!=1:raise ValueError('No independently retained C report origin')
            entry=matches[0];path=Path(entry['report'])
            if env.digest(path)!=entry['report_sha256']:raise ValueError('Authenticated C report changed')
            report=v.read(path);profile=recover.c_profile(va)
            returns,problems=recover._semantic_contract(va,text) if profile=='semantic' else ('int',[])
            if problems or report.get('profile')!=profile or (profile=='semantic' and report.get('returns')!=returns):raise ValueError('Changed return/profile contract')
            expected=recover._key(ex,text,(env.ROOT/'recovery/include/cmre.h').read_text(encoding='utf-8'),env.rel(source),current['build'],opts,
                profile if profile=='abi' else [profile,returns],recover.C_OPT_LEVELS,current['jump_contract'],current['x87_contract'])
            if accepted['run_key']!=expected or report['source_dependencies']!=current['build']['files'] or report['preprocessed_sha256']!=current['build']['preprocessed']:
                raise ValueError('Changed C key/CPP/headers/bindings')
            for opt,metrics in accepted['validation'].items():
                for key,value in metrics.items():
                    if report['builds'][opt]['difftest'].get(key)!=value:raise ValueError('Accepted C metrics disagree with report')
        else:
            if not old.get('ok') or not same_inputs(old.get('dependencies'),current):raise ValueError('No matching prior successful lift receipt')
            root=env.RUNS/f'fn_{va:08x}';request=recover._key(ex,va,opts,recover.LIFTED_PROFILE)
            key=v.read(root/f'request-{request}.json')['key'];path=root/key/'report.json';report=v.read(path);archived=root/key/source.name
            if archived.read_bytes()!=source.read_bytes():raise ValueError('Archived/canonical lifted bytes differ')
            _lift_source(va,report,source);expected=recover._key(ex,text,opts,recover.LIFTED_PROFILE)
            if report.get('profile',report['difftest']['profile'])!=recover.LIFTED_PROFILE:raise ValueError('Changed lifted profile')
            if not accepted.get('validation'):raise ValueError('Missing accepted lifted metrics')
            for key,value in accepted['validation'].items():
                if report['difftest'].get(key)!=value:raise ValueError('Accepted lifted metrics disagree with report')
        if report['key']!=expected or report['execution_identity']!=ex or report['source_sha256']!=current['source'] or report['harness_sha256']!=OLD_HARNESS:
            raise ValueError('Old complete input key/identity drift')
        report_pass(report,tier,opts)
        comparisons=([b['difftest'] for b in report['builds'].values()] if tier=='c' else [report['difftest']])
        return {'ok':True,'detail':'exact known-data proof; original report retained','dependencies':current,'identity':v.digest(ex),
                'refresh':True,'elapsed_seconds':old.get('elapsed_seconds'),
                'inconclusive_cases':sum(d['inconclusive'] for d in comparisons),
                'code_as_data_cases':sum(d['code_as_data_cases'] for d in comparisons),
                'differential_executions':0,
                'origin':{'report':str(path.resolve()),'sha256':env.digest(path),'old_key':report['key'],'harness':OLD_HARNESS}},None
    except (OSError,ValueError,KeyError,TypeError,UnicodeError,recover.build.BuildError) as exc:
        return None,str(exc)

def _selection():
    entries=ledger.cmre_entries();selection=[f'{e.va:08x}' for e in entries]
    return selection,{str(v.TierKey(e.va,t)): (e.va,t) for e in entries for t in (('lifted','c') if e.record.get('c') else ('lifted',))}

def _regressions(path):
    current=session_evidence.identity(env.ROOT);receipt=v.read(path)
    steps=[s.get('inputs_before') for s in receipt.get('steps',[]) if s.get('command')==['py','-3.13','-m','pytest']]
    if not steps:raise ValueError('Completed full pytest required')
    witness=steps[-1]
    def signature(identity):
        # Recovery inputs/provenance are authenticated separately for every tier;
        # status is generated acceptance bookkeeping. Preserve all other test
        # inputs, including non-Python fixtures and pytest configuration.
        return {**{k:x for k,x in identity.items() if k!='files'},'files':{k:x for k,x in identity['files'].items()
                if not k.startswith('recovery/') and k!='docs/STATUS.md'}}
    if signature(witness)!=signature(current) or 'tests/test_cmre_known_data_compat.py' not in witness.get('files',{}):raise ValueError('Proof regressions or environment changed')
    if witness.get('files',{}).get('tests/fixtures/known_data_recover_before.py')!=OLD_RECOVER:
        raise ValueError('Exact old-source regression fixture is not covered')
    result=session_evidence.successful_step(path,witness)
    if not result:raise ValueError('Full pytest did not finish successfully')
    return result,signature(current)

def create(before_path,after_path,regressions,origins,origins_sha256,reviewer,reason,output):
    before=v._load_results(before_path);plan=v.read(after_path);current=v.global_identity()
    if before.get('schema')!=v.SCHEMA or before.get('identity_drift') or plan.get('schema')!=v.SCHEMA or plan['identity']!=current:raise ValueError('Stale or unknown full inventory evidence')
    if not reviewer.strip() or not reason.strip():raise ValueError('Explicit independent reviewer and reason required')
    transition(before['identity'],current)
    if env.digest(origins)!=origins_sha256:raise ValueError('Reviewed origin index changed')
    index=v.read(origins);selection,tiers=_selection()
    if plan['functions']!=selection or set(plan['items'])!=set(tiers):raise ValueError('Current whole inventory selection required')
    tests,signature=_regressions(regressions);scope={};excluded={}
    for key,(va,tier) in tiers.items():
        item=plan['items'][key]
        try:
            now=v.snapshot(va,tier,item['dependencies'],execution=execution(before['identity']))
            if now!=item['dependencies']:raise ValueError('Plan inputs changed')
            origin,error=candidate(va,tier,now,before['identity'],index,before)
        except (OSError,ValueError,KeyError,TypeError,recover.build.BuildError) as exc:origin,error=None,str(exc)
        if origin:scope[key]=origin
        else:excluded[key]=error
    data={'schema':SCHEMA,'before':before['identity'],'after':current,'before_receipt':str(before_path.resolve()),'before_sha256':env.digest(before_path),
          'prior_results_sha256':v.digest(before.get('results',{})),'after_plan':str(after_path.resolve()),'after_sha256':env.digest(after_path),
          'origin_index':str(origins.resolve()),'origin_index_sha256':origins_sha256,'scope':scope,'excluded':excluded,'selection':selection,
          'tiers':sorted(tiers),'reviewer':reviewer,'reason':reason,'regressions':tests,'regression_signature':signature,
          'checker_sha256':{n:env.digest(Path(__file__).with_name(n)) for n in CHECKERS},
          'limitations':'Closed zero-CAD old PASS corpus equivalence only; unknowns execute. Old reports/identities retained, no new execution or acceptance.'}
    coordination.atomic_json(output,data);return data

def check(data,prior,identity):
    if data.get('schema')!=SCHEMA or data.get('before')!=prior.get('identity') or data.get('after')!=identity or prior.get('identity_drift') or not data.get('reviewer') or not data.get('reason'):
        raise ValueError('Known-data proof identities do not match')
    transition(data['before'],identity)
    if set(data['checker_sha256'])!=set(CHECKERS) or any(env.digest(Path(__file__).with_name(n))!=h for n,h in data['checker_sha256'].items()):raise ValueError('Known-data checker changed')
    for field,digest in [('before_receipt','before_sha256'),('after_plan','after_sha256'),('origin_index','origin_index_sha256')]:
        if env.digest(Path(data[field]))!=data[digest]:raise ValueError('Retained proof origin changed')
    if v.digest(prior.get('results',{}))!=data['prior_results_sha256']:raise ValueError('Prior result origins changed')
    selection,tiers=_selection()
    if selection!=data['selection'] or sorted(tiers)!=data['tiers']:raise ValueError('Current whole inventory changed')
    tests,signature=_regressions(Path(data['regressions']['receipt']))
    if tests!=data['regressions'] or signature!=data['regression_signature']:raise ValueError('Regression evidence changed')
    index=v.read(Path(data['origin_index']));eligible={}
    for key,origin in data['scope'].items():
        if env.digest(Path(origin['origin']['report']))!=origin['origin']['sha256']:raise ValueError('Authenticated old report body changed')
        va,tier=tiers[key]
        try:now=v.snapshot(va,tier,origin['dependencies'],execution=execution(data['before']))
        except (OSError,ValueError,KeyError,TypeError,recover.build.BuildError) as exc:
            raise ValueError('Known-data scope lost eligibility: '+key) from exc
        actual,error=candidate(va,tier,now,data['before'],index,prior)
        if not actual or not same_inputs(origin['dependencies'],now) or actual['origin']!=origin['origin']:
            raise ValueError('Known-data scope lost eligibility: '+key+': '+str(error))
        eligible[key]=origin
    return Scope(eligible)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('before','after','regressions','origins','origins-sha256','reviewer','reason','output'):p.add_argument('--'+name,required=True)
    a=p.parse_args();r=create(Path(a.before),Path(a.after),Path(a.regressions),Path(a.origins),a.origins_sha256,a.reviewer,a.reason,Path(a.output))
    print(f'Known-data proof: {len(r["scope"])} verified reusable tiers, {len(r["excluded"])} require execution; no acceptance')

if __name__=='__main__':main()
