"""`capture apply` writes hints under the writer lease, so a session check is not made inconclusive."""
import json
import threading
import time

from cmre import capture_expand, coordination, difftest, env


def test_apply_waits_for_a_held_lease_before_writing_hints(monkeypatch, tmp_path):
    lease = tmp_path / 'command.lock'
    monkeypatch.setenv('CMRE_COORDINATION_PATH', str(lease))
    monkeypatch.delenv('CMRE_WRITER_TOKEN', raising=False)
    monkeypatch.delenv('CMRE_WRITER_PATH', raising=False)
    monkeypatch.setattr(env, 'HINTS_DIR', tmp_path)
    monkeypatch.setattr(capture_expand, 'select', lambda va: ['a' * 64])
    monkeypatch.setattr(difftest, 'validate_hints', lambda hints: None)
    monkeypatch.setattr(capture_expand, 'LEASE_POLL_SECONDS', 0.02)
    hint = tmp_path / 'fn_00401000.json'
    hint.write_text(json.dumps({'auto': True}), encoding='utf-8')

    held, release = threading.Event(), threading.Event()
    seen_while_held = []

    def check():
        with coordination.lock(lease):
            held.set()
            release.wait(5)
            seen_while_held.append(json.loads(hint.read_text(encoding='utf-8')))

    holder = threading.Thread(target=check)
    holder.start()
    held.wait(5)
    threading.Timer(0.3, release.set).start()
    started = time.monotonic()
    out = capture_expand.apply(0x401000, test=False)
    holder.join(5)

    assert seen_while_held == [{'auto': True}]  # nothing written while the check held the lease
    assert time.monotonic() - started >= 0.25
    assert out['captures'] == 1
    assert json.loads(hint.read_text(encoding='utf-8')) == {'captures': ['a' * 64]}
