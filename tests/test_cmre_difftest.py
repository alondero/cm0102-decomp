"""The differential engine must accept faithful code and reject altered code."""
import pytest

from cmre import build, difftest, lift, recover

LEAP = 0x0054CFE0  # leap test on unsigned year word: pure arithmetic, every path returns
CALLER = 0x00414C20  # calls other functions and imports: exercises return-address mapping


def _run(va, text, tmp_path, cases=120):
    lifted = lift.discover(va)
    src = tmp_path / f"fn_{va:08x}.S"
    src.write_text(text, encoding="utf-8")
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(va, {"cases": cases})
    return difftest.run(va, (lifted.va, lifted.region_end), set(lifted.insns), built,
                        recover._call_returns(lifted, built), opts, "exact")


def test_faithful_lift_passes(tmp_path):
    result = _run(LEAP, lift.render(lift.discover(LEAP)), tmp_path)
    assert result["mismatches"] == []
    assert result["outcomes"] == {"return": 120}


@pytest.mark.parametrize("profile", ["abi", "semantic"])
def test_candidate_timeout_cannot_hide_a_terminating_original(tmp_path, profile):
    lifted = lift.discover(LEAP)
    src = tmp_path / "loop.S"
    # Retain the entry symbol but never return. This exercises the actual runner,
    # not only a report predicate. The original leap-year routine always returns.
    src.write_text(f".intel_syntax noprefix\n.text\n.globl {lifted.symbol}\n"
                   f"{lifted.symbol}:\n jmp {lifted.symbol}\n", encoding="utf-8")
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(LEAP, {"cases": 4, "budget": 128})
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns),
                          built, {}, opts, profile)
    assert result["outcomes"] == {"return": 4}
    assert result["mismatches"], "a candidate-only timeout must prevent acceptance"
    assert result["mismatches"][0]["kind"] == "budget-asymmetry"


@pytest.mark.parametrize("profile", ["abi", "semantic"])
@pytest.mark.parametrize("budget", [1, 128])
def test_faithful_code_preserves_mutual_budget_and_return_outcomes(tmp_path, profile, budget):
    lifted = lift.discover(LEAP)
    src = tmp_path / "faithful.S"
    src.write_text(lift.render(lifted), encoding="utf-8")
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(LEAP, {"cases": 4, "budget": budget})
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns),
                          built, {}, opts, profile)
    assert not result["mismatches"]
    assert result["inconclusive"] == (4 if budget == 1 else 0)


@pytest.mark.parametrize("original,altered", [
    ("    and edx, 0x80000003", "    and edx, 0x80000007"),  # constant
    ("    mov esi, 0x64", "    mov esi, 0x65"),  # divisor
    ("    jns v_0054cff8", "    js v_0054cff8"),  # branch sense
])
def test_altered_instruction_is_detected(tmp_path, original, altered):
    text = lift.render(lift.discover(LEAP))
    assert original in text
    result = _run(LEAP, text.replace(original, altered, 1), tmp_path, cases=400)
    assert result["mismatches"], f"{altered!r} was not detected"


def test_dead_stack_difference_is_detected(tmp_path):
    # push/pop esi leaves ESI's value below ESP; a different scratch value must be seen.
    text = lift.render(lift.discover(LEAP)).replace("    push esi", "    push edi", 1).replace("    pop esi", "    pop esi", 1)
    result = _run(LEAP, text, tmp_path, cases=200)
    assert result["mismatches"]


def test_calls_do_not_cause_false_mismatches(tmp_path):
    result = _run(CALLER, lift.render(lift.discover(CALLER)), tmp_path, cases=150)
    assert result["mismatches"] == []
    assert result["import_calls"] > 0  # stubs ran and their arguments were compared


def test_results_are_reproducible(tmp_path):
    text = lift.render(lift.discover(CALLER))
    a = _run(CALLER, text, tmp_path / "a" if (tmp_path / "a").mkdir() is None else None, cases=80)
    b = _run(CALLER, text, tmp_path / "b" if (tmp_path / "b").mkdir() is None else None, cases=80)
    assert a["result_digest"] == b["result_digest"]


def _res(events):
    from cmre import emu

    return emu.Result("return", (), (0,) * 9, events, {}, set())


REGION = (0x00938CC0, 0x00938D40)


def test_difference_after_reading_own_code_is_code_as_data():
    # A callee read instruction bytes of the function under test (seen in 0x00938CC0).
    a = _res([("r", 0x00938D29, 1, 0x83), ("r", 0x00938DB4, 1, 0x90)])
    b = _res([("r", 0x00938D29, 1, 0x83), ("r", 0x00938DB4, 1, 0x00)])
    verdict, _ = difftest._classify(difftest._normalize(a, None, "exact"), difftest._normalize(b, None, "exact"),
                                    a, None, REGION, [])
    assert verdict == "code-as-data"


def test_reading_copied_data_in_region_is_still_a_mismatch():
    a = _res([("r", 0x00938D30, 1, 1)])
    b = _res([("r", 0x00938D30, 1, 2)])
    verdict, _ = difftest._classify(difftest._normalize(a, None, "exact"), difftest._normalize(b, None, "exact"),
                                    a, None, REGION, [(0x00938D30, 0x00938D40)])
    assert verdict == "mismatch"


def test_difference_without_own_code_reads_is_a_mismatch():
    a = _res([("r", 0x03000000, 4, 1)])
    b = _res([("r", 0x03000000, 4, 2)])
    verdict, _ = difftest._classify(difftest._normalize(a, None, "exact"), difftest._normalize(b, None, "exact"),
                                    a, None, REGION, [])
    assert verdict == "mismatch"


def test_arena_templates_are_process_constants():
    first = difftest.arena_templates()
    assert difftest.arena_templates() is first  # generated once per process, from a fixed seed
    assert len(first) == difftest.ARENA_TEMPLATES
