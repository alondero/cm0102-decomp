"""Explicit withdrawal of invalid C evidence, retaining source and validated lift."""
from __future__ import annotations

import copy
from datetime import datetime, timezone
import hashlib
import json
import os
from pathlib import Path

from . import coordination, env, inventory, ledger, work

SCHEMA = 'cmre-c-retraction/1'


def _bytes(value):
    return (json.dumps(value, indent=2, sort_keys=True) + '\n').encode('utf-8')


def _digest(blob):
    return hashlib.sha256(blob).hexdigest()


def _regular(path):
    path = Path(path)
    if not path.is_file() or path.is_symlink():
        raise ValueError(f'Required regular evidence file is missing or linked: {path}')
    return path.read_bytes()


def _implementation_identity():
    """Re-read every module: a cached process digest cannot fence code drift."""
    from . import recover
    directory = Path(__file__).parent
    modules = {p.relative_to(directory).as_posix(): _digest(p.read_bytes())
               for p in sorted(directory.rglob('*.py'))}
    harness = _digest(b''.join(name.encode() + (directory / name).read_bytes()
                               for name in recover.HARNESS_MODULES))
    return {'harness_sha256': harness, 'implementation_sha256': modules}


def _workspace_relative(path):
    resolved = Path(path).resolve()
    if not resolved.is_relative_to(env.ROOT.resolve()):
        raise ValueError('Retraction state must remain inside the workspace')
    return resolved.relative_to(env.ROOT.resolve()).as_posix()


def _manifest_path(va, reference):
    """History resolves only a canonical relative path within this function's archive."""
    if not isinstance(reference, str) or '\\' in reference:
        raise ValueError('Retraction manifest must be a workspace-relative POSIX path')
    relative = Path(reference)
    if relative.is_absolute() or '..' in relative.parts or relative.as_posix() != reference:
        raise ValueError('Retraction manifest path escapes or is not canonical')
    path = (env.ROOT / relative).resolve()
    base = (env.ANALYSIS / 'cmre/c-retractions' / f'fn_{va:08x}').resolve()
    if not path.is_relative_to(env.ROOT.resolve()) or not path.is_relative_to(base):
        raise ValueError('Retraction manifest escapes this function archive')
    parts = path.relative_to(base).parts
    if (len(parts) != 2 or parts[1] != 'manifest.json' or len(parts[0]) != 16
            or any(c not in '0123456789abcdef' for c in parts[0])):
        raise ValueError('Retraction manifest path has an unsupported archive identity')
    return path


def _snapshot(va, reason, witness):
    if not isinstance(reason, str) or not reason.strip():
        raise ValueError('A nonempty withdrawal reason is required')
    if any(entry.va == va for entry in ledger.legacy_entries()):
        raise ValueError('Legacy accepted records are frozen; C withdrawal is not supported')
    if f'fn_{va:08x}' in work.claims():
        raise ValueError('Function has a live claim; finish or release it before withdrawal')
    path = ledger.record_path(va)
    before_bytes = _regular(path)
    before = json.loads(before_bytes)
    if before.get('schema') != ledger.SCHEMA or int(before.get('va', '-1'), 16) != va:
        raise ValueError('Unsupported or inconsistent active ledger')
    if not before.get('c'):
        raise ValueError('Function has no active accepted C evidence')
    if not before.get('lifted') or not all(before.get('status', {}).get(k) for k in ('identified', 'compiled', 'validated')):
        raise ValueError('Withdrawal requires a retained compiled and validated lifted tier')
    if before.get('status', {}).get('integrated'):
        raise ValueError('Integrated evidence requires a separate reviewed withdrawal plan')
    function = inventory.load().functions.get(va)
    if function is None:
        raise ValueError('Function is absent from the pinned inventory')
    c = before['c']
    if c.get('source') != f'recovery/c/fn_{va:08x}.c':
        raise ValueError('Only the canonical single-function C record is supported')
    lifted = before['lifted']
    files = {}

    def retain(name, source, expected=None):
        blob = _regular(source)
        if expected is not None and _digest(blob) != expected:
            raise ValueError(f'Accepted evidence hash is stale: {source}')
        resolved = Path(source).resolve()
        original = (resolved.relative_to(env.ROOT.resolve()).as_posix()
                    if resolved.is_relative_to(env.ROOT.resolve()) else 'external:' + resolved.name)
        files[name] = {'original': original, 'sha256': _digest(blob), 'bytes': blob}
        return blob

    retain('ledger-before.json', path)
    retain('accepted-source.c', env.ROOT / c['source'], c['source_sha256'])
    retain('validated-lift.S', env.ROOT / lifted['source'], lifted['source_sha256'])
    witness = Path(witness)
    if witness.is_symlink():
        raise ValueError('Witness must be a regular retained file, not a link')
    witness = witness.resolve()
    witness_bytes = retain('witness.json', witness)
    try:
        proof = json.loads(witness_bytes)
        verdict, difference = proof['after_classifier']
        valid = (int(proof['function'].removeprefix('fn_'), 16) == va
                 and proof['accepted_source_sha256'] == c['source_sha256']
                 and proof['accepted_ledger_sha256'] == _digest(before_bytes)
                 and type(proof['case']) is int and proof['case'] >= 0
                 and proof['opt'] in ('-O0', '-O2')
                 and verdict == 'mismatch' and isinstance(difference, dict) and bool(difference.get('field')))
    except (ValueError, KeyError, TypeError, AttributeError):
        valid = False
    if not valid:
        raise ValueError('Witness must pin this accepted ledger/source and a named compared mismatch')
    run_key = c.get('run_key', '')
    if len(run_key) != 16 or any(x not in '0123456789abcdef' for x in run_key):
        raise ValueError('Accepted C run key is malformed')
    report_path = env.RUNS / f'fn_{va:08x}' / f'c-{run_key}' / 'report.json'
    report = json.loads(retain('accepted-report.json', report_path))
    if (report.get('key') != run_key or report.get('status') != 'pass'
            or report.get('source_sha256') != c['source_sha256']
            or report.get('harness_sha256') != c.get('harness_sha256')):
        raise ValueError('Retained report does not match the active accepted C provenance')
    c_bytes = _bytes(c)
    files['accepted-c-metadata.json'] = {'original': _workspace_relative(path) + '#c',
                                         'sha256': _digest(c_bytes), 'bytes': c_bytes}
    for index, (relative, expected) in enumerate(sorted(c.get('source_dependencies', {}).items())):
        dependency = (env.ROOT / relative).resolve()
        if not dependency.is_relative_to(env.ROOT.resolve()):
            raise ValueError('Accepted dependency escapes the workspace')
        retain(f'c-dependency-{index:03d}{dependency.suffix}', dependency, expected)
    if before.get('hints'):
        retain('hints-before.json', env.ROOT / before['hints'])
    canonical_hint = env.HINTS_DIR / f'fn_{va:08x}.json'
    canonical_hint_identity = {'path': _workspace_relative(canonical_hint), 'sha256': None}
    if canonical_hint.exists():
        retain('canonical-hints-before.json', canonical_hint)
        canonical_hint_identity['sha256'] = files['canonical-hints-before.json']['sha256']
    for index, extra in enumerate(proof.get('retained_evidence', [])):
        if not isinstance(extra, dict) or not isinstance(extra.get('path'), str):
            raise ValueError('Witness retained evidence must name paths and hashes')
        retain(f'witness-evidence-{index:03d}{Path(extra["path"]).suffix}', Path(extra['path']), extra.get('sha256'))
    work_path = work.path()
    old_work = work.read()['functions'].get(f'fn_{va:08x}')
    if work_path.exists():
        retain('work-state-before.json', work_path)
    from . import difftest, recover
    options = difftest.Options.for_function(va, tier='c')
    options.inputs = recover.c_inputs(va)
    options_bytes = _bytes(options.__dict__)
    options_sha = _digest(options_bytes)
    files['validation-options-before.json'] = {'original': 'effective C validation options',
                                               'sha256': options_sha, 'bytes': options_bytes}
    validation_contract = {'profile': recover.c_profile(va), 'inputs': options.inputs,
                           'effective_options_sha256': options_sha}
    base = {'schema': SCHEMA, 'va': f'0x{va:08x}', 'reason': reason.strip(),
            'files': {name: {k: v for k, v in row.items() if k != 'bytes'} for name, row in files.items()},
            'previous_function_work': old_work, 'inventory_body_bytes': function.body_bytes,
            'canonical_hints': canonical_hint_identity, 'validation_contract': validation_contract,
            **_implementation_identity()}
    archive_id = _digest(_bytes(base))
    archive = env.ANALYSIS / 'cmre/c-retractions' / f'fn_{va:08x}' / archive_id[:16]
    manifest = {**base, 'archive': _workspace_relative(archive), 'archive_id': archive_id,
                'retention': 'Append-only snapshots; original source, hints, lift and reports remain untouched.'}
    manifest_bytes = _bytes(manifest)
    after = copy.deepcopy(before)
    after.pop('c')
    after['status']['decompiled'] = False
    after.setdefault('c_retractions', []).append({
        'schema': SCHEMA, 'reason': reason.strip(), 'witness_sha256': _digest(witness_bytes),
        'source_sha256': c['source_sha256'], 'accepted_run_key': run_key,
        'accepted_harness_sha256': c['harness_sha256'],
        'validation_contract': validation_contract,
        'archive_manifest': _workspace_relative(archive / 'manifest.json'),
        'archive_manifest_sha256': _digest(manifest_bytes)})
    preview = {'schema': SCHEMA, 'state': 'preview', 'va': f'0x{va:08x}', 'reason': reason.strip(),
               'before_ledger_sha256': _digest(before_bytes), 'after_ledger_sha256': _digest(_bytes(after)),
               'before_ledger': before, 'after_ledger': after, 'archive': str(archive.resolve()),
               'archive_manifest': manifest, 'archive_manifest_sha256': _digest(manifest_bytes),
               'count_delta': {'c_tier_functions': -1, 'source_functions': -1,
                               'c_tier_body_bytes': -function.body_bytes, 'source_body_bytes': -function.body_bytes,
                               'lifted_functions': 0, 'recovered_functions': 0, 'recovered_body_bytes': 0},
               'work_after': {'state': 'evidence-needed', 'blocker': 'withdrawn-c-evidence',
                              'reason': reason.strip(), 'retry_when': 'Resolve the archived named mismatch, independently review retained C source, then explicitly retry and use normal cmre test/accept; withdrawal never reaccepts.'},
               'preserved': ['canonical C bytes', 'hints bytes', 'validated lift and source', 'old reports', 'prior accepted ledger and C metadata']}
    preview['plan_sha256'] = _digest(_bytes(preview))
    return preview, files, manifest_bytes, old_work


def preview(va, reason, witness):
    return _snapshot(va, reason, witness)[0]


def _archive(plan, files, manifest_bytes):
    folder = Path(plan['archive'])
    folder.parent.mkdir(parents=True, exist_ok=True)
    folder.mkdir()  # Exclusive: never overwrite a previous or interrupted archive.
    for name, row in files.items():
        _exclusive_bytes(folder / name, row['bytes'])
    _exclusive_bytes(folder / 'manifest.json', manifest_bytes)
    for name, row in files.items():
        if env.digest(folder / name) != row['sha256']:
            raise OSError(f'Archive verification failed: {name}')
    if env.digest(folder / 'manifest.json') != plan['archive_manifest_sha256']:
        raise OSError('Archive manifest verification failed')
    return folder


def _exclusive_bytes(path, blob):
    with path.open('xb') as stream:
        stream.write(blob)
        stream.flush()
        os.fsync(stream.fileno())


def apply(va, reason, witness, expected_plan):
    if not expected_plan:
        raise ValueError('Apply requires the exact --expect-plan SHA from a reviewed preview')
    with coordination.writer(), work.attempt_fence(va):
        plan, files, manifest_bytes, previous_work = _snapshot(va, reason, witness)
        if plan['plan_sha256'] != expected_plan:
            raise ValueError('Retraction preview is stale; regenerate and review the exact plan')
        folder = _archive(plan, files, manifest_bytes)
        # A changed source/witness/ledger after archival must not be withdrawn.
        if _snapshot(va, reason, witness)[0]['plan_sha256'] != expected_plan:
            raise ValueError('Retraction inputs changed during archival; retained archive is untouched')
        hold = plan['work_after']
        work.update(va, hold['state'], hold['reason'], hold['retry_when'], hold['blocker'],
                    expected_previous=previous_work)
        # If publication fails, old C evidence remains and the conservative hold
        # survives. Never claim success or remove the retained archive.
        coordination.atomic_bytes(ledger.record_path(va), _bytes(plan['after_ledger']))
        applied = {'schema': SCHEMA, 'state': 'applied', 'plan_sha256': expected_plan,
                   'created': datetime.now(timezone.utc).isoformat(),
                   'before_ledger_sha256': plan['before_ledger_sha256'],
                   'after_ledger_sha256': plan['after_ledger_sha256'],
                   'archive_manifest_sha256': plan['archive_manifest_sha256'],
                   'count_delta': plan['count_delta']}
        try:
            _exclusive_bytes(folder / 'applied.json', _bytes(applied))
        except OSError as exc:
            applied['state'] = 'applied-receipt-incomplete'
            applied['receipt_error'] = str(exc)
            applied['next_action'] = 'Ledger withdrawal succeeded and archive/history remain intact; inspect before any retry.'
        return applied


def command(args):
    from .cli import _va
    va = _va(args.va)
    result = (apply(va, args.reason, args.witness, args.expect_plan) if args.apply
              else preview(va, args.reason, args.witness))
    print(json.dumps(result, indent=2))
    return 1 if result.get('state') == 'applied-receipt-incomplete' else 0


def verify_history(record):
    """Retraction history must retain an intact, self-contained evidence archive."""
    problems = []
    history = record.get('c_retractions', [])
    if not isinstance(history, list) or not history:
        return ['Retraction history must be a nonempty list']
    for entry in history:
        try:
            manifest_path = _manifest_path(int(record['va'], 16), entry['archive_manifest'])
            if entry['schema'] != SCHEMA or _digest(_regular(manifest_path)) != entry['archive_manifest_sha256']:
                raise ValueError('retraction manifest changed')
            manifest = json.loads(manifest_path.read_text(encoding='utf-8'))
            if manifest.get('schema') != SCHEMA or manifest.get('va') != record['va']:
                raise ValueError('retraction manifest has a different function or schema')
            if manifest.get('archive') != _workspace_relative(manifest_path.parent):
                raise ValueError('retraction manifest archive provenance differs')
            if manifest.get('validation_contract') != entry.get('validation_contract'):
                raise ValueError('retraction validation contract differs from its retained manifest')
            for name, info in manifest['files'].items():
                if Path(name).name != name or _digest(_regular(manifest_path.parent / name)) != info['sha256']:
                    raise ValueError(f'retraction archive is missing or changed: {name}')
        except (OSError, KeyError, ValueError, TypeError) as exc:
            problems.append(str(exc))
    return problems


def add_commands(sub):
    parser = sub.add_parser('retract-c', help='preview withdrawal of invalid C evidence; retain source and validated lift')
    parser.add_argument('va')
    parser.add_argument('--reason', required=True)
    parser.add_argument('--witness', type=Path, required=True)
    parser.add_argument('--apply', action='store_true', help='explicitly apply a reviewed preview; never reaccept')
    parser.add_argument('--expect-plan', help='required with --apply: exact reviewed preview SHA256')
    parser.set_defaults(func=command)
