"""Versioned C input policy: v2 removes inputs no original call can receive.

v1 (lifted records and C accepted before v2) is unchanged. v2, for new C:
- entry EFLAGS never set DF, which the Win32 ABI clears at every call;
- writable globals are randomized only when the function's own instructions
  read them. Callee-internal state (MSVC 6 keeps string literals and C runtime
  state in writable .data) stays at its image bytes: randomizing a sprintf
  format string made the formatter over-read stack bytes, whose layout the
  semantic profile deliberately does not compare (the AE2000 WIP class).
"""
import json
import random
import struct
from copy import deepcopy
from dataclasses import replace

import pytest

from cmre import difftest, emu, env, image, recover

STOSD3 = 0x0094F827  # push edi; mov edi, [esp+8]; xor eax, eax; stosd x3; pop edi; ret
STOSD3_C = """#include "cmre.h"
/* 0x0094F827: clears three dwords at the cdecl pointer argument (stosd x3, DF clear). */
void CM_CDECL cm_fn_0094f827(u32 *p)
{
    volatile u32 *q = p;
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
}
"""


def _cases(inputs, n=64, seed=0x1234):
    opts = difftest.Options.for_function(STOSD3)
    opts.inputs = inputs
    rng = random.Random(seed)
    return [difftest.make_case(rng, opts, [0x00AE24D0], difftest.arena_templates()) for _ in range(n)]


def test_v1_cases_keep_the_direction_flag_seed():
    assert any(c.regs["eflags"] & 0x400 for c in _cases("v1"))


def test_v2_cases_never_enter_with_the_direction_flag_set():
    assert not any(c.regs["eflags"] & 0x400 for c in _cases("v2"))


def test_v2_draws_the_same_random_stream_apart_from_df():
    for a, b in zip(_cases("v1"), _cases("v2")):
        assert a.regs["eflags"] & ~0x400 == b.regs["eflags"]
        assert {k: v for k, v in a.regs.items() if k != "eflags"} == {k: v for k, v in b.regs.items() if k != "eflags"}
        assert (a.stack, a.arena, a.globals, a.import_seed) == (b.stack, b.arena, b.globals, b.import_seed)


def _direction_fixture(flags):
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = flags
    return {
        'regs': regs, 'stack': [emu.ARENA + 8] + [0] * 9,
        'arena_template': 0,
        'arena_writes': {hex(i * 4): (i + 1) * 0x11111111 for i in range(5)},
        'globals': {}, 'import_seed': 0xDF01, 'fpcw': 0x027F,
    }


@pytest.mark.parametrize('flags', [0x202, 0x602, 0x683])
def test_fixture_v2_clears_only_df_without_mutating_replay_or_rng(flags):
    fixture = _direction_fixture(flags)
    before = deepcopy(fixture)
    opts = difftest.Options.for_function(STOSD3)
    opts.hints = {'fixtures': [fixture]}
    old_rng, new_rng = random.Random(2026), random.Random(2026)
    templates = difftest.arena_templates()
    selected = 0
    for _ in range(64):
        old = difftest.make_case(old_rng, replace(opts, inputs='v1'), [], templates)
        new = difftest.make_case(new_rng, replace(opts, inputs='v2'), [], templates)
        assert new == replace(old, regs={**old.regs, 'eflags': old.regs['eflags'] & ~0x400})
        if old.import_seed == fixture['import_seed']:
            selected += 1
            assert old.regs == fixture['regs']  # v1 keeps even DF-set saved flags
    assert 0 < selected < 64  # both saved fixtures and ordinary cases were exercised
    assert old_rng.getstate() == new_rng.getstate()
    assert fixture == before
    assert difftest._fixture_case(fixture, templates, 10).regs == fixture['regs']


@pytest.mark.parametrize('inputs, expected', [
    ('v1', (0, 0, 0, 0x44444444, 0x55555555)),
    ('v2', (0x11111111, 0x22222222, 0, 0, 0)),
])
def test_saved_fixture_string_store_direction_follows_input_policy(inputs, expected):
    fixture = _direction_fixture(0x602)
    opts = difftest.Options.for_function(STOSD3)
    opts.inputs, opts.hints = inputs, {'fixtures': [fixture]}
    rng = random.Random(2026)
    cases = [difftest.make_case(rng, opts, [], difftest.arena_templates()) for _ in range(40)]
    selected = next(case for case in cases if case.import_seed == fixture['import_seed'])
    machine = emu.Machine(image.primary(), watch=(STOSD3, STOSD3 + 15))
    result = machine.run(STOSD3, selected, 100)
    assert result.outcome == 'return'
    assert struct.unpack('<5I', machine.uc.mem_read(emu.ARENA, 20)) == expected


def test_unknown_input_policy_is_rejected():
    opts = difftest.Options.for_function(STOSD3)
    opts.inputs = "v3"
    with pytest.raises(ValueError):
        difftest.make_case(random.Random(0), opts, [], difftest.arena_templates())


def test_own_global_reads_exclude_reads_made_by_callees():
    img = image.primary()
    region = (0x00426130, 0x00426200)
    own, callee = 0x00AE24D0, 0x009870E8  # both writable .data
    result = emu.Result("return", (), (), [("r", own, 4, 0), ("r", callee, 1, 0x76), ("c", "x", 0, ())],
                        {}, set(), read_sites={0: 0x00426140, 1: 0x00944CB0})
    assert difftest._global_reads(result, img) == {own, callee & ~3}
    assert difftest._global_reads(result, img, region) == {own}


def test_machine_attributes_reads_to_the_reading_instruction():
    # 0x008B4F50: movsx eax, byte [esp+4]; mov al, [eax+ecx+4]; ret 4
    va = 0x008B4F50
    machine = emu.Machine(image.primary(), watch=(va, va + 12))
    machine.attribute_reads = True
    case = emu.Case({"ecx": emu.ARENA}, [3] + [0] * 9, bytes(emu.ARENA_SIZE))
    result = machine.run(va, case, 1000)
    sites = {result.events[i][1]: site for i, site in result.read_sites.items()}
    assert sites[emu.STACK_TOP + 4] == va
    assert sites[emu.ARENA + 7] == va + 5
    machine.attribute_reads = False
    assert machine.run(va, case, 1000).read_sites == {}


@pytest.fixture
def c_dir(tmp_path, monkeypatch):
    monkeypatch.setattr(env, "C_DIR", tmp_path / "c")
    monkeypatch.setattr(env, "LEDGER_DIR", tmp_path / "ledger")
    (tmp_path / "c").mkdir()
    (tmp_path / "ledger").mkdir()
    (tmp_path / "c" / f"fn_{STOSD3:08x}.c").write_text(STOSD3_C)
    return tmp_path


def test_string_store_cannot_pass_under_v1_direction_flag_cases(c_dir, monkeypatch):
    monkeypatch.setattr(recover, "C_INPUTS", "v1")
    assert recover.test_c(STOSD3, {"cases": 100}, fresh=True)["status"] == "fail"


def test_string_store_passes_under_v2_and_records_its_policy(c_dir):
    r = recover.test_c(STOSD3, {"cases": 100}, fresh=True)
    assert r["status"] == "pass", r
    assert {b["difftest"]["inputs"] for b in r["builds"].values()} == {"v2"}


def _record(c_dir, validation):
    path = c_dir / "ledger" / f"fn_{STOSD3:08x}.json"
    path.write_text(json.dumps({"c": {"validation": validation}}))


def test_new_c_uses_v2(c_dir):
    assert recover.c_inputs(STOSD3) == "v2"


def test_c_accepted_before_v2_keeps_v1(c_dir):
    _record(c_dir, {"-O0": {"profile": "semantic"}, "-O2": {"profile": "semantic"}})
    assert recover.c_inputs(STOSD3) == "v1"


def test_c_accepted_under_v2_keeps_v2(c_dir):
    _record(c_dir, {"-O0": {"profile": "semantic", "inputs": "v2"}, "-O2": {"profile": "semantic", "inputs": "v2"}})
    assert recover.c_inputs(STOSD3) == "v2"
