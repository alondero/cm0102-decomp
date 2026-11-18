"""Coordinator-mediated same-machine C workers; no shared recovery directory.

Workers edit one authored C file. Evidence, leases, attempts, review and acceptance
remain in the authoritative checkout. This coordinates trusted local processes;
it is not a filesystem sandbox against a malicious process running as the user.
"""
from __future__ import annotations

from contextlib import contextmanager
import json
import os
from pathlib import Path
import secrets
import subprocess
import sys

from . import coordination, env, tasks, work

SCHEMA = 'cmre-worktree/1'
LOCAL = Path('CM0102/Analysis/cmre/worktree')


def _file(root: Path, relative: str | Path) -> Path:
    path = root / relative
    if path.resolve() != path:
        raise ValueError(f'Worktree path may not redirect: {path}')
    return path


def _git_identity(root: Path) -> dict:
    def git(*args):
        result = subprocess.run(['git', '-C', str(root), *args], capture_output=True, text=True)
        if result.returncode:
            raise ValueError(f'Not an accessible Git worktree: {root}')
        return result.stdout.strip()
    if Path(git('rev-parse', '--show-toplevel')).resolve() != root:
        raise ValueError('Expected a Git worktree root')
    common = Path(git('rev-parse', '--git-common-dir'))
    if not common.is_absolute():
        common = root / common
    return {'common': os.path.normcase(str(common.resolve())),
            'head': git('rev-parse', 'HEAD')}


def _code_files(root: Path) -> dict[str, Path]:
    modules, headers = _file(root, 'cmre'), _file(root, 'recovery/include')
    files = sorted([*modules.glob('*.py'), *headers.rglob('*')])
    return {p.relative_to(root).as_posix(): _file(root, p.relative_to(root))
            for p in files if p.is_file()}


def _code_identity(root: Path) -> str:
    return tasks._hash({name: env.digest(path) for name, path in _code_files(root).items()})


def _align_tooling(worker: Path, expected: str) -> None:
    """Preflight every destination, then copy exact bytes into an unbound tree.

    Copies already matching the coordinator are left alone, including copies
    retained by an interrupted alignment. Extra worker files require manual
    reconciliation; this command never deletes them or touches candidate C.
    """
    source, destination = _code_files(env.ROOT), _code_files(worker)
    extra = sorted(destination.keys() - source.keys())
    if extra:
        raise ValueError('Worker has extra tooling/header files; preserve and reconcile: ' + ', '.join(extra))
    payloads = {name: path.read_bytes() for name, path in source.items()}
    if tasks._hash({name: env.digest_bytes(data) for name, data in payloads.items()}) != expected:
        raise ValueError('Coordinator tooling changed while planning alignment')
    changes = []
    for name, payload in payloads.items():
        path = _file(worker, name)
        if path.exists() and not path.is_file():
            raise ValueError(f'Worker tooling destination is not a file: {name}')
        before = path.read_bytes() if path.exists() else None
        if before == payload:
            continue
        flags = subprocess.run(['git', '-C', str(worker), 'ls-files', '-v', '-z', '--', name],
                               capture_output=True)
        if flags.returncode:
            raise ValueError(f'Cannot inspect worker index flags: {name}')
        # Status can hide changes under assume-unchanged or skip-worktree.
        # Require an ordinary tracked entry, or no entry for a new module.
        if any(entry[:1] != b'H' for entry in flags.stdout.split(b'\0') if entry):
            raise ValueError(f'Alignment cannot establish clean worker edits: {name}')
        # Include ignored and untracked paths as well as index/worktree edits.
        # A tracked deletion is a worker edit, not an invitation to restore it.
        result = subprocess.run(['git', '-C', str(worker), 'status', '--porcelain=v1',
                                 '-z', '--untracked-files=all', '--ignored', '--', name],
                                capture_output=True)
        if result.returncode:
            raise ValueError(f'Cannot inspect worker edits: {name}')
        if result.stdout:
            raise ValueError(f'Alignment would overwrite worker edits: {name}')
        changes.append((path, before, payload))
    for path, before, payload in changes:
        _file(worker, path.relative_to(worker))
        if (path.read_bytes() if path.exists() else None) != before:
            raise ValueError(f'Worker tooling changed during alignment: {path}')
        coordination.atomic_bytes(path, payload)
    if _code_identity(env.ROOT) != expected or _code_identity(worker) != expected:
        raise ValueError('Tooling changed during alignment; retain the copies and inspect before retrying')


def _pins() -> None:
    for path, expected in {env.TARGET: env.TARGET_SHA256, **env.INVENTORY_SHA256}.items():
        if not path.is_file() or env.digest(path) != expected:
            raise ValueError(f'Pinned asset hash mismatch: {env.rel(path)}')


def _state_path(worker: Path) -> Path:
    key = env.digest_bytes(str(worker).encode())[:32]
    return _file(env.ROOT, env.ANALYSIS.relative_to(env.ROOT) / f'cmre/worktrees/{key}.json')


def _save(worker: Path, state: dict) -> None:
    coordination.atomic_json(_state_path(worker), state)


def _worker_lock(worker: Path):
    return coordination.lock(_state_path(worker).with_suffix('.lock'))


def _load(worker: Path) -> dict:
    state = json.loads(_state_path(worker).read_text(encoding='utf-8'))
    local = json.loads(_file(worker, LOCAL / 'binding.json').read_text(encoding='utf-8'))
    if (state.get('schema') != SCHEMA or state.get('worker') != str(worker)
            or state.get('coordinator') != str(env.ROOT)
            or local != {k: state[k] for k in ('schema', 'worker', 'coordinator', 'binding')}):
        raise ValueError('Worktree binding mismatch; coordinator must bootstrap it')
    if (_git_identity(worker) != state['worker_git']
            or _git_identity(env.ROOT) != state['coordinator_git']
            or _code_identity(worker) != state['code'] or _code_identity(env.ROOT) != state['code']):
        raise ValueError('Stale worktree code/HEAD snapshot; finish or retire claims before bootstrapping a fresh worktree')
    return state


def bootstrap(worker: Path, *, align: bool = False) -> dict:
    worker = Path(worker).resolve()
    if worker.is_relative_to(env.ROOT) or env.ROOT.is_relative_to(worker):
        raise ValueError('Use a separate, non-nested Git worktree')
    with coordination.writer(), _worker_lock(worker):
        _pins()
        coordinator_git, worker_git = _git_identity(env.ROOT), _git_identity(worker)
        if coordinator_git['common'] != worker_git['common']:
            raise ValueError('Worker must share the coordinator Git common directory')
        # No links into authoritative assets, evidence or mutable recovery state.
        for relative in ('CM0102', LOCAL, 'recovery', 'recovery/c', 'recovery/hints',
                         'recovery/ledger', 'recovery/lifted', 'recovery/blocked'):
            _file(worker, relative)
        if _state_path(worker).exists():
            retained = json.loads(_state_path(worker).read_text(encoding='utf-8'))
            if (retained.get('schema') != SCHEMA or retained.get('worker') != str(worker)
                    or retained.get('coordinator') != str(env.ROOT)):
                raise ValueError('Worktree binding mismatch; preserve existing state')
            if (retained['worker_git'] != worker_git or retained['coordinator_git'] != coordinator_git
                    or retained['code'] != _code_identity(env.ROOT)
                    or retained['code'] != _code_identity(worker)):
                raise ValueError('Stale worktree code/HEAD snapshot; preserve claims and use a fresh worktree')
            if not _file(worker, LOCAL / 'binding.json').exists():
                coordination.atomic_json(_file(worker, LOCAL / 'binding.json'),
                                         {k: retained[k] for k in ('schema', 'worker', 'coordinator', 'binding')})
            return _load(worker)
        if _file(worker, LOCAL / 'binding.json').exists():
            raise ValueError('Worker has a retained local binding without coordinator state; reconcile it first')
        code = _code_identity(env.ROOT)
        if _code_identity(worker) != code:
            if not align:
                raise ValueError('Worker tooling/header snapshot differs from coordinator; '
                                 'use bootstrap --align for a fresh worker with clean tooling paths')
            _align_tooling(worker, code)
        state = {'schema': SCHEMA, 'worker': str(worker), 'coordinator': str(env.ROOT),
                 'binding': secrets.token_hex(24), 'code': code,
                 'worker_git': worker_git, 'coordinator_git': coordinator_git, 'functions': {}}
        # Save authority first. An interrupted bootstrap can be retried with the
        # retained binding; it never creates a second authority or clears attempts.
        _save(worker, state)
        coordination.atomic_json(_file(worker, LOCAL / 'binding.json'),
                                 {k: state[k] for k in ('schema', 'worker', 'coordinator', 'binding')})
        return state


def _function_lock(va: int):
    return coordination.lock(env.ANALYSIS / f'cmre/worktrees/fn_{va:08x}.lock')


def _budget(va: int) -> None:
    # Count retained tasks across input fingerprints, owners and worker roots.
    record = work.read()['functions'].get(f'fn_{va:08x}', {})
    if tasks.attempts_used(va, record) >= tasks.MAX_ATTEMPTS:
        raise ValueError('Function exhausted its five-attempt budget; retained evidence requires coordinator investigation')


def _entry(state: dict, va: int) -> dict:
    try:
        return state['functions'][f'fn_{va:08x}']
    except KeyError:
        raise ValueError('Function has no worktree claim') from None


def _result(entry: dict, manifest: dict) -> dict:
    return {'task_id': manifest['task_id'], 'status': manifest['status'],
            'owner': entry['owner'], 'expires_at': entry['expires_at'],
            'attempt_count': len(manifest['attempts']), 'report': entry.get('report')}


def claim(worker: Path, va: int, owner: str, seconds: int = 900) -> dict:
    worker = Path(worker).resolve()
    if not isinstance(owner, str) or not owner.strip() or len(owner) > 60:
        raise ValueError('Owner must be a nonempty name of at most 60 characters')
    if type(seconds) is not int or not 1 <= seconds <= 3600:
        raise ValueError('Lease must be 1..3600 seconds')
    with coordination.writer(), _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        _pins()
        key = f'fn_{va:08x}'
        with work.attempt_fence(va), work.locked():
            if key in work.claims():
                raise ValueError('Function already claimed; wait for expiry or integrator completion')
            hold = work.dispatch_hold(va, work.read()['functions'])
            if hold:
                raise ValueError(f"Function has a scheduling hold: {hold['reason']}; {hold['retry_when']}")
            manifest = tasks.prepare(va)
            task_id = manifest['task_id']
            central_owner = f"wt:{state['binding'][:16]}:{owner}"
            manifest = tasks._claim_with_work_lock(task_id, central_owner, seconds)
            work._claim_unlocked([va], central_owner, seconds / 3600)
        entry = {'task_id': task_id, 'owner': central_owner, 'token': manifest['lease']['token'],
                 'expires_at': manifest['lease']['expires_at']}
        packet = tasks.status(task_id)
        packet_path = tasks.task_dir(task_id) / f"worktree-{state['binding']}.json"
        coordination.atomic_json(packet_path, packet)
        entry.update(packet=env.rel(packet_path), packet_sha256=env.digest(packet_path))
        state['functions'][key] = entry
        _save(worker, state)
        # Packet is an ignored evidence snapshot, never a tracked authored file.
        coordination.atomic_json(_file(worker, LOCAL / key / 'packet.json'), packet)
        _record_attempts(va, manifest)
        return _result(entry, manifest)


def _current(state: dict, va: int) -> tuple[dict, dict]:
    entry = _entry(state, va)
    manifest = tasks._load(entry['task_id'])
    tasks.validate_current(manifest)
    return entry, manifest


def _source(worker: Path, va: int) -> Path:
    source = _file(worker, f'recovery/c/fn_{va:08x}.c')
    if not source.is_file() or source.stat().st_size > 1 << 20:
        raise ValueError('Write the claimed C source (maximum 1 MiB) before testing')
    return source


def _packet(worker: Path, va: int, entry: dict, *, restore: bool = False) -> None:
    original = (env.ROOT / entry['packet']).read_bytes()
    if env.digest_bytes(original) != entry['packet_sha256']:
        raise ValueError('Authoritative packet changed')
    local = _file(worker, LOCAL / f'fn_{va:08x}/packet.json')
    if restore:
        coordination.atomic_bytes(local, original)
    if not local.is_file() or env.digest(local) != entry['packet_sha256']:
        raise ValueError('Worker packet snapshot changed or is missing')


@contextmanager
def _claim_guard(va: int, entry: dict):
    with work.locked():
        claim = work.claims().get(f'fn_{va:08x}')
        if not claim or claim['owner'] != entry['owner']:
            raise ValueError('Central claim expired or replaced')
        yield


def _record_attempts(va: int, manifest: dict) -> None:
    for attempt in manifest['attempts']:
        if attempt['status'] == 'testing':
            continue
        event = f"{manifest['task_id']}:{attempt['number']}"
        report = {'status': attempt['status'], 'key': event}
        work.update(va, 'needs-review' if report['status'] == 'pass' else 'translation-defect',
                    'Worktree task ' + manifest['task_id'], 'Review retained task evidence',
                    report=report, event_id=event, preserve_hold=True)


def _report_feedback(report: dict) -> dict:
    """Expose a bounded failure without requiring access to another report file."""
    feedback = {'report_status': report['status'], 'first_failure': None}
    if report['status'] == 'pass':
        return feedback
    failure = None
    if report.get('problems'):
        failure = {'kind': 'problem', 'detail': str(report['problems'][0])}
    else:
        for opt, built in sorted(report.get('builds', {}).items()):
            if built.get('status') == 'pass':
                continue
            if built.get('status') == 'build-failed':
                failure = {'kind': 'build-failed', 'opt': opt,
                           'detail': built.get('stderr') or built.get('detail') or 'Compilation failed'}
            else:
                tested = built.get('difftest', {})
                if tested.get('mismatches'):
                    mismatch = tested['mismatches'][0]
                    failure = {'kind': 'mismatch', 'opt': opt, 'case': mismatch.get('case'),
                               'detail': json.dumps(mismatch)}
                else:
                    failure = {'kind': 'difftest', 'opt': opt,
                               **{k: tested[k] for k in ('coverage', 'inconclusive', 'code_as_data_cases')
                                  if k in tested},
                               'detail': 'Validation gate failed; inspect the numbered report for gate details'}
            break
    if failure is None:
        failure = {'kind': report['status'], 'detail': 'Validation failed; inspect the numbered report'}
    failure['detail'] = failure['detail'].strip()[:600]
    feedback['first_failure'] = failure
    return feedback


def _export_report(worker: Path, state: dict, va: int, manifest: dict, *, restore: bool = False) -> dict:
    entry = _entry(state, va)
    attempt = manifest['attempts'][-1]
    feedback = {}
    if 'report_path' in attempt:
        report_bytes = (env.ROOT / attempt['report_path']).read_bytes()
        if env.digest_bytes(report_bytes) != attempt['report_sha256']:
            raise ValueError('Authoritative report changed')
        feedback = _report_feedback(json.loads(report_bytes))
        entry['report'] = (LOCAL / f'fn_{va:08x}' / f"report-{attempt['number']}.json").as_posix()
        local = _file(worker, entry['report'])
        if local.exists() and not restore and env.digest(local) != attempt['report_sha256']:
            raise ValueError('Worker report changed; coordinator sync is required to restore it')
        coordination.atomic_bytes(local, report_bytes)
    _save(worker, state)
    return {**_result(entry, manifest), **feedback}


def submit(worker: Path, va: int, *, binding: str | None = None) -> dict:
    worker = Path(worker).resolve()
    with _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        if binding is not None and not secrets.compare_digest(binding, state['binding']):
            raise ValueError('Worktree binding mismatch')
        entry, manifest = _current(state, va)
        _packet(worker, va, entry)
        if manifest['status'] == 'submitted':
            # Reconcile a completed validation interrupted before report export.
            tasks.resolve_for_publish(entry['task_id'])
            if env.digest(_source(worker, va)) != manifest['attempts'][-1]['candidate_sha256']:
                raise ValueError('Worker source changed after submission')
            _record_attempts(va, manifest)
            return _export_report(worker, state, va, manifest)
        tasks._lease(manifest, entry['owner'], entry['token'])
        _budget(va)
        current_claim = work.claims().get(f'fn_{va:08x}')
        if not current_claim or current_claim['owner'] != entry['owner']:
            raise ValueError('Central claim expired or replaced')
        source = _source(worker, va)
        source_bytes = source.read_bytes()
        candidate = tasks._candidate(entry['task_id'], va)
        coordination.atomic_bytes(candidate, source_bytes)
        before = len(manifest['attempts'])
        try:
            manifest = tasks.submit(entry['task_id'], entry['owner'], entry['token'],
                                    ownership_guard=lambda: _claim_guard(va, entry))
        finally:
            # The task reserves an attempt durably before executing. A killed
            # process leaves that attempt for reclaim; it cannot reset the budget.
            manifest = tasks._load(entry['task_id'])
            if len(manifest['attempts']) > before:
                _record_attempts(va, manifest)
        entry.pop('review', None)
        result = _export_report(worker, state, va, manifest)
        if source.read_bytes() != source_bytes:
            raise ValueError('Worker source changed during validation; review is forbidden until it matches')
        return result


def _submitted(worker: Path, state: dict, va: int) -> tuple[dict, dict]:
    entry = _entry(state, va)
    _packet(worker, va, entry)
    manifest = tasks.resolve_for_publish(entry['task_id'])
    attempt = manifest['attempts'][-1]
    if env.digest(_source(worker, va)) != attempt['candidate_sha256']:
        raise ValueError('Worker source changed after submission')
    if not entry.get('report') or env.digest(_file(worker, entry['report'])) != attempt['report_sha256']:
        raise ValueError('Worker report changed after submission')
    return entry, manifest


def sync(worker: Path, va: int) -> dict:
    """Coordinator restores pinned evidence copies after an interrupted export."""
    worker = Path(worker).resolve()
    with _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        entry, manifest = _current(state, va)
        _packet(worker, va, entry, restore=True)
        if manifest['status'] == 'submitted':
            tasks.resolve_for_publish(entry['task_id'])
            if env.digest(_source(worker, va)) != manifest['attempts'][-1]['candidate_sha256']:
                raise ValueError('Worker source changed after submission')
        _record_attempts(va, manifest)
        return _export_report(worker, state, va, manifest, restore=True) if manifest['attempts'] else _result(entry, manifest)


def review(worker: Path, va: int, *, reviewer: str, reason: str,
           source_sha256: str, report_sha256: str, nonreturning_reason: str | None = None) -> dict:
    from . import screen

    worker = Path(worker).resolve()
    if nonreturning_reason is not None and not nonreturning_reason.strip():
        raise ValueError('Nonreturning review requires specific original instruction evidence')
    if not reviewer.strip() or not reason.strip():
        raise ValueError('Review requires reviewer identity and instruction-evidence justification')
    with coordination.writer(), _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        entry, manifest = _submitted(worker, state, va)
        attempt = manifest['attempts'][-1]
        if source_sha256 != attempt['candidate_sha256'] or report_sha256 != attempt['report_sha256']:
            raise ValueError('Review hashes differ from the submitted source/report')
        screened = screen.run(va)
        path = env.ROOT / screened['receipt']
        return_screen = {'receipt': screened['receipt'], 'sha256': env.digest(path),
                         'nonreturning_reason': nonreturning_reason}
        gate = json.loads((env.ROOT / attempt['report_path']).read_bytes())
        screen.require_promotion(va, gate, return_screen)
        with work.locked():
            records = work.read()['functions']
            record = records.get(f'fn_{va:08x}', {})
            if record.get('state') in work.HARD_HOLDS:
                raise ValueError(f"Function has a review hold: {record['state']}; {record.get('retry_when', '')}")
            generation = record.get('hold_generation', 0)
            entry['review'] = {'reviewer': reviewer, 'reason': reason, 'source_sha256': source_sha256,
                               'report_sha256': report_sha256, 'input_fingerprint': manifest['input_fingerprint'],
                               'hold_generation': generation, 'attempt_number': attempt['number'],
                               'return_screen': return_screen}
            _save(worker, state)
        return entry['review']


def revise(worker: Path, va: int, *, reason: str) -> dict:
    """Return a reviewed submission for editing without clearing its attempts."""
    worker = Path(worker).resolve()
    with coordination.writer(), _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        entry = _entry(state, va)
        manifest = tasks.revise(entry['task_id'], reason)
        entry.pop('review', None)
        _save(worker, state)
        work.release(entry['owner'], [va])
        return _result(entry, manifest)


def publish(worker: Path, va: int) -> dict:
    worker = Path(worker).resolve()
    with coordination.writer(), _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        entry = _entry(state, va)
        if not entry.get('review'):
            raise ValueError('Integrator publication requires a recorded review')
        manifest = tasks._load(entry['task_id'])
        if manifest['status'] not in ('publishing', 'published'):
            entry, manifest = _submitted(worker, state, va)
            attempt = manifest['attempts'][-1]
            review = entry['review']
            if (review['source_sha256'] != attempt['candidate_sha256']
                    or review['report_sha256'] != attempt['report_sha256']
                    or review['input_fingerprint'] != manifest['input_fingerprint']
                    or review.get('attempt_number') != attempt['number']):
                raise ValueError('Stale review; review the current submission')
        manifest = tasks.publish(entry['task_id'], worktree_worker=worker)
        if manifest['status'] == 'published':
            work.release(entry['owner'], [va])
        return _result(entry, manifest)


def forward(va: str) -> int:
    """Run the pinned coordinator tooling, without inheriting writer authority."""
    binding = json.loads(_file(env.ROOT, LOCAL / 'binding.json').read_text(encoding='utf-8'))
    if binding.get('schema') != SCHEMA or binding.get('worker') != str(env.ROOT):
        raise ValueError('Invalid local worktree binding')
    child_env = dict(os.environ)
    for key in ('CMRE_WRITER_TOKEN', 'CMRE_WRITER_PATH', 'CMRE_COORDINATION_PATH', 'PYTHONPATH'):
        child_env.pop(key, None)
    return subprocess.run([sys.executable, '-m', 'cmre', 'worktree', 'submit', str(env.ROOT), va,
                           '--binding', binding['binding']], cwd=binding['coordinator'], env=child_env).returncode


def add_commands(sub) -> None:
    commands = sub.add_parser('worktree', help='same-machine isolated C workers via one coordinator').add_subparsers(
        dest='action', required=True)
    for action in ('bootstrap', 'claim', 'submit', 'sync', 'review', 'revise', 'publish'):
        p = commands.add_parser(action)
        p.add_argument('worker', type=Path)
        if action == 'bootstrap':
            p.add_argument('--align', action='store_true',
                           help='copy coordinator tooling into a fresh worker after checking for edits')
        if action != 'bootstrap':
            p.add_argument('va')
        if action == 'claim':
            p.add_argument('--owner', required=True)
            p.add_argument('--seconds', type=int, default=900)
        if action == 'submit':
            p.add_argument('--binding')
        if action == 'review':
            for key in ('reviewer', 'reason', 'source-sha256', 'report-sha256'):
                p.add_argument('--' + key, required=True)
            p.add_argument('--nonreturning-reason', help='original instruction evidence for intentional nonreturn')
        if action == 'revise':
            p.add_argument('--reason', required=True)
        p.set_defaults(func=command)
    p = commands.add_parser('test', help='worker: validate source through the bound coordinator')
    p.add_argument('va')
    p.set_defaults(func=command)


def command(a) -> int:
    from .cli import _va
    if a.action == 'test':
        return forward(a.va)
    args = [a.worker] if a.action == 'bootstrap' else [a.worker, _va(a.va)]
    options = {k: v for k, v in vars(a).items() if k in (
        'align', 'owner', 'seconds', 'binding', 'reviewer', 'reason', 'source_sha256', 'report_sha256',
        'nonreturning_reason')}
    result = globals()[a.action](*args, **options)
    if a.action == 'bootstrap':
        result = {k: result[k] for k in ('schema', 'worker', 'coordinator')}
    print(json.dumps(result, indent=2))
    if 'report_status' in result and result['report_status'] != 'pass':
        return 1
    return 1 if result.get('status') in ('claimed', 'exhausted', 'publication-failed') and a.action != 'claim' else 0
