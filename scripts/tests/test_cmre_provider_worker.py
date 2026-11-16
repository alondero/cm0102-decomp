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


@pytest.mark.skipif(os.name != 'nt', reason='Windows process query contract')
@pytest.mark.parametrize('stage', ['open', 'exit-code'])
def test_denied_process_query_is_unknown_not_dead(monkeypatch, stage):
    from types import SimpleNamespace

    class Function:
        def __init__(self, result):
            self.result = result

        def __call__(self, *args):
            return self.result

    kernel = SimpleNamespace(OpenProcess=Function(0 if stage == 'open' else 1),
        GetExitCodeProcess=Function(0), GetProcessTimes=Function(0), CloseHandle=Function(1))
    monkeypatch.setattr(worker.ctypes, 'WinDLL', lambda *args, **kwargs: kernel)
    monkeypatch.setattr(worker.ctypes, 'get_last_error', lambda: 5)
    with pytest.raises(PermissionError):
        worker.identity(123)


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


def test_timeout_kills_surviving_descendant(setup, monkeypatch):
    root, _ = setup
    (root / 'ignored').mkdir()
    # Start the timeout only after the child exists, so host load during Python
    # startup cannot turn a containment test into a missing-marker failure.
    real_time = time.time
    monkeypatch.setattr(worker.time, 'time', lambda: real_time() +
                        (60 if (root / 'ignored/child').exists() else 0))
    result = launch(setup, SLEEPING_CHILD_TREE, timeout_seconds=30)
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


@pytest.mark.parametrize('finish', ['unknown', 'other', 'error', 'unexpected', None])
def test_completed_timestamp_without_known_finish_keeps_initialized_usage_unknown(finish):
    message = {'id':'m1','providerID':'test','modelID':'model',
        'time':{'created':1,'completed':2},'finish':finish,'cost':0,
        'tokens':{'input':0,'output':0,'reasoning':0,'cache':{'read':0,'write':0}}}
    call = worker.exported_calls([message])[0]
    assert call['usage_observed'] is False and call['cost'] is None
    assert all(value is None for key,value in call.items() if key.endswith('_tokens'))
    assert message['tokens']['input'] == 0 and message['finish'] == finish


def test_completed_length_export_retains_reasoning_and_deduplicates():
    message = {'id': 'm1', 'providerID': 'test', 'modelID': 'model',
               'time': {'completed': 2}, 'finish': 'length',
               'tokens': {'input': 12, 'output': 32000, 'reasoning': 900,
                          'cache': {'read': 8, 'write': 0}}}
    calls = worker.exported_calls([message, message])
    assert len(calls) == 1
    assert calls[0]['reasoning_tokens'] == 900
    assert calls[0]['output_tokens'] == 32000


@pytest.mark.parametrize('content', ['packet'.encode('utf-16'), b'p\x00acket'])
def test_unreadable_packet_stops_before_process_launch(setup, content):
    root, run_dir = setup
    (root / 'packet.txt').write_bytes(content)
    with pytest.raises(ValueError, match='UTF-8|NUL'):
        launch(setup, "from pathlib import Path;Path('candidate.c').write_text('launched');" + DONE,
               phase='smoke', required_text_files={'packet.txt': 'text'},
               permission_files=['header.h'], smoke_file='candidate.c')
    assert (root / 'candidate.c').read_text() == 'before'
    assert not run_dir.exists()


def test_denied_read_cannot_be_overridden_by_success_phrase(setup):
    result = launch(setup, "from pathlib import Path;Path('candidate.c').write_text('READ_WRITE_OK');" + DONE,
                    phase='smoke', required_text_files={'header.h': 'text'},
                    permission_files=['header.h'], smoke_file='candidate.c')
    assert result['state'] == 'readiness_failed'


def test_work_without_successful_smoke_stops_before_process_launch(setup):
    with pytest.raises(ValueError, match='smoke'):
        launch(setup, DONE, phase='work', required_text_files={'header.h': 'text'},
               permission_files=['header.h'], smoke_file='candidate.c')
    assert not setup[1].exists()


@pytest.fixture
def terminal_export(setup):
    root, run_dir = setup
    run_dir.mkdir()
    spec = {'directory': str(root), 'executable': sys.executable, 'model': 'test/model'}
    worker.write_json(run_dir / 'spec.json', spec)
    record = {'state': 'export_failed', 'model_requested': 'test/model', 'session_ids': ['s1'],
              'pid': 99999999, 'controller_pid': 99999998, 'process_identity': 'old',
              'controller_identity': 'old', 'readiness_identity': {
                  'executable_sha256': worker.digest(sys.executable)}, 'calls': []}
    worker.write_json(run_dir / 'receipt.json', record)
    (run_dir / 'session-export.json').write_bytes(b'')
    return run_dir


def test_export_recovery_is_versioned_and_never_changes_original(terminal_export, monkeypatch):
    before = worker.digest(terminal_export / 'receipt.json')
    calls = []

    def export(executable, directory, environment, session, output, timeout=30):
        calls.append((session, timeout))
        worker.write_json(output / 'session-export.json', {'info': {'id': session}, 'messages': [
            {'info': {'role': 'assistant', 'sessionID': session, 'id': 'm1', 'providerID': 'test',
                      'modelID': 'model', 'time': {'completed': 2}, 'finish': 'stop', 'tokens': {}}}]})
        return worker.artifacts.assistant_messages(output / 'session-export.json', session)

    monkeypatch.setattr(worker, 'collect_export', export)
    result = worker.collect(terminal_export, recover_export=True)
    assert result['usage_status'] == 'collected'
    assert result['run_state'] == 'export_failed'
    assert calls == [('s1', 120)]
    assert worker.digest(terminal_export / 'receipt.json') == before
    assert (terminal_export / 'session-export.json').read_bytes() == b''
    assert (terminal_export / 'collections/0001/receipt.json').is_file()
    # Collection projects evidence; it never changes the original run to completed.
    assert worker.inspect(terminal_export, 'collect')['state'] == 'export_failed'


@pytest.mark.parametrize('payload', [b'', b'{"info":'])
def test_empty_or_truncated_terminal_export_is_explicitly_unknown(terminal_export, payload):
    (terminal_export / 'session-export.json').write_bytes(payload)
    result = worker.collect(terminal_export)
    assert result['usage_status'] == 'unknown'
    assert result['calls'] is None
    assert result['error']


def test_export_timeout_preserves_failed_version(terminal_export, monkeypatch):
    def timeout(*args, **kwargs):
        (args[4] / 'session-export.json').write_bytes(b'{')
        raise subprocess.TimeoutExpired('export', 120)

    monkeypatch.setattr(worker, 'collect_export', timeout)
    result = worker.collect(terminal_export, recover_export=True)
    assert result['usage_status'] == 'unknown'
    assert (terminal_export / 'collections/0001/session-export.json').read_bytes() == b'{'


def test_recovery_refuses_live_controller(terminal_export, monkeypatch):
    monkeypatch.setattr(worker, 'identity', lambda pid: 'old' if pid == 99999998 else None)
    result = worker.collect(terminal_export, recover_export=True)
    assert result['usage_status'] == 'unknown' and 'live' in result['error']
    assert not (terminal_export / 'collections/0001/session-export.json').exists()


def test_control_projection_and_omission_marker_are_distinct():
    result = worker.artifacts.control_projection({'schema': 'cmre-control-bundle/2',
                                                  'status': 'controls-complete', 'readiness': 'proposal'})
    assert result == {'controls_complete': True, 'readiness': 'proposal', 'acceptance_ready': False}
    rows = worker.artifacts.mismatch_projection([
        {'case': 2, 'regs': {'eax': [0, 1]}}, {'case': 1184, 'note': 'further mismatches omitted'}])
    assert len(rows['witnesses']) == 1 and rows['truncated']


def tool_event(root, tool, name, content, status='completed'):
    state = {'status': status, 'input': {'filePath': str(root / name)}, 'metadata': {'truncated': False}}
    if tool == 'read':
        state['metadata']['display'] = {'type': 'file', 'lineStart': 1,
            'lineEnd': len(content.splitlines()), 'totalLines': len(content.splitlines()), 'text': content}
    else:
        state['input']['content'] = content
    return {'type': 'tool_use', 'sessionID': 'test', 'part': {'tool': tool, 'state': state}}


def test_call_projection_keeps_full_bits_and_absent_receiver_distinct():
    rows = [{'function_va': 0x521b30, 'ecx_argument': 0x03001000,
             'stack_args': [0x03005000, 0xabcd00ff, 0, 0xffffffff]},
            {'function_va': 0x48e380, 'ecx_argument': None, 'stack_args': []}]
    assert worker.artifacts.call_projection(rows) == [
        {'function_va': '0x00521b30', 'ecx_argument': '0x03001000',
         'stack_args': ['0x03005000', '0xabcd00ff', '0x00000000', '0xffffffff']},
        {'function_va': '0x0048e380', 'ecx_argument': None, 'stack_args': []}]
    assert rows[0]['stack_args'][1] == 0xabcd00ff


@pytest.mark.parametrize('field,value', [('function_va', True), ('function_va', -1),
    ('ecx_argument', 0x100000000), ('stack_args', [False]), ('stack_args', ['0xabcd00ff'])])
def test_call_projection_rejects_ambiguous_or_out_of_range_values(field, value):
    row = {'function_va': 0x521b30, 'ecx_argument': None, 'stack_args': []}
    row[field] = value
    with pytest.raises(ValueError):
        worker.artifacts.call_projection([row])


def test_call_projection_refuses_missing_fields_and_nonlists():
    for rows in ({}, [None], [{'function_va': 0x521b30, 'stack_args': []}]):
        with pytest.raises(ValueError):
            worker.artifacts.call_projection(rows)


def test_call_projection_cli_rejects_duplicate_fields(tmp_path):
    path = tmp_path / 'calls.json'
    path.write_text('[{"function_va":5380912,"ecx_argument":null,"stack_args":[1],"stack_args":[2]}]',
                    encoding='utf-8')
    result = subprocess.run([sys.executable, str(MODULE), 'project', 'calls', str(path)], capture_output=True, text=True)
    assert result.returncode != 0 and not result.stdout
    assert 'Duplicate JSON key' in result.stderr


def test_call_projection_cli_outputs_exact_recorded_words(tmp_path):
    path = tmp_path / 'calls.json'
    path.write_text(json.dumps([{'function_va': 0x521b30, 'ecx_argument': 0x03001000,
                                 'stack_args': [0x03005000, 0xabcd00ff]}]), encoding='utf-8')
    result = subprocess.run([sys.executable, str(MODULE), 'project', 'calls', str(path)], capture_output=True, text=True)
    assert result.returncode == 0
    assert json.loads(result.stdout) == [{'function_va': '0x00521b30', 'ecx_argument': '0x03001000',
                                        'stack_args': ['0x03005000', '0xabcd00ff']}]


def ready_smoke(setup, **overrides):
    root, _ = setup
    values = [tool_event(root, 'read', 'header.h', 'fixed'),
              tool_event(root, 'write', 'candidate.c', 'READ_WRITE_OK')]
    code = ("from pathlib import Path;Path('candidate.c').write_text('READ_WRITE_OK');"
            + ';'.join(f'print({json.dumps(json.dumps(e))})' for e in values) + ';' + DONE)
    return launch(setup, code, phase='smoke', required_text_files={'header.h': 'text'},
                  permission_files=['.gitignore'], smoke_file='candidate.c', **overrides)


def paired_specs(setup, *, smoke_code=None, work_code=DONE):
    root, run_dir = setup
    values = [tool_event(root, 'read', 'header.h', 'fixed'),
              tool_event(root, 'write', 'candidate.c', 'READ_WRITE_OK')]
    smoke_code = smoke_code or (
        "from pathlib import Path;Path('candidate.c').write_text('READ_WRITE_OK');"
        + ';'.join(f'print({json.dumps(json.dumps(e))})' for e in values) + ';' + DONE)
    base = {'directory': str(root), 'owner': 'fake-pair', 'allowed_files': ['candidate.c'],
            'required_text_files': {'header.h': 'text'}, 'permission_files': ['.gitignore'],
            'smoke_file': 'candidate.c', 'timeout_seconds': 10}
    smoke, work = root.parent / 'smoke.json', root.parent / 'work.json'
    worker.write_json(smoke, dict(base, phase='smoke', command=[sys.executable, '-c', smoke_code]))
    worker.write_json(work, dict(base, phase='work', command=[sys.executable, '-c', work_code]))
    return smoke, work, run_dir


def test_pair_runs_real_scoped_fake_stages_and_retains_both(setup):
    smoke, work, run_dir = paired_specs(setup)
    result = worker.pair(smoke, work, run_dir)
    assert result['state'] == 'completed'
    assert worker.inspect(run_dir / 'smoke', 'collect')['readiness']['passed']
    assert worker.inspect(run_dir / 'work', 'collect')['state'] == 'completed'
    with pytest.raises(FileExistsError):
        worker.pair(smoke, work, run_dir)


def test_pair_never_dispatches_work_after_failed_smoke(setup):
    smoke, work, run_dir = paired_specs(setup, smoke_code=DONE)
    assert worker.pair(smoke, work, run_dir)['state'] == 'readiness_failed'
    assert not (run_dir / 'work').exists()


def test_pair_rejects_scope_mismatch_before_launch(setup):
    smoke, work, run_dir = paired_specs(setup)
    value = worker.artifacts.load(work)
    value['allowed_files'].append('other.c')
    worker.write_json(work, value)
    with pytest.raises(ValueError, match='identity'):
        worker.pair(smoke, work, run_dir)
    assert not run_dir.exists()


def test_pair_pins_work_spec_before_smoke(setup):
    smoke, work, run_dir = paired_specs(setup)
    value = worker.artifacts.load(smoke)
    value['command'][-1] += f';from pathlib import Path;Path({str(work)!r}).write_text("changed")'
    worker.write_json(smoke, value)
    result = worker.pair(smoke, work, run_dir)
    assert result['state'] == 'input_drift'
    assert not (run_dir / 'work').exists()


def test_pair_cancel_reaches_live_stage_and_prevents_work(setup):
    smoke, work, run_dir = paired_specs(setup, smoke_code='import time;time.sleep(60)')
    results = []
    thread = threading.Thread(target=lambda: results.append(worker.pair(smoke, work, run_dir)))
    thread.start()
    try:
        deadline = time.monotonic() + 5
        while not (run_dir / 'smoke/receipt.json').exists():
            assert time.monotonic() < deadline
            time.sleep(.02)
        assert worker.inspect(run_dir, 'cancel')['cancel_requested']
    finally:
        thread.join(5)
    assert not thread.is_alive()
    assert results[0]['state'] == 'cancelled'
    assert not (run_dir / 'work').exists()


def test_packet_pretty_json_keeps_raw_and_output_hashes(tmp_path):
    source = tmp_path / 'compact.json'
    source.write_text('{"a":[1,2],"b":"ok"}', encoding='utf-8')
    target = tmp_path / 'packet'
    result = worker.artifacts.prepare_packet(
        [{'source': str(source), 'name': 'input.json', 'kind': 'json'}], target)
    assert result['files']['input.json']['source_sha256'] == worker.digest(source)
    assert result['files']['input.json']['sha256'] == worker.digest(target / 'input.json')
    assert json.loads((target / 'input.json').read_text()) == json.loads(source.read_text())
    assert source.read_bytes() != (target / 'input.json').read_bytes()
    with pytest.raises(FileExistsError):
        worker.artifacts.prepare_packet([], target)


@pytest.mark.parametrize('content,kind', [
    (b'\xff\xfea\x00', 'text'), (b'{"a":NaN}', 'json'),
    (b'{"a":1,"a":2}', 'json'), (b'{broken', 'json'), (b'a' * 1801, 'text')])
def test_packet_rejects_unprovable_inputs_before_creating_target(tmp_path, content, kind):
    source = tmp_path / 'input'; source.write_bytes(content)
    with pytest.raises(ValueError):
        worker.artifacts.prepare_packet([{'source': str(source), 'name': 'input', 'kind': kind}], tmp_path / 'out')
    assert not (tmp_path / 'out').exists()


@pytest.mark.parametrize('names', [('a', 'A'), ('../a', 'b'), ('a\\b', 'c'), ('manifest.json', 'b'),
                                 ('CON', 'b'), ('NUL.txt', 'b'), ('CON.foo.bar', 'b'),
                                 ('COM\u00b9', 'b'), ('a?', 'b'), ('a\0', 'b')])
def test_packet_rejects_aliased_or_redirected_destinations(tmp_path, names):
    source = tmp_path / 'input'; source.write_text('ok')
    with pytest.raises(ValueError):
        worker.artifacts.prepare_packet(
            [{'source': str(source), 'name': name, 'kind': 'text'} for name in names], tmp_path / 'out')


def test_valid_smoke_and_work_with_ignored_harmless_write(setup):
    root, run_dir = setup
    assert ready_smoke(setup)['state'] == 'completed'
    # A new run has the same exact input, model and permission binding.
    work_spec = worker.artifacts.load(run_dir / 'spec.json')
    work_spec.update(phase='work', smoke_run=str(run_dir), command=[sys.executable, '-c', DONE])
    worker.write_json(root.parent / 'work.json', work_spec)
    assert worker.run(root.parent / 'work.json', root.parent / 'work')['state'] == 'completed'


@pytest.mark.parametrize('target', ['header.h', '.gitignore'])
def test_smoke_is_invalid_after_input_or_permission_change(setup, target):
    root, run_dir = setup
    assert ready_smoke(setup)['state'] == 'completed'
    spec = worker.artifacts.load(run_dir / 'spec.json')
    spec.update(phase='work', smoke_run=str(run_dir))
    (root / target).write_text('changed')
    worker.write_json(root.parent / 'work.json', spec)
    with pytest.raises(ValueError, match='smoke identity'):
        worker.run(root.parent / 'work.json', root.parent / 'work')
    assert not (root.parent / 'work').exists()


def test_inherited_environment_change_requires_new_smoke(setup, monkeypatch):
    root, run_dir = setup
    assert ready_smoke(setup)['state'] == 'completed'
    spec = worker.artifacts.load(run_dir / 'spec.json')
    spec.update(phase='work', smoke_run=str(run_dir))
    worker.write_json(root.parent / 'work.json', spec)
    monkeypatch.setenv('CMRE_INHERITED_TEST_FLAG', '{"permission":{"read":"deny"}}')
    with pytest.raises(ValueError, match='smoke identity'):
        worker.run(root.parent / 'work.json', root.parent / 'work')
    assert not (root.parent / 'work').exists()


def test_launcher_session_variables_are_not_inherited(setup, monkeypatch):
    """A provider child must not resolve an enclosing session's OpenCode config."""
    root, run_dir = setup
    assert ready_smoke(setup)['state'] == 'completed'
    spec = worker.artifacts.load(run_dir / 'spec.json')
    for name in worker.artifacts.LAUNCHER_ENVIRONMENT:
        monkeypatch.setenv(name, f'inherited-{name}')
    environment = worker.artifacts.child_environment(spec)
    for name in worker.artifacts.LAUNCHER_ENVIRONMENT:
        assert name not in environment
    # An explicit profile value still wins and stays part of the identity.
    spec['environment'] = {'OPENCODE_CONFIG': 'C:/profile/opencode.json'}
    assert worker.artifacts.child_environment(spec)['OPENCODE_CONFIG'] == 'C:/profile/opencode.json'
    identity = worker.artifacts.readiness_identity(spec, root)
    monkeypatch.setenv('OPENCODE_CONFIG', 'C:/parent/opencode.json')
    assert worker.artifacts.readiness_identity(spec, root) == identity


def test_partial_or_denied_read_cannot_qualify_smoke(setup):
    root, _ = setup
    spec = {'phase': 'smoke', 'command': ['fake'], 'required_text_files': {'header.h': 'text'},
            'permission_files': ['.gitignore'], 'smoke_file': 'candidate.c', 'allowed_files': ['candidate.c']}
    (root / 'candidate.c').write_text('OK')
    write = tool_event(root, 'write', 'candidate.c', 'OK')
    read = tool_event(root, 'read', 'header.h', 'fixed', status='error')
    assert not worker.artifacts.smoke_readiness(spec, root, [read, write], ['candidate.c'])['passed']
    read['part']['state']['status'] = 'completed'
    read['part']['state']['metadata']['display']['text'] = 'unrelated'
    assert not worker.artifacts.smoke_readiness(spec, root, [read, write], ['candidate.c'])['passed']


def ranged_smoke(setup):
    root, _ = setup
    content = 'first\nsecond\nthird\nfourth\n'
    (root / 'header.h').write_text(content)
    (root / 'candidate.c').write_text('OK')
    spec = {'phase': 'smoke', 'command': ['fake'], 'required_text_files': {'header.h': 'text'},
            'permission_files': ['.gitignore'], 'smoke_file': 'candidate.c', 'allowed_files': ['candidate.c']}

    def read(first, last, more):
        event = tool_event(root, 'read', 'header.h', content)
        metadata = event['part']['state']['metadata']
        metadata['truncated'] = more
        metadata['display'].update(lineStart=first, lineEnd=last, truncated=more,
                                   text='\n'.join(content.splitlines()[first - 1:last]))
        return event

    return root, spec, read, tool_event(root, 'write', 'candidate.c', 'OK')


def test_exact_read_ranges_cover_file_despite_continuation_flags(setup):
    root, spec, read, write = ranged_smoke(setup)
    result = worker.artifacts.smoke_readiness(
        spec, root, [read(1, 2, True), read(3, 4, False), write], ['candidate.c'])
    assert result['passed'] and not result['missing_reads']


@pytest.mark.parametrize('defect', ['gap', 'clipped', 'wrong-total', 'wrong-range', 'denied'])
def test_read_range_proof_rejects_missing_or_inexact_content(setup, defect):
    root, spec, read, write = ranged_smoke(setup)
    first, second = read(1, 2, True), read(3, 4, False)
    if defect == 'gap':
        second = read(4, 4, False)
    elif defect == 'clipped':
        first['part']['state']['metadata']['truncated'] = False
        first['part']['state']['metadata']['display']['text'] = 'first\nsecon'
    elif defect == 'wrong-total':
        first['part']['state']['metadata']['display']['totalLines'] += 1
    elif defect == 'wrong-range':
        first['part']['state']['metadata']['display']['lineStart'] = True
    elif defect == 'denied':
        first['part']['state']['status'] = 'error'
    assert not worker.artifacts.smoke_readiness(
        spec, root, [first, second, write], ['candidate.c'])['passed']


@pytest.mark.parametrize('content', ['x' * 1801, 'line\n\n', ''],
                         ids=['overlong-line', 'terminal-blank', 'empty'])
def test_unreadable_scope_text_fails_before_launch(setup, content):
    root, run_dir = setup
    (root / 'header.h').write_text(content)
    with pytest.raises(ValueError, match='Required text'):
        launch(setup, DONE, phase='smoke', required_text_files={'header.h': 'text'},
               permission_files=['.gitignore'], smoke_file='candidate.c')
    assert not run_dir.exists()
    assert (root / 'header.h').read_text() == content


def test_packet_normalizes_terminal_blank_lines_without_changing_source(tmp_path):
    source = tmp_path / 'listing.txt'
    raw = b'first\r\nsecond\r\n\r\n'
    source.write_bytes(raw)
    target = tmp_path / 'packet'
    result = worker.artifacts.prepare_packet(
        [{'source': str(source), 'name': 'listing.txt', 'kind': 'text'}], target)
    copied = (target / 'listing.txt').read_bytes().decode('utf-8')
    assert copied.splitlines() == ['first', 'second']
    assert source.read_bytes() == raw
    assert result['files']['listing.txt']['source_sha256'] == worker.digest(source)
    assert result['files']['listing.txt']['sha256'] == worker.digest(target / 'listing.txt')


@pytest.mark.parametrize('bound', [0, 1801])
def test_packet_line_bound_stays_within_supported_reader_limit(tmp_path, bound):
    source = tmp_path / 'input'; source.write_text('ok')
    with pytest.raises(ValueError):
        worker.artifacts.prepare_packet(
            [{'source': str(source), 'name': 'input', 'kind': 'text'}], tmp_path / 'out',
            max_line_length=bound)
    assert not (tmp_path / 'out').exists()


def test_ignored_smoke_write_is_audited(setup):
    root, run_dir = setup
    (root / 'ignored').mkdir()
    spec = {'directory': str(root), 'owner': 'fake', 'phase': 'smoke',
            'command': [sys.executable, '-c', ''], 'required_text_files': {'header.h': 'text'},
            'permission_files': ['.gitignore'], 'allowed_files': ['ignored/smoke.txt'],
            'smoke_file': 'ignored/smoke.txt'}
    values = [tool_event(root, 'read', 'header.h', 'fixed'), tool_event(root, 'write', 'ignored/smoke.txt', 'OK')]
    spec['command'][-1] = ("from pathlib import Path;Path('ignored/smoke.txt').write_text('OK');"
                          + ';'.join(f'print({json.dumps(json.dumps(e))})' for e in values) + ';' + DONE)
    worker.write_json(root.parent / 'spec.json', spec)
    result = worker.run(root.parent / 'spec.json', run_dir)
    assert result['state'] == 'completed'
    assert result['changed_files'] == ['ignored/smoke.txt']


def test_account_import_is_atomic_idempotent_and_counts_adoption_before_finish(tmp_path, monkeypatch):
    from cmre import env, pilot
    monkeypatch.setattr(env, 'ANALYSIS', tmp_path)
    run_dir = tmp_path / 'run'
    run_dir.mkdir()
    message = {'role': 'assistant', 'sessionID': 's1', 'id': 'm1', 'providerID': 'test',
               'modelID': 'model', 'time': {'completed': 2}, 'finish': 'length',
               'tokens': {'input': 12, 'output': 32000, 'reasoning': 900,
                          'cache': {'read': 8, 'write': 0}}}
    worker.write_json(run_dir / 'spec.json', {'directory': str(tmp_path), 'model': 'test/model'})
    worker.write_json(run_dir / 'receipt.json', {'state': 'incomplete', 'model_requested': 'test/model',
                                               'session_ids': ['s1']})
    worker.write_json(run_dir / 'session-export.json', {'info': {'id': 's1'}, 'messages': [{'info': message}]})
    assert worker.collect(run_dir)['usage_status'] == 'collected'
    adoption = tmp_path / 'adoption.json'
    worker.write_json(adoption, {'schema': 'cmre-provider-adoptions/1',
                                'calls': [{'call_id': 'm1', 'review_corrections': 2}]})
    manifest = tmp_path / 'account.json'
    worker.write_json(manifest, {'schema': 'cmre-provider-accounting/1', 'pilot': 'test',
        'collections': [str(run_dir / 'collections/0001/receipt.json')] * 2, 'adoptions': str(adoption)})
    path = pilot.path('test')
    path.parent.mkdir(parents=True)
    worker.write_json(path, {'ended': None, 'calls': {}, 'usage_complete': False})
    assert worker.account(manifest)['review_corrections'] == 2
    before = path.read_bytes()
    worker.account(manifest)
    assert path.read_bytes() == before
    data = worker.artifacts.load(path)
    assert len(data['calls']) == 1
    assert data['calls']['m1']['review_corrections'] == 2
    assert data['calls']['m1']['reasoning_tokens'] == 900
    assert data['calls']['m1']['output_tokens'] == 32000
    # A conflicting adoption is rejected before writing anything.
    worker.write_json(adoption, {'schema': 'cmre-provider-adoptions/1',
                                'calls': [{'call_id': 'm1', 'review_corrections': 3}]})
    with pytest.raises(ValueError, match='differs'):
        worker.account(manifest)
    assert path.read_bytes() == before
    data['ended'] = 3
    worker.write_json(path, data)
    before = path.read_bytes()
    with pytest.raises(ValueError, match='immutable'):
        worker.account(manifest)
    assert path.read_bytes() == before


@pytest.mark.parametrize('kind,content', [('json', '{broken'), ('text', '\0bad')])
def test_packet_format_error_precedes_dispatch(setup, kind, content):
    root, run_dir = setup
    (root / 'packet').write_text(content)
    with pytest.raises(ValueError):
        launch(setup, DONE, phase='smoke', required_text_files={'packet': kind},
               permission_files=['header.h'], smoke_file='candidate.c')
    assert not run_dir.exists()


def test_export_cannot_mix_model_or_session_identities(terminal_export):
    message = {'role': 'assistant', 'sessionID': 's1', 'id': 'm1', 'providerID': 'wrong',
               'modelID': 'model', 'time': {'completed': 2}, 'finish': 'stop', 'tokens': {}}
    path = terminal_export / 'session-export.json'
    worker.write_json(path, {'info': {'id': 's1'}, 'messages': [{'info': message}]})
    assert worker.collect(terminal_export)['usage_status'] == 'unknown'
    message.update(providerID='test', sessionID='another-session')
    worker.write_json(path, {'info': {'id': 's1'}, 'messages': [{'info': message}]})
    assert worker.collect(terminal_export)['usage_status'] == 'unknown'
    assert worker.artifacts.load(terminal_export / 'collections/0001/receipt.json')['usage_status'] == 'unknown'


def test_conflicting_calls_and_invalid_usage_fail_closed():
    message = {'id': 'm1', 'providerID': 'test', 'modelID': 'model',
               'time': {'completed': 2}, 'finish': 'stop', 'tokens': {'input': 1}}
    other = {**message, 'tokens': {'input': 2}}
    with pytest.raises(ValueError, match='Conflicting'):
        worker.exported_calls([message, other])
    message['tokens']['reasoning'] = -1
    with pytest.raises(ValueError, match='token count'):
        worker.exported_calls([message])


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


@pytest.mark.skipif(os.name != 'nt', reason='Windows delete-pending sharing denial')
def test_inspect_retries_a_receipt_read_during_its_atomic_replace(tmp_path, monkeypatch):
    # Gate 2026-11-13: inspect() opened receipt.json while write_json() replaced it and
    # Windows denied the open (delete pending); the writer retries, the reader did not.
    receipt = tmp_path / 'receipt.json'
    receipt.write_text(json.dumps({'state': 'finished'}), encoding='utf-8')
    real = Path.read_text
    calls = []

    def denied_once(self, *args, **kwargs):
        if self.name == 'receipt.json' and not calls:
            calls.append(self)
            error = PermissionError(13, 'Permission denied')
            error.winerror = 5
            raise error
        return real(self, *args, **kwargs)

    monkeypatch.setattr(Path, 'read_text', denied_once)
    assert worker.read_json(receipt) == {'state': 'finished'}
    assert calls


@pytest.mark.skipif(os.name != 'nt', reason='Windows delete-pending sharing denial')
def test_receipt_read_retries_the_crt_open_denial_without_winerror(tmp_path, monkeypatch):
    # CI 2026-11-16 (PR #45): io.open goes through the CRT, so a delete-pending
    # receipt raises errno 13 with winerror None and read_json() gave up at once.
    receipt = tmp_path / 'receipt.json'
    receipt.write_text(json.dumps({'state': 'running'}), encoding='utf-8')
    real = Path.read_text
    calls = []

    def denied_once(self, *args, **kwargs):
        if self.name == 'receipt.json' and not calls:
            calls.append(self)
            raise PermissionError(13, 'Permission denied')
        return real(self, *args, **kwargs)

    monkeypatch.setattr(Path, 'read_text', denied_once)
    assert worker.read_json(receipt) == {'state': 'running'}
    assert calls
