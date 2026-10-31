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


def _code_identity(root: Path) -> str:
    files = sorted([*(root / 'cmre').glob('*.py'), *(root / 'recovery/include').rglob('*')])
    return tasks._hash({p.relative_to(root).as_posix(): env.digest(p) for p in files if p.is_file()})


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


def bootstrap(worker: Path) -> dict:
    worker = Path(worker).resolve()
    if worker.is_relative_to(env.ROOT) or env.ROOT.is_relative_to(worker):
        raise ValueError('Use a separate, non-nested Git worktree')
    with coordination.writer():
        _pins()
        coordinator_git, worker_git = _git_identity(env.ROOT), _git_identity(worker)
        if coordinator_git['common'] != worker_git['common']:
            raise ValueError('Worker must share the coordinator Git common directory')
        code = _code_identity(env.ROOT)
        if _code_identity(worker) != code:
            raise ValueError('Worker tooling/header snapshot differs from coordinator')
        # No links into authoritative assets, evidence or mutable recovery state.
        for relative in ('CM0102', LOCAL, 'recovery', 'recovery/c', 'recovery/hints',
                         'recovery/ledger', 'recovery/lifted', 'recovery/blocked'):
            _file(worker, relative)
        if _state_path(worker).exists():
            if not _file(worker, LOCAL / 'binding.json').exists():
                retained = json.loads(_state_path(worker).read_text(encoding='utf-8'))
                coordination.atomic_json(_file(worker, LOCAL / 'binding.json'),
                                         {k: retained[k] for k in ('schema', 'worker', 'coordinator', 'binding')})
            return _load(worker)
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
    events = record.get('report_events', {})
    attempts, counted_failures = 0, 0
    for path in (env.ANALYSIS / 'cmre/tasks').glob(f'c-{va:08x}-*/task.json'):
        manifest = json.loads(path.read_text(encoding='utf-8'))
        for attempt in manifest['attempts']:
            attempts += 1
            event = f"{manifest['task_id']}:{attempt['number']}"
            counted_failures += event in events and events[event] != 'pass'
    if attempts + record.get('failures', 0) - counted_failures >= tasks.MAX_ATTEMPTS:
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
        _budget(va)
        key = f'fn_{va:08x}'
        with work.locked():
            if key in work.claims():
                raise ValueError('Function already claimed; wait for expiry or integrator completion')
            manifest = tasks.prepare(va)
            task_id = manifest['task_id']
            central_owner = f"wt:{state['binding'][:16]}:{owner}"
            manifest = tasks.claim(task_id, central_owner, seconds)
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
                    report=report, event_id=event)


def _export_report(worker: Path, state: dict, va: int, manifest: dict, *, restore: bool = False) -> dict:
    entry = _entry(state, va)
    attempt = manifest['attempts'][-1]
    if 'report_path' in attempt:
        report_bytes = (env.ROOT / attempt['report_path']).read_bytes()
        if env.digest_bytes(report_bytes) != attempt['report_sha256']:
            raise ValueError('Authoritative report changed')
        entry['report'] = (LOCAL / f'fn_{va:08x}' / f"report-{attempt['number']}.json").as_posix()
        local = _file(worker, entry['report'])
        if local.exists() and not restore and env.digest(local) != attempt['report_sha256']:
            raise ValueError('Worker report changed; coordinator sync is required to restore it')
        coordination.atomic_bytes(local, report_bytes)
    _save(worker, state)
    return _result(entry, manifest)


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
           source_sha256: str, report_sha256: str) -> dict:
    worker = Path(worker).resolve()
    if not reviewer.strip() or not reason.strip():
        raise ValueError('Review requires reviewer identity and instruction-evidence justification')
    with coordination.writer(), _worker_lock(worker), _function_lock(va):
        state = _load(worker)
        entry, manifest = _submitted(worker, state, va)
        attempt = manifest['attempts'][-1]
        if source_sha256 != attempt['candidate_sha256'] or report_sha256 != attempt['report_sha256']:
            raise ValueError('Review hashes differ from the submitted source/report')
        entry['review'] = {'reviewer': reviewer, 'reason': reason, 'source_sha256': source_sha256,
                           'report_sha256': report_sha256, 'input_fingerprint': manifest['input_fingerprint']}
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
                    or review['input_fingerprint'] != manifest['input_fingerprint']):
                raise ValueError('Stale review; review the current submission')
        manifest = tasks.publish(entry['task_id'])
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
        'owner', 'seconds', 'binding', 'reviewer', 'reason', 'source_sha256', 'report_sha256')}
    result = globals()[a.action](*args, **options)
    if a.action == 'bootstrap':
        result = {k: result[k] for k in ('schema', 'worker', 'coordinator')}
    print(json.dumps(result, indent=2))
    return 1 if result.get('status') in ('claimed', 'exhausted', 'publication-failed') and a.action != 'claim' else 0
