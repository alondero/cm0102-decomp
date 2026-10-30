"""Explicit batch selection must retain the ordinary recovery frontier rules."""
from types import SimpleNamespace

from cmre import cli, env, inventory, ledger, queue


def test_selected_batch_filters_before_limit_and_skips_done_and_blocked(monkeypatch, capsys, tmp_path):
    monkeypatch.setattr(env, "ANALYSIS", tmp_path)
    monkeypatch.setenv("CMRE_COORDINATION_PATH", str(tmp_path / "command.lock"))
    functions = {va: SimpleNamespace(name=f"fn_{va:08x}", body_bytes=size)
                 for va, size in [(0x401000, 1), (0x401010, 10), (0x401020, 20),
                                  (0x401030, 30), (0x401040, 40)]}
    inv = SimpleNamespace(functions=functions, callees={}, modules={})
    monkeypatch.setattr(inventory, "load", lambda: inv)
    monkeypatch.setattr(ledger, "recovered", lambda: {0x401000: None})
    monkeypatch.setattr(ledger, "blocked", lambda: {0x401020: {}})
    monkeypatch.setattr(queue.image, "primary", lambda: SimpleNamespace(text=SimpleNamespace(va=0x401000, end=0x402000)))
    scheduled = []

    class Pool:
        def __init__(self, max_workers):
            pass

        def __enter__(self):
            return self

        def __exit__(self, *args):
            pass

        def submit(self, fn, va):
            scheduled.append(va)
            return SimpleNamespace(result=lambda: (va, True, "test scaffolding"))

    monkeypatch.setattr(cli, "ProcessPoolExecutor", Pool)
    monkeypatch.setattr(cli, "as_completed", iter)
    assert [i.va for i in queue.frontier(limit=1)] == [0x401010]
    assert cli.main(["batch", "-n", "1", "--functions", "fn_00401000", "fn_00401020",
                     "fn_00401030", "fn_00401040"]) == 0
    assert scheduled == [0x401030]
    assert "batch: 1/1 accepted" in capsys.readouterr().out
