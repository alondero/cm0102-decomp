"""Instruction-pinned original stack-probe return-slot migration.

These synthetic caller bodies test the VM seam, not recovered game logic.
The callee is the unchanged original VA00945610 of the pinned primary.
"""
import pytest
from cmre import build, emu, image, lift

OWNER = 0x0095288C
CALL = 0x00952894
RETURN = 0x00952899
PROBE = 0x00945610


def compile_caller(tmp_path, count=0x1000, extra=''):
    source = tmp_path / 'probe.c'
    source.write_text('''#include "cmre.h"
unsigned __attribute__((naked)) cm_fn_0095288c(void) {
    __asm__("pushl %ebp\\n movl %esp,%ebp\\n movl $''' + str(count) + ''',%eax\\n"
        CM_ORIGINAL_CALL(00952894,00952899,00945610)
        "movl %ebp,%esp\\n popl %ebp\\n ret\\n");
}
''' + extra, encoding='utf-8')
    return build.build(source, 'cm_fn_0095288c', tmp_path)


def case():
    regs = {name: 0x12345678 for name in emu.REG_NAMES}
    regs['eflags'] = 0x202
    return emu.Case(regs, [0]*10, bytes(emu.ARENA_SIZE))


def machine(compiled):
    return emu.Machine(image.primary(), compiled.image,
                       watch=(OWNER, lift.discover(OWNER).region_end), escape=True)


@pytest.mark.parametrize('count', [0, 1, 2, 3, 0xfff, 0x1000, 0x1001, 0x2000])
def test_original_probe_can_return_through_its_instruction_proven_moved_slot(tmp_path, count):
    compiled = compile_caller(tmp_path, count)
    vm = machine(compiled)
    for _ in range(2):
        result = vm.run(compiled.entry, case(), 1000)
        assert result.outcome == 'return', (result.detail, vm.return_runtime.rejection)
        assert result.regs[0] == RETURN
        assert result.regs[1] == 0x12345678
        assert vm.return_runtime.dispatches == 1
        assert not vm.return_runtime.frames
        old_slot, moved_slot = emu.STACK_TOP-8, emu.STACK_TOP-8-count
        raw = RETURN.to_bytes(4, 'little')
        assert bytes(vm.uc.mem_read(moved_slot, 4)) == raw
        # Small counts overlap the original word: preserve the real byte writes.
        expected_old = bytes(raw[address-moved_slot] if moved_slot <= address < moved_slot+4
                             else raw[address-old_slot] for address in range(old_slot, old_slot+4))
        assert bytes(vm.uc.mem_read(old_slot, 4)) == expected_old


def test_compiled_imitation_cannot_claim_original_probe_slot_migration(tmp_path):
    imitation = '''
void __attribute__((naked)) cm_fn_00945610(void) {
    __asm__("popl %edx\\n subl $0x1000,%esp\\n pushl %edx\\n ret\\n");
}
'''
    compiled = compile_caller(tmp_path, extra=imitation)
    vm = machine(compiled)
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'escape'
    assert result.detail == (RETURN,)


def test_original_probe_fault_does_not_dispatch_a_continuation(tmp_path):
    compiled = compile_caller(tmp_path, 0x80000)
    vm = machine(compiled)
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'fault'
    assert result.detail[-1] == 0x00945627
    assert vm.return_runtime.dispatches == 0


def test_changed_original_probe_cannot_authorize_moved_slot(tmp_path):
    compiled = compile_caller(tmp_path)
    vm = machine(compiled)
    vm.uc.mem_write(PROBE+0x2d, b'\x90')
    result = vm.run(compiled.entry, case(), 1000)
    assert result.outcome == 'escape'
    assert vm.return_runtime.dispatches == 0


def test_original_probe_ret_from_wrong_slot_is_rejected(tmp_path):
    import unicorn

    compiled = compile_caller(tmp_path)
    vm = machine(compiled)

    def change_slot(uc, address, size, data):
        sp = uc.reg_read(emu.R.UC_X86_REG_ESP)
        uc.mem_write(sp+4, RETURN.to_bytes(4, 'little'))
        uc.reg_write(emu.R.UC_X86_REG_ESP, sp+4)

    # Execute this control before Runtime observes the actual RET.
    # A pre-RET hook after Runtime is too late for a truthful observation.
    hook = vm.uc.hook_add(unicorn.UC_HOOK_CODE, change_slot, begin=0x0094563D, end=0x0094563D)
    result = vm.run(compiled.entry, case(), 1000)
    vm.uc.hook_del(hook)
    assert result.outcome == 'escape'
    assert result.detail == (RETURN,)
    assert vm.return_runtime.dispatches == 0


def test_probe_identity_is_a_local_lookup_not_embedded_original_bytes():
    from cmre import return_transport
    code = image.primary().read(return_transport.STACK_PROBE, return_transport.STACK_PROBE_SIZE)
    assert return_transport.is_stack_probe(code)
    assert not return_transport.is_stack_probe(code[:-1] + bytes([0x90]))
    assert not hasattr(return_transport, "STACK_PROBE_BYTES")
