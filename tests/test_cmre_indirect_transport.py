"""Native indirect CALL ordering and owned near-RET continuation controls.

Synthetic callees test the VM seam only; no application behavior is inferred.
Original CALL bytes and ownership are verified against the pinned executable.
"""
import struct

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, image, lift, return_transport

CALL, RETURN = 0x7e6189, 0x7e618b
JUMP, CONT, TARGET, TABLE = 0x10000000, 0x10000005, 0x10000100, 0x10000400
DPUSH, DCONT, DTARGET = 0x10000200, 0x1000020a, 0x10000300
END, STACK = 0x10000500, 0x20000800


def jump(at, target):
    return b'\xe9' + struct.pack('<I', (target - at - 5) & 0xffffffff)


def runtime_fixture(body=b'\xc3', direct=False, esp=STACK, ecx=0):
    uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    for base in (0x7e6000, 0x10000000, 0x20000000, 0xfffff000):
        uc.mem_map(base, 0x1000)
    original = image.primary().read(CALL, 2)
    assert original == b'\xff\x10'
    uc.mem_write(CALL, original)
    native = jump(JUMP, CALL)
    uc.mem_write(JUMP, native)
    uc.mem_write(CONT, jump(CONT, END))
    uc.mem_write(TARGET, body)
    uc.mem_write(TABLE, struct.pack('<I', TARGET))
    uc.reg_write(R.UC_X86_REG_EAX, TABLE)
    uc.reg_write(R.UC_X86_REG_ESP, esp)
    uc.reg_write(R.UC_X86_REG_ECX, ecx)
    binding = return_transport.Binding(0x7e6150, CALL, RETURN, 0, JUMP, CONT, 0,
                                       original.hex(), native.hex(), 'indirect')
    bindings = [binding]
    if direct:
        pair = b'\x68' + struct.pack('<I', 0x7e61aa) + jump(DPUSH+5, DTARGET)
        uc.mem_write(DPUSH, pair)
        uc.mem_write(DCONT, b'\xc3')
        uc.mem_write(DTARGET, b'\xc3')
        bindings.append(return_transport.Binding(
            0x7e6150, 0x7e61a5, 0x7e61aa, 0x48c650, DPUSH, DCONT, DTARGET,
            image.primary().read(0x7e61a5, 5).hex(), pair.hex()))
    runtime = return_transport.Runtime(uc, tuple(bindings))
    trace = []
    def hook(uc, at, size, data):
        if not runtime.step(at, size):
            trace.append((at, runtime.rejection))
            uc.emu_stop()
    uc.hook_add(unicorn.UC_HOOK_CODE, hook)
    return uc, runtime, trace


@pytest.mark.parametrize('cleanup', [0, 4, 8])
@pytest.mark.parametrize('esp', [STACK, 0])
def test_actual_cleanup_and_32bit_stack_wrap(cleanup, esp):
    body = b'\xc3' if cleanup == 0 else b'\xc2' + struct.pack('<H', cleanup)
    uc, runtime, trace = runtime_fixture(body, esp=esp)
    uc.emu_start(JUMP, END, count=100)
    assert not runtime.rejection, trace
    assert runtime.dispatches == 1 and not runtime.frames
    assert uc.reg_read(R.UC_X86_REG_ESP) == (esp + cleanup) & 0xffffffff


def test_recursive_same_indirect_call_has_distinct_owned_slots():
    uc, runtime, trace = runtime_fixture(ecx=3)
    uc.mem_write(CONT, b'\xc3')
    uc.mem_write(TARGET, b'\x85\xc9\x74\x06\x49' + jump(TARGET+5, JUMP) + b'\xc3')
    uc.mem_write(STACK, struct.pack('<I', END))
    uc.emu_start(JUMP, END, count=1000)
    assert not runtime.rejection, trace
    assert runtime.dispatches == 4 and not runtime.frames


@pytest.mark.parametrize('borrowed_outer', [True, False])
def test_mixed_calls_share_one_lifo(borrowed_outer):
    uc, runtime, trace = runtime_fixture(jump(TARGET, DPUSH), direct=True)
    start = JUMP
    if not borrowed_outer:
        start = DPUSH
        uc.mem_write(DTARGET, jump(DTARGET, JUMP))
        uc.mem_write(TARGET, b'\xc3')
        uc.mem_write(CONT, b'\xc3')
        uc.mem_write(DCONT, jump(DCONT, END))
    uc.emu_start(start, END, count=1000)
    assert not runtime.rejection, trace
    assert runtime.dispatches == 2 and not runtime.frames


@pytest.mark.parametrize('body', [
    b'\x83\xc4\x04' + jump(TARGET+3, RETURN),
    b'\x83\xc4\x04\xc7\x04\x24' + struct.pack('<I', RETURN) + b'\xc3',
    jump(TARGET, 0x7e61aa),
])
def test_non_ret_wrong_slot_or_crossed_continuation_rejects(body):
    uc, runtime, trace = runtime_fixture(body, direct=True)
    uc.emu_start(JUMP, END, count=1000)
    assert runtime.rejection, trace
    assert runtime.dispatches == 0


def test_direct_callee_cannot_cross_active_direct_frame_to_borrowed_return():
    uc, runtime, trace = runtime_fixture(jump(TARGET, DPUSH), direct=True)
    uc.mem_write(DTARGET, jump(DTARGET, RETURN))
    uc.emu_start(JUMP, END, count=1000)
    assert runtime.rejection == 'original continuation has no matching active call', trace
    assert runtime.dispatches == 0


@pytest.mark.parametrize('address', [JUMP+1, CALL+1])
@pytest.mark.parametrize('when', ['before-transfer', 'callee-entry'])
def test_changed_transfer_or_original_call_rejects(address, when):
    uc, runtime, trace = runtime_fixture()
    if when == 'before-transfer':
        uc.mem_write(address, b'\x00')
    else:
        def mutate(uc, at, size, data):
            uc.mem_write(address, b'\x00')
        uc.hook_add(unicorn.UC_HOOK_CODE, mutate, begin=TARGET, end=TARGET)
    uc.emu_start(JUMP, END, count=1000)
    assert runtime.rejection, trace
    assert runtime.dispatches == 0


def compile_fixture(tmp_path, opt='-O0', call=CALL, returnva=RETURN, target=None):
    # This fixture has a real compiled owner and a byte-verified declaration.
    # Getter executes original instructions with RET4, with no behavior stub.
    transfer = call if target is None else target
    text = f'''#include "cmre.h"
void __attribute__((naked)) cm_fn_007e6150(void) {{
    __asm__("movl $0x03000400, %ecx\\n movl $0x0203eff8, %eax\\n pushl $1\\n"
            "cmre_indirect_{call:08x}_{returnva:08x}:\\n"
            ".byte 0xe9\\n.long 0x{transfer:08x} - . - 4\\n"
            "cmre_return_{call:08x}_{returnva:08x}:\\n"
            "ret\\n");
}}
'''
    path = tmp_path / 'indirect.c'
    path.write_text(text, encoding='utf-8')
    return build.build(path, 'cm_fn_007e6150', tmp_path, opt)


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_native_target_read_precedes_return_write_with_same_address(tmp_path, opt):
    compiled = compile_fixture(tmp_path, opt)
    # Entry S, argument S-4, actual CALL return S-8: table aliases that slot.
    arena = bytearray(emu.ARENA_SIZE)
    struct.pack_into('<I', arena, 0x409, 0xcafebabe)
    case = emu.Case({}, [], bytes(arena), {emu.STACK_TOP-8: 0x785830})
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(0x7e6150, lift.discover(0x7e6150).region_end), escape=True)
    result = machine.run(compiled.entry, case, 1000)
    assert result.outcome == 'return', result
    assert result.regs[0] == 0xcafebabe
    relevant = [event for event in result.events if event[1] == emu.STACK_TOP-8]
    assert relevant[:2] == [('r', emu.STACK_TOP-8, 4, 0x785830),
                            ('w', emu.STACK_TOP-8, 4, RETURN)]
    assert bytes(machine.uc.mem_read(CALL, 2)) == b'\xff\x10'
    assert machine.return_runtime.dispatches == 1
    for entry in (CALL, RETURN):
        assert machine.run(entry, case, 1000).outcome == 'escape'


@pytest.mark.parametrize('changes', [
    dict(call=0x7e6188),
    dict(returnva=RETURN+1),
    dict(target=CALL+1),
])
def test_indirect_declaration_must_match_pinned_call_owner_and_jump(tmp_path, changes):
    with pytest.raises(build.BuildError, match='return transport'):
        compile_fixture(tmp_path, **changes)


@pytest.mark.parametrize('intel', [False, True])
def test_public_indirect_macro_in_both_assembler_dialects(tmp_path, intel):
    prefix = ('".intel_syntax noprefix\\n mov ecx, 0x03000400\\n mov eax, 0x0203eff8\\n push 1\\n"'
              if intel else '"movl $0x03000400, %ecx\\n movl $0x0203eff8, %eax\\n pushl $1\\n"')
    suffix = '"ret\\n .att_syntax prefix\\n"' if intel else '"ret\\n"'
    path = tmp_path / 'macro.c'
    path.write_text('#include "cmre.h"\nvoid __attribute__((naked)) cm_fn_007e6150(void) { __asm__('
                    + prefix + '\nCM_ORIGINAL_INDIRECT_CALL(007e6189, 007e618b)\n'
                    + suffix + '); }\n', encoding='utf-8')
    compiled = build.build(path, 'cm_fn_007e6150', tmp_path)
    arena = bytearray(emu.ARENA_SIZE)
    struct.pack_into('<I', arena, 0x409, 0x12345678)
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(0x7e6150, lift.discover(0x7e6150).region_end), escape=True)
    result = machine.run(compiled.entry,
                         emu.Case({}, [], bytes(arena), {emu.STACK_TOP-8: 0x785830}), 1000)
    assert result.outcome == 'return'
    assert result.regs[0] == 0x12345678
    assert machine.return_runtime.dispatches == 1
