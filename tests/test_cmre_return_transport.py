"""Original CALL words are data, and original text remains readable as data.

Synthetic recovered bodies below exercise the execution seam, not game recovery.
The declared CALL site/target/return bytes are checked against the pinned image.
"""
import pytest

from cmre import build, emu, image, lift

OWNER = 0x007E5160
CALL = 0x007E522F
RETURN = 0x007E5234
CALLEE = 0x00785790


def source(transport=True):
    transfer = '''
        "v_007e522f:\\n"
        "cmre_call_007e522f_007e5234_00785790:\\n"
        "pushl $0x007e5234\\n"
        ".byte 0xe9\\n.long _cm_fn_00785790 - . - 4\\n"
        "cmre_return_007e522f_007e5234:\\n"
    ''' if transport else '"call _cm_fn_00785790\\n"'
    return '''#include "cmre.h"
unsigned long long __attribute__((naked)) cm_fn_007e5160(void) {
    __asm__("pushl $0x12345678\\n"
            "pushl $0\\n"
            "movl $0x03001000, %ecx\\n"
    ''' + transfer + '''
            "v_007e5234:\\n"
            "movl -12(%esp), %eax\\n"
            "movzbl 0x007e5234, %edx\\n"
            "ret\\n");
}
'''


def compile_source(tmp_path, text, opt='-O0', entry='cm_fn_007e5160'):
    path = tmp_path / 'transport.c'
    path.write_text(text, encoding='utf-8')
    return build.build(path, entry, tmp_path, opt)


def fixture():
    regs = dict.fromkeys(emu.REG_NAMES, 0)
    regs['eflags'] = 0x202
    return emu.Case(regs, [0]*10, bytes(emu.ARENA_SIZE))


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_original_return_word_and_code_data_survive_without_text_patches(tmp_path, opt):
    compiled = compile_source(tmp_path, source(), opt)
    original_bytes = image.primary().read(RETURN, 32)
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    result = machine.run(compiled.entry, fixture(), 1000)
    assert result.outcome == 'return', result
    assert result.regs[0] == RETURN
    assert result.regs[2] == original_bytes[0]
    assert bytes(machine.uc.mem_read(RETURN, 32)) == original_bytes
    assert bytes(machine.uc.mem_read(emu.ARENA+0x1005, 4)) == b'\x78\x56\x34\x12'
    assert [event for event in result.events if event[:3] == ('w', emu.STACK_TOP-12, 4)] == [
        ('w', emu.STACK_TOP-12, 4, RETURN)]


def test_ordinary_native_call_still_exposes_its_relocated_word(tmp_path):
    compiled = compile_source(tmp_path, source(False))
    machine = emu.Machine(image.primary(), compiled.image)
    result = machine.run(compiled.entry, fixture(), 1000)
    assert result.outcome == 'return'
    assert compiled.image.text.va <= result.regs[0] < compiled.image.text.end
    assert result.regs[0] != RETURN
    assert result.regs[2] == image.primary().read(RETURN, 1)[0]


@pytest.mark.parametrize('entry', [OWNER, RETURN])
def test_unowned_original_entry_or_return_remains_an_escape(tmp_path, entry):
    compiled = compile_source(tmp_path, source())
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    assert machine.run(entry, fixture(), 1000).outcome == 'escape'


def test_declared_call_target_must_match_pinned_instruction(tmp_path):
    forged = source().replace('cmre_call_007e522f_007e5234_00785790',
                              'cmre_call_007e522f_007e5234_00785830')
    with pytest.raises(build.BuildError, match='return transport'):
        compile_source(tmp_path, forged)


@pytest.mark.parametrize('transfer', [
    'addl $4, %esp\\n movl $0x007e5234, (%esp)\\n ret $4\\n',
    'addl $12, %esp\\n jmp 0x007e5234\\n',
])
def test_same_return_word_and_final_esp_cannot_authorize_wrong_slot_or_non_ret(tmp_path, transfer):
    callee = '''\nvoid __attribute__((naked)) cm_fn_00785790(void) {
        __asm__("''' + transfer + '''");
    }\n'''
    compiled = compile_source(tmp_path, source()+callee)
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    result = machine.run(compiled.entry, fixture(), 1000)
    assert result.outcome == 'escape'
    assert result.detail == (RETURN,)


@pytest.mark.parametrize('intel', [False, True])
def test_macro_emits_owned_native_pair_in_both_assembler_dialects(tmp_path, intel):
    prefix = ('".intel_syntax noprefix\\n push 0x12345678\\n push 0\\n mov ecx, 0x03001000\\n"'
              if intel else '"pushl $0x12345678\\n pushl $0\\n movl $0x03001000, %ecx\\n"')
    suffix = ('"mov eax, DWORD PTR [esp-12]\\n ret\\n .att_syntax prefix\\n"'
              if intel else '"movl -12(%esp), %eax\\n ret\\n"')
    text = '#include "cmre.h"\nunsigned __attribute__((naked)) cm_fn_007e5160(void) { __asm__(' + prefix + '''
        CM_ORIGINAL_CALL(007e522f, 007e5234, 00785790)
    ''' + suffix + '); }\n'
    compiled = compile_source(tmp_path, text)
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    result = machine.run(compiled.entry, fixture(), 1000)
    assert result.outcome == 'return'
    assert result.regs[0] == RETURN
    assert machine.return_runtime.dispatches == 1


def test_nested_composed_calls_dispatch_both_active_frames_and_reset_between_cases(tmp_path):
    nested = '''
void __attribute__((naked)) cm_fn_00785790(void) {
    __asm__(CM_ORIGINAL_CALL(007857d4, 007857d9, 00944cff) "ret $8\\n");
}
void __attribute__((naked)) cm_fn_00944cff(void) { __asm__("ret\\n"); }
'''
    compiled = compile_source(tmp_path, source()+nested)
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    import unicorn
    machine.uc.hook_add(unicorn.UC_HOOK_CODE, machine._escape,
                        begin=CALLEE, end=lift.discover(CALLEE).region_end-1)
    for _ in range(2):
        result = machine.run(compiled.entry, fixture(), 1000)
        assert result.outcome == 'return'
        assert result.regs[0] == RETURN
        assert machine.return_runtime.dispatches == 2
        assert not machine.return_runtime.frames
        assert bytes(machine.uc.mem_read(emu.STACK_TOP-16, 4)) == b'\xd9\x57\x78\x00'


def test_recursive_same_call_site_keeps_distinct_active_stack_frames(tmp_path):
    text = '''#include "cmre.h"
unsigned __attribute__((naked)) cm_fn_0090cfc0(void) {
    __asm__("testl %ecx, %ecx\\n jz 1f\\n decl %ecx\\n"
            CM_ORIGINAL_CALL(0090d06e, 0090d073, 0090cfc0)
            "movl -4(%esp), %eax\\n ret\\n 1: movl $0, %eax\\n ret\\n");
}
'''
    compiled = compile_source(tmp_path, text, entry='cm_fn_0090cfc0')
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(0x0090cfc0, lift.discover(0x0090cfc0).region_end), escape=True)
    case = fixture()
    case.regs['ecx'] = 3
    result = machine.run(compiled.entry, case, 1000)
    assert result.outcome == 'return'
    assert result.regs[0] == 0x0090d073
    assert machine.return_runtime.dispatches == 3
    for offset in (4, 8, 12):
        assert bytes(machine.uc.mem_read(emu.STACK_TOP-offset, 4)) == b'\x73\xd0\x90\x00'


def test_modified_native_pair_is_rejected_before_its_push_executes(tmp_path):
    compiled = compile_source(tmp_path, source())
    machine = emu.Machine(image.primary(), compiled.image,
                          watch=(OWNER, lift.discover(OWNER).region_end), escape=True)
    push = compiled.image.return_bindings[0].compiled_push
    machine.uc.mem_write(push+1, b'\x00')
    result = machine.run(compiled.entry, fixture(), 1000)
    assert result.outcome == 'escape'
    assert result.detail == (push,)
    assert not [event for event in result.events if event[:3] == ('w', emu.STACK_TOP-12, 4)]
