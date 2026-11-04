"""Authored x86 tests: visible x87 control and execution precision must agree."""
import struct
from types import SimpleNamespace

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import emu, image


ENTRY = 0x401000


def _memory(opcode, offset):
    return bytes.fromhex(opcode) + struct.pack('<I', emu.ARENA + offset)


def _machine():
    paths = {}
    code = bytearray()
    for mode in ('unchanged', 'reload', 'helper-cycle'):
        paths[mode] = ENTRY + len(code)
        code += _memory('d93d', 0x40)  # fnstcw: observe, do not choose an incoming CW
        if mode == 'helper-cycle':
            code += _memory('d92d', 0x42)  # fldcw explicit temporary chop word
        if mode != 'unchanged':
            code += _memory('d92d', 0x40)  # fldcw exact observed incoming word
        code += _memory('db05', 4)  # fild factor
        code += _memory('db05', 0)  # fild integer product
        code += bytes.fromhex('d8c9')  # fmul st(0),st(1)
        code += _memory('dc0d', 8)  # fmul original-shape double constant
        code += bytes.fromhex('d9c0') + _memory('db3d', 0x50)  # duplicate, fstp m80
        code += _memory('dd0d', 0x60)  # fisttp qword: source-cast-shaped truncation
        code += _memory('d93d', 0x44)
        code += _memory('a1', 0x60) + b'\xc3'
    img = SimpleNamespace(base=0x400000, headers=bytes(0x1000), imports={},
        sections=[image.Section('.text', ENTRY, 0x1000, bytes(code), 0x60000000)])
    return emu.Machine(img), paths


def _run(machine, entry, fpcw=None):
    arena = bytearray(emu.ARENA_SIZE)
    struct.pack_into('<iid', arena, 0, -964952064, -122, 0.001)
    struct.pack_into('<H', arena, 0x42, 0x0c00)
    result = machine.run(entry, emu.Case({'eflags': 0x202}, [], bytes(arena), fpcw=fpcw), 100)
    assert result.outcome == 'return'
    raw = bytes(machine.uc.mem_read(emu.ARENA, 0x70))
    return result.regs[0], raw[0x50:0x5a], struct.unpack_from('<H', raw, 0x40)[0], struct.unpack_from('<H', raw, 0x44)[0]


@pytest.mark.parametrize('mode', ['reload', 'helper-cycle'])
def test_omitted_control_word_matches_reloading_its_observed_value(mode):
    machine, entries = _machine()
    visible = machine.uc.reg_read(R.UC_X86_REG_FPCW)
    # Keep the backend's visible initial value; do not introduce a new ABI default.
    backend = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    assert visible == backend.reg_read(R.UC_X86_REG_FPCW)
    plain = _run(machine, entries['unchanged'])
    reloaded = _run(machine, entries[mode])
    assert plain[2:] == reloaded[2:] == (visible, visible)
    assert plain == reloaded


@pytest.mark.parametrize('control, expected', [
    (0x0000, 117724152),  # PC24, nearest
    (0x0400, 117724144),  # PC24, down
    (0x0800, 117724152),  # PC24, up
    (0x0c00, 117724144),  # PC24, chop
    (0x0200, 117724151),  # PC53, nearest
    (0x0300, 117724151),  # PC64, nearest
])
def test_explicit_control_words_keep_precision_and_rounding(control, expected):
    machine, entries = _machine()
    plain = _run(machine, entries['unchanged'], control)
    reloaded = _run(machine, entries['reload'], control)
    assert plain == reloaded
    assert plain[0] == expected
    assert plain[2:] == (control, control)


def test_pristine_reset_restores_coherent_control_after_explicit_cases():
    machine, entries = _machine()
    reference = _run(machine, entries['reload'])
    for control in (0x0300, 0x0c00, 0x0200):
        _run(machine, entries['unchanged'], control)
        assert _run(machine, entries['unchanged']) == reference
