"""Session runner: compact console output and measured progress between receipts."""
import importlib.util

from cmre import env

spec = importlib.util.spec_from_file_location("cmre_session", env.ROOT / "scripts/cmre_session.py")
runner = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runner)


def test_pytest_progress_lines_are_condensed():
    lines = ["............................................ [ 10%]\n",
             "....F....................................... [ 20%]\n",
             "FAILED tests/test_x.py::test_y - assert 1 == 2\n",
             "1 failed, 682 passed in 153.98s (0:02:33)\n"]
    shown = runner.condense(("pytest",), lines)
    assert shown == ["FAILED tests/test_x.py::test_y - assert 1 == 2\n", "1 failed, 682 passed in 153.98s (0:02:33)\n"]


def test_other_commands_are_shown_in_full():
    lines = ["ok  ledgers: verified\n", "ok  inventory\n"]
    assert runner.condense(("cmre", "doctor"), lines) == lines


def test_progress_reports_accepted_source_change():
    before = {"source_functions": 157, "source_body_bytes": 182913, "c_tier_functions": 106, "lifted_functions": 7929}
    after = {"source_functions": 170, "source_body_bytes": 190000, "c_tier_functions": 119, "lifted_functions": 7929}
    assert runner.progress(before, after) == {"source_functions": 13, "source_body_bytes": 7087,
                                              "c_tier_functions": 13, "lifted_functions": 0}
