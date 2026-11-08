"""Exercise supervision with real local fake processes, never paid providers."""
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import threading
import time

import pytest

MODULE = Path(__file__).resolve().parents[1] / 'cmre_provider_worker.py'
spec = importlib.util.spec_from_file_location('provider_worker', MODULE)
worker = importlib.util.module_from_spec(spec)
spec.loader.exec_module(worker)


@pytest.fixture
def setup(tmp_path):
    root = tmp_path / 'repo'
    root.mkdir()
    subprocess.run(['git', 'init', '-q', str(root)], check=True)
    (root / 'candidate.c').write_text('before')
    (root / 'header.h').write_text('fixed')
    (root / '.gitignore').write_text('ignored/\n')
    return root, tmp_path / 'run'


def launch(setup, code, **extra):
    root, run_dir = setup
    value = {'directory': str(root), 'allowed_files': ['candidate.c'], 'owner': 'fake',
             'command': [sys.executable, '-c', code], 'timeout_seconds': 10, **extra}
    path = root.parent / 'spec.json'
    path.write_text(json.dumps(value))
    return worker.run(path, run_dir)


DONE = "print('{\"type\":\"step_finish\",\"sessionID\":\"test\",\"part\":{\"reason\":\"stop\",\"messageID\":\"m1\"}}',flush=True)"
SLEEPING_CHILD_TREE = (
    "import subprocess,sys,time; from pathlib import Path; "
    "p=subprocess.Popen([sys.executable,'-c','import time;time.sleep(60)']); "
    "marker=Path('ignored/child.tmp');marker.write_text(str(p.pid));"
    "marker.replace('ignored/child');time.sleep(60)"
)


@pytest.mark.skipif(os.name != 'nt', reason='Windows open-reader replacement contract')
def test_receipt_replace_waits_for_an_open_windows_reader(tmp_path, monkeypatch):
    target = tmp_path / 'receipt.json'
    target.write_text('{"state":"starting"}')
    denied = threading.Event()
    errors = []
    original_replace = Path.replace

    def observe_denial(path, destination):
        try:
            return original_replace(path, destination)
        except PermissionError:
            denied.set()
            raise

    monkeypatch.setattr(Path, 'replace', observe_denial)

    def writer():
        try:
            worker.write_json(target, {'state': 'running'})
        except Exception as error:
            errors.append(error)

    thread = threading.Thread(target=writer)
    try:
        with target.open() as reader:
            thread.start()
            assert denied.wait(3), 'The real Windows reader did not exercise replacement denial'
            assert json.load(reader) == {'state': 'starting'}
    finally:
        thread.join(3)
    assert not thread.is_alive()
    assert not errors
    assert json.loads(target.read_text()) == {'state': 'running'}


@pytest.mark.skipif(os.name != 'nt', reason='Windows replacement retry contract')
def test_permanent_receipt_denial_is_bounded_and_retains_both_files(tmp_path, monkeypatch):
    target = tmp_path / 'receipt.json'
    target.write_text('{"state":"starting"}')
    moments = iter((0, 1, 3))
    monkeypatch.setattr(worker.time, 'monotonic', lambda: next(moments))
    monkeypatch.setattr(worker.time, 'sleep', lambda _: None)
    calls = []

    def deny_replace(path, destination):
        calls.append((path, destination))
        error = PermissionError('synthetic sharing denial')
        error.winerror = 32
        raise error

    monkeypatch.setattr(Path, 'replace', deny_replace)
    with pytest.raises(PermissionError):
        worker.write_json(target, {'state': 'running'})
    assert len(calls) == 2
    assert json.loads(target.read_text()) == {'state': 'starting'}
    assert json.loads(target.with_suffix('.tmp').read_text()) == {'state': 'running'}


def test_unrelated_receipt_error_is_not_retried(tmp_path, monkeypatch):
    target = tmp_path / 'receipt.json'
    calls = []

    def fail_replace(path, destination):
        calls.append(path)
        error = PermissionError('synthetic unrelated error')
        error.winerror = 87
        raise error

    monkeypatch.setattr(Path, 'replace', fail_replace)
    with pytest.raises(PermissionError):
        worker.write_json(target, {'state': 'running'})
    assert len(calls) == 1


def test_success_audits_untracked_source_and_unknown_usage(setup):
    result = launch(setup, "from pathlib import Path; Path('candidate.c').write_text('after');" + DONE)
    assert result['state'] == 'completed'
    assert result['changed_files'] == ['candidate.c']
    assert result['calls'][0]['input_tokens'] is None
    assert result['source_after']['candidate.c'] != result['source_before']['candidate.c']
    assert worker.inspect(setup[1], 'collect')['state'] == 'completed'


@pytest.mark.parametrize('code,state', [
    ("print('not-json')", 'incomplete'),
    ("print('{\"type\":')", 'incomplete'),
    ("print('{\"type\":\"text\"}')", 'incomplete'),
    ("print('{\"type\":\"step_finish\",\"part\":null}')", 'incomplete'),
    ("print('{\"type\":\"step_finish\",\"part\":[]}')", 'incomplete'),
    ("print('{\"type\":\"step_finish\",\"part\":{\"tokens\":[]}}')", 'incomplete'),
    (DONE + ';raise SystemExit(2)', 'incomplete'),
    ("print('{\"type\":\"error\",\"error\":\"quota\"}');" + DONE, 'incomplete'),
    ("from pathlib import Path; Path('header.h').write_text('changed');" + DONE, 'scope_violation'),
    ("from pathlib import Path; Path('extra.c').write_text('extra');" + DONE, 'scope_violation'),
    ("from pathlib import Path; Path('header.h').unlink();" + DONE, 'scope_violation'),
])
def test_bad_completion_and_scope(setup, code, state):
    assert launch(setup, code)['state'] == state


def test_timeout_kills_surviving_descendant(setup):
    root, _ = setup
    (root / 'ignored').mkdir()
    result = launch(setup, SLEEPING_CHILD_TREE, timeout_seconds=0.8)
    pid = int((root / 'ignored/child').read_text())
    deadline = time.monotonic() + 3
    while worker.identity(pid) and time.monotonic() < deadline:
        time.sleep(.05)
    assert result['state'] == 'timeout'
    assert worker.identity(pid) is None
    assert worker.identity(result['pid']) is None


def test_cancellation_and_live_collection_refusal(setup):
    root, run_dir = setup
    value = {'directory': str(root), 'allowed_files': ['candidate.c'], 'owner': 'fake',
             'command': [sys.executable, '-c', 'import time; time.sleep(60)']}
    path = root.parent / 'spec.json'
    path.write_text(json.dumps(value))
    results = []
    thread = threading.Thread(target=lambda: results.append(worker.run(path, run_dir)))
    thread.start()
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        try:
            if worker.inspect(run_dir, 'status')['state'] == 'running':
                break
        except FileNotFoundError:
            pass
        time.sleep(.05)
    try:
        with pytest.raises(ValueError, match='not reconciled'):
            worker.inspect(run_dir, 'collect')
    finally:
        worker.inspect(run_dir, 'cancel')
        thread.join(10)
    assert not thread.is_alive()
    assert results[0]['state'] == 'cancelled'


def test_step_limit(setup):
    result = launch(setup, "import time;print('{\"type\":\"step_start\"}',flush=True);time.sleep(60)", max_steps=0)
    assert result['state'] == 'step_limit'


def test_pins_include_ignored_evidence(setup):
    root, _ = setup
    (root / 'ignored').mkdir()
    packet = root / 'ignored/packet'
    packet.write_text('pinned')
    result = launch(setup, "from pathlib import Path;Path('ignored/packet').write_text('altered');" + DONE,
                    pinned_files=[str(packet)])
    assert result['state'] == 'input_drift'
    assert result['pin_drift'] == [str(packet.resolve())]


def test_refuses_duplicate_run_and_path_escape(setup):
    launch(setup, DONE)
    with pytest.raises(FileExistsError):
        launch(setup, DONE)
    with pytest.raises(ValueError, match='Invalid allowed path'):
        launch(setup, DONE, allowed_files=['../escape.c'])


def test_restart_never_trusts_recycled_controller_pid(setup):
    _, run_dir = setup
    launch(setup, DONE)
    receipt = run_dir / 'receipt.json'
    record = json.loads(receipt.read_text())
    record.update(state='running', controller_pid=os.getpid(), controller_identity='old identity')
    receipt.write_text(json.dumps(record))
    assert worker.inspect(run_dir, 'status')['state'] == 'orphaned-needs-reconciliation'
    with pytest.raises(ValueError):
        worker.inspect(run_dir, 'collect')


def test_token_accounting_deduplicates_events_and_keeps_cache_separate():
    event = {'type': 'step_finish', 'sessionID': 'test', 'part': {'messageID': 'm1',
             'reason': 'stop', 'tokens': {'input': 12, 'output': 3, 'cache': {'read': 8, 'write': 0}}}}
    result = worker.summarize([event, event])
    assert len(result['calls']) == 1
    assert result['calls'][0]['input_tokens'] == 12
    assert result['calls'][0]['cache_read_tokens'] == 8


def test_interrupted_export_zero_placeholders_are_unknown():
    message = {'id': 'm1', 'providerID': 'test', 'modelID': 'model',
               'time': {'created': 1}, 'tokens': {'input': 0, 'output': 0,
               'cache': {'read': 0, 'write': 0}}, 'cost': 0}
    call = worker.exported_calls([message])[0]
    assert call['input_tokens'] is None and call['cache_read_tokens'] is None
    message.update(time={'created': 1, 'completed': 2}, finish='stop')
    assert worker.exported_calls([message])[0]['input_tokens'] == 0


@pytest.mark.parametrize('sessions', [[], ['s1', 's2']])
def test_provider_completion_requires_one_session(setup, sessions):
    root, run_dir = setup
    run_dir.mkdir()
    records = [{'type': 'step_finish', 'part': {'reason': 'stop'}, 'sessionID': s} for s in sessions]
    if not records:
        records = [{'type': 'step_finish', 'part': {'reason': 'stop'}}]
    (run_dir / 'stdout.jsonl').write_text('\n'.join(json.dumps(e) for e in records))
    result = worker.finalize({'allowed_files': ['candidate.c'], 'model': 'test/model'}, root,
                             run_dir, {}, worker.snapshot(root), {},
                             {'started': time.time(), 'exit_code': 0}, None)
    assert result['state'] == 'session_identity_error'


def test_finalization_failure_retains_exit_and_terminal_receipt(setup, monkeypatch):
    original = worker.snapshot
    calls = []

    def fail_after(root):
        calls.append(root)
        if len(calls) > 1:
            raise ValueError('Scope contains redirected path: extra.c')
        return original(root)

    monkeypatch.setattr(worker, 'snapshot', fail_after)
    result = launch(setup, DONE)
    assert result['state'] == 'finalization_error'
    assert result['exit_code'] == 0
    assert 'redirected path' in result['error']
    assert worker.inspect(setup[1], 'collect')['state'] == 'finalization_error'


@pytest.mark.skipif(os.name != 'nt', reason='Windows job assignment contract')
def test_windows_worker_cannot_execute_before_job_assignment(setup, monkeypatch):
    original = worker.ProcessTree.__init__

    def delayed_assignment(self, process):
        time.sleep(.2)
        assert not (setup[0] / 'candidate-executed').exists()
        original(self, process)

    monkeypatch.setattr(worker.ProcessTree, '__init__', delayed_assignment)
    result = launch(setup, "from pathlib import Path;Path('candidate-executed').write_text('ran');" + DONE)
    assert result['state'] == 'scope_violation'
    assert (setup[0] / 'candidate-executed').exists()


@pytest.mark.skipif(os.name != 'nt', reason='Windows kill-on-close job contract')
def test_killed_controller_stops_children_and_refuses_restart_collection(setup):
    root, run_dir = setup
    (root / 'ignored').mkdir()
    spec_path = root.parent / 'spec.json'
    spec_path.write_text(json.dumps({'directory': str(root), 'allowed_files': ['candidate.c'],
                                    'owner': 'killed-controller',
                                    'command': [sys.executable, '-c', SLEEPING_CHILD_TREE]}))
    process = subprocess.Popen([sys.executable, str(MODULE), 'run', str(spec_path), str(run_dir)],
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        deadline = time.monotonic() + 10
        while not (root / 'ignored/child').exists() and time.monotonic() < deadline:
            time.sleep(.05)
        child_pid = int((root / 'ignored/child').read_text())
        process.kill()
        process.wait(timeout=10)
        deadline = time.monotonic() + 3
        while worker.identity(child_pid) and time.monotonic() < deadline:
            time.sleep(.05)
        assert worker.identity(child_pid) is None
        assert worker.inspect(run_dir, 'status')['state'] == 'orphaned-needs-reconciliation'
        with pytest.raises(ValueError):
            worker.inspect(run_dir, 'collect')
    finally:
        if process.poll() is None:
            process.kill()
            process.wait(timeout=10)
