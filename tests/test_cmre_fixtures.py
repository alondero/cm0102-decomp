"""Captured input fixtures must replay exactly without changing default cases."""
from copy import copy, deepcopy
from dataclasses import replace
import hashlib
import json
import random

import pytest

from cmre import difftest, emu, image


def input_fixture():
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = 0x202
    return {
        'regs': regs,
        'stack': [emu.ARENA] + [0] * 9,
        'arena_template': 0,
        'arena_writes': {'0x4': emu.ARENA + 0x1000, '0x1030': 0x5A000000, '0x1034': 0},
        'globals': {'0x00b67a34': 0x12345678},
        'import_seed': 0x5EED,
    }


def options(hints=None):
    opts = difftest.Options.for_function(0x00450A70)
    opts.hints = hints or {}
    return opts


def test_default_case_and_rng_stream_are_unchanged():
    rng = random.Random(123456)
    case = difftest.make_case(rng, options(), [0x00B67A34], difftest.arena_templates())
    summary = {
        'regs': case.regs, 'stack': case.stack, 'globals': case.globals,
        'import_seed': case.import_seed, 'arena_sha256': hashlib.sha256(case.arena).hexdigest(),
    }
    assert hashlib.sha256(json.dumps(summary, sort_keys=True).encode()).hexdigest() == (
        '8946d6850a447128e5227b42611aa1dc2323103e2fdde679dce0933197963fae'
    )
    assert hashlib.sha256(repr(rng.getstate()).encode()).hexdigest() == (
        '4e3ba77b0975e06f0b1ab11fa93c826df7c5128fc3b7eff5a572e82f3bc20ace'
    )


def test_fixture_mix_is_reproducible_and_keeps_random_cases():
    data = input_fixture()
    before = deepcopy(data)
    opts = options({'fixtures': [data]})
    def generate():
        rng = random.Random(2026)
        return [difftest.make_case(rng, opts, [], difftest.arena_templates()) for _ in range(40)]
    cases = generate()
    assert cases == generate()
    selected = [case for case in cases if case.import_seed == data['import_seed']]
    assert 0 < len(selected) < len(cases)
    assert all(case.regs == data['regs'] and case.stack == data['stack'] for case in selected)
    selected[0].regs['eax'] = 0xDEADBEEF
    selected[0].stack[0] = 0
    selected[0].globals.clear()
    assert data == before


@pytest.mark.parametrize('fpcw', [0, 0x007F, 0xFFFF])
def test_fixture_retains_explicit_sixteen_bit_fpcw(fpcw):
    data = {**input_fixture(), 'fpcw': fpcw}
    case = difftest._fixture_case(data, difftest.arena_templates(), 10)
    assert case.fpcw == fpcw


def test_fixture_replays_nested_pointer_reads_in_original_instructions():
    data = input_fixture()
    rng = random.Random(2026)
    opts = options({'fixtures': [data]})
    cases = [difftest.make_case(rng, opts, [], difftest.arena_templates()) for _ in range(40)]
    selected = [case for case in cases if case.import_seed == data['import_seed']]
    assert selected, 'input fixture was not selected'
    case = selected[0]
    # Synthetic bytes only in an in-memory image copy: load args[0], follow
    # its +4 pointer, load a dword at +0x33, store it back at args[0]+8.
    va = 0x006CD5F0  # preferred-image VA; original image pinned by cmre.env
    code = bytes.fromhex('8b4424048b48048b5133895008c3')
    img = copy(image.primary())
    section = img.section_at(va)
    offset = va - section.va
    patched = replace(section, raw=section.raw[:offset] + code + section.raw[offset + len(code):])
    img.sections = [patched if s is section else s for s in img.sections]
    machine = emu.Machine(img, watch=(va, va + len(code)))
    result = machine.run(va, case, 100)
    assert result.outcome == 'return'
    assert result.regs[2] == 0x5A
    assert bytes(machine.uc.mem_read(emu.ARENA + 8, 4)) == b'\x5a\x00\x00\x00'
    assert machine.uc.mem_read(0x00B67A34, 4) == b'\x78\x56\x34\x12'


@pytest.mark.parametrize('change', [
    {'arena_template': -1},
    {'arena_template': difftest.ARENA_TEMPLATES},
    {'arena_writes': {'0x3ffff': 1}},
    {'arena_writes': {'0x3': 1}},
    {'globals': {'0x00400000': 0}},
    {'stack': [0]},
    {'import_seed': 0x100000000},
    {'fpcw': -1},
    {'fpcw': 0x10000},
    {'fpcw': True},
    {'fpcw': None},
    {'fpcw': 0.5},
    {'fpcw': '127'},
])
def test_invalid_fixture_cannot_silently_be_used(change):
    data = {**input_fixture(), **change}
    opts = options({'fixtures': [data]})
    rng = random.Random(2026)
    with pytest.raises(ValueError, match='fixture'):
        for _ in range(40):
            difftest.make_case(rng, opts, [], difftest.arena_templates())
