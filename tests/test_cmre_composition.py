"""Actual composed builds: faithful arithmetic, changed callee, original fallback.

The synthetic helper call is test scaffolding, not a recovered source claim.
Both original entries are LEA/RET getters in the pinned executable.
"""
from types import SimpleNamespace
import json

import pytest

from cmre import campaign, composition, env, lift

A, B = 0x004671B0, 0x004671C0
CALLER = '''#include "cmre.h"
extern CM_THISCALL void *cm_fn_004671c0(void *self);
CM_THISCALL void *cm_fn_004671b0(void *self) {
    return (void *)((u32)cm_fn_004671c0(self) - 0x50u);
}
'''
CALLEE = '''#include "cmre.h"
__attribute__((noinline)) CM_THISCALL void *cm_fn_004671c0(void *self) {
    return (void *)((u32)self + 0x70u);
}
'''


@pytest.fixture
def sources(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "C_DIR", tmp_path)
    (tmp_path / "fn_004671b0.c").write_text(CALLER)
    (tmp_path / "fn_004671c0.c").write_text(CALLEE)
    return tmp_path


def test_composed_c_executes_recovered_callee_at_both_optimizations(sources):
    result = composition.test(campaign.create("test", [A, B]), {"cases": 12})
    assert result["status"] == "pass", result
    assert result["observed_composed_calls_at_both_optimizations"] is True
    assert result["milestone_accepted"] is False
    for value in result["builds"].values():
        assert value["entries"]["fn_004671b0"]["observed_other_selected_entries"] == ["fn_004671c0"]
        assert all(int(a, 16) >= env.LINK_BASE for a in value["compiled_symbols"].values())


def test_composed_c_uses_c_tier_input_shapes(sources):
    (env.HINTS_DIR / f'fn_{A:08x}.json').write_text(json.dumps({
        'ecx': 'small', 'c_hints': {'ecx': 'ptr'},
    }))
    result = composition.test(campaign.create('tier-inputs', [A, B]), {'cases': 12})
    assert result['status'] == 'pass', result
    assert result['options']['fn_004671b0']['hints'] == {'ecx': 'ptr'}


def test_changed_selected_callee_fails_composed_corpus(sources):
    path = sources / "fn_004671c0.c"
    path.write_text(CALLEE.replace("0x70u", "0x74u"))
    result = composition.test(campaign.create("test", [A, B]), {"cases": 8})
    assert result["status"] == "fail"
    assert all(b["entries"]["fn_004671b0"]["mismatches"] for b in result["builds"].values())


def test_hardcoded_original_selected_callee_is_escape_failure(sources):
    path = sources / "fn_004671b0.c"
    path.write_text(CALLER.replace("cm_fn_004671c0(self)",
                                 "((void * (__attribute__((thiscall)) *)(void *))0x004671c0u)(self)"))
    result = composition.test(campaign.create("test", [A, B]), {"cases": 8})
    assert result["status"] == "fail"
    for value in result["builds"].values():
        mismatch = value["entries"]["fn_004671b0"]["mismatches"][0]
        assert mismatch["field"] == "outcome"
        assert mismatch["recovered"] == "escape"


def test_selected_symbol_outside_recovered_image_is_rejected():
    built = SimpleNamespace(symbols={"_cm_fn_004671b0": A},
                            image=SimpleNamespace(text=SimpleNamespace(va=env.LINK_BASE, end=env.LINK_BASE + 4096)))
    with pytest.raises(ValueError, match="outside"):
        composition._symbols(built, [A])


@pytest.mark.parametrize("events,expected", [
    # Actual table accesses in the retained copy normal-0 original trace.
    ([("r", 0x94F2E4, 4, 0), ("r", 0x94F364, 4, 0),
      ("r", 0x94F3DC, 4, 0)], 0),
    ([("r", 0x94F248, 1, 0)], 1),
    # Starts in table data, but includes the instruction at F248.
    ([("r", 0x94F247, 4, 0)], 1),
    # Starts outside the function, but includes its first instruction.
    ([("r", 0x94F0EF, 2, 0)], 1),
    ([("w", 0x94F248, 1, 0)], 1),
    ([("c", "scaffolding", 0, ())], 0),
])
def test_composition_classifies_entire_access_against_instruction_bytes(monkeypatch, events, expected):
    """Exercise _run's classifier without executing the synthetic trace as code."""
    va = 0x94F0F0
    body = lift.discover(va)
    result = SimpleNamespace(events=events, covered=set(body.insns), outcome="return")
    machine = SimpleNamespace(uc=SimpleNamespace(hook_add=lambda *a, **k: None),
                              _escape=None, run=lambda *a: result)
    monkeypatch.setattr(composition.emu, "Machine", lambda *a, **k: machine)
    monkeypatch.setattr(composition.difftest, "arena_templates", lambda: ())
    monkeypatch.setattr(composition.difftest, "make_case", lambda *a: None)
    monkeypatch.setattr(composition.difftest, "_normalize", lambda *a: {})
    monkeypatch.setattr(composition.difftest, "_digest_view", lambda *a: "trace")
    monkeypatch.setattr(composition.difftest, "_diff", lambda *a, fault_mapping=None, strict_data_fault=False: None)
    built = SimpleNamespace(symbols={}, image=SimpleNamespace(
        text=SimpleNamespace(va=env.LINK_BASE, end=env.LINK_BASE + 4096)))
    opts = SimpleNamespace(seed=1, cases=1, budget=10, discovery=0, min_coverage=1, inputs='v1')
    report = composition._run(va, {va: body}, built, {va: env.LINK_BASE}, opts)
    assert report["code_as_data_cases"] == expected
    assert report["status"] == ("fail" if expected else "pass")
