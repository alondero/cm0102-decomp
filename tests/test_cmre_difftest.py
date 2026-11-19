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


def _slow_faithful(tmp_path, delay):
    """The faithful lift with `delay` NOPs at entry: same behaviour, more instructions."""
    lifted = lift.discover(LEAP)
    text = lift.render(lifted)
    entry = f"{lifted.symbol}:\n"
    assert entry in text
    src = tmp_path / "slow.S"
    src.write_text(text.replace(entry, entry + "    nop\n" * delay, 1), encoding="utf-8")
    return lifted, build.build(src, lifted.symbol, tmp_path)


@pytest.mark.parametrize("profile", ["abi", "semantic"])
def test_slower_faithful_candidate_gets_a_bounded_extension(tmp_path, profile):
    # ADR 0012: C that needs more steps than the shared budget, on a case the original
    # finished, is re-run once with RECOVERED_BUDGET_FACTOR times the budget and compared.
    lifted, built = _slow_faithful(tmp_path, 300)
    opts = difftest.Options.for_function(LEAP, {"cases": 4, "budget": 64})
    assert 300 < 64 * difftest.RECOVERED_BUDGET_FACTOR
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile)
    assert result["mismatches"] == [] and result["inconclusive"] == 0
    assert result["recovered_budget_extended"] == 4
    assert result["recovered_budget_factor"] == difftest.RECOVERED_BUDGET_FACTOR


@pytest.mark.parametrize("profile", ["abi", "semantic"])
def test_extension_is_bounded_and_still_catches_a_slow_candidate(tmp_path, profile):
    lifted, built = _slow_faithful(tmp_path, 64 * difftest.RECOVERED_BUDGET_FACTOR + 50)
    opts = difftest.Options.for_function(LEAP, {"cases": 2, "budget": 64})
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile)
    assert result["mismatches"] and result["mismatches"][0]["kind"] == "budget-asymmetry"
    assert result["mismatches"][0]["recovered_budget"] == 64 * difftest.RECOVERED_BUDGET_FACTOR


def test_explain_uses_the_same_extension(tmp_path):
    lifted, built = _slow_faithful(tmp_path, 300)
    opts = difftest.Options.for_function(LEAP, {"cases": 4, "budget": 64})
    out = difftest.explain(LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts,
                           "semantic", case_index=1)
    assert out["recovered"]["outcome"] == "return"
    assert out["recovered"]["budget_extended"] is True


@pytest.mark.parametrize("profile", ["abi", "semantic"])
def test_a_failed_extension_is_not_repeated_on_later_cases(tmp_path, profile):
    # Review of ADR 0012: re-running every case of a never-finishing candidate at 20x made it
    # ~10x slower to fail. One extended run that still exhausts settles it for the function.
    lifted = lift.discover(LEAP)
    src = tmp_path / "loop.S"
    src.write_text(f".intel_syntax noprefix\n.text\n.globl {lifted.symbol}\n"
                   f"{lifted.symbol}:\n jmp {lifted.symbol}\n", encoding="utf-8")
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(LEAP, {"cases": 6, "budget": 128})
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile)
    assert result["mismatches"][0]["kind"] == "budget-asymmetry"
    assert result["recovered_budget_extended"] == 1 and result["inconclusive"] == 6


@pytest.mark.parametrize("profile", ["abi", "semantic"])
def test_mutual_exhaustion_is_not_extended(tmp_path, profile):
    lifted, built = _slow_faithful(tmp_path, 300)
    opts = difftest.Options.for_function(LEAP, {"cases": 4, "budget": 1})
    result = difftest.run(LEAP, (lifted.va, lifted.region_end), set(lifted.insns), built, {}, opts, profile)
    assert result["inconclusive"] == 4 and "recovered_budget_extended" not in result


def test_extended_runs_compare_memory_and_imports_like_fresh_runs(tmp_path):
    # A caller with imports and memory effects: a re-run on the same machine must start clean.
    lifted = lift.discover(CALLER)
    text = lift.render(lifted)
    entry = f"{lifted.symbol}:\n"
    src = tmp_path / "slow_caller.S"
    src.write_text(text.replace(entry, entry + "    nop\n" * 2000, 1), encoding="utf-8")
    built = build.build(src, lifted.symbol, tmp_path)
    opts = difftest.Options.for_function(CALLER, {"cases": 60, "budget": 1500})
    result = difftest.run(CALLER, (lifted.va, lifted.region_end), set(lifted.insns), built,
                          recover._call_returns(lifted, built), opts, "semantic")
    assert result["mismatches"] == []
    assert result.get("recovered_budget_extended", 0) > 0 and result["import_calls"] > 0
