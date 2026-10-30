"""Pinned original CALL forms; synthetic inputs prove only the VM seam."""
import importlib.util
import os
from pathlib import Path
import struct
import sys

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, env, image, lift, return_transport

FORMS = {'register': (0x94895b, 0x948968, 0x94896a, bytes.fromhex('ffd0')),
         'absolute': (0x944e84, 0x944ef0, 0x944ef6, bytes.fromhex('ff1554719600')),
         'esi': (0x948bbb, 0x948e13, 0x948e15, bytes.fromhex('ffd6')),
         'ebp': (0x94968f, 0x9496d4, 0x9496d6, bytes.fromhex('ffd5')),
         'parent-slot': (0x519690, 0x51970c, 0x51970f, bytes.fromhex('ff5244'))}
REGISTER_FORMS={'register':R.UC_X86_REG_EAX,'esi':R.UC_X86_REG_ESI,'ebp':R.UC_X86_REG_EBP}
JUMP, CONT, TARGET, END = 0x10000000, 0x10000005, 0x10000100, 0x10000500
DPUSH, DCONT, DTARGET = 0x10000200, 0x1000020a, 0x10000300
STACK, IAT = 0x20000800, 0x967154


@pytest.fixture
def transport(monkeypatch):
    path = os.environ.get('CMRE_EXTENDED_CALL_PROTOTYPE')
    module = return_transport
    if path:
        name = 'cmre.extended_call_prototype'
        spec = importlib.util.spec_from_file_location(name, Path(path))
        module = importlib.util.module_from_spec(spec)
        sys.modules[name] = module
        spec.loader.exec_module(module)
        monkeypatch.setattr(build, 'return_transport', module)
        monkeypatch.setattr(emu, 'return_transport', module)
    return module


def jump(at, target):
    return b'\xe9' + struct.pack('<I', (target-at-5) & 0xffffffff)


def fixture(transport, form, cleanup=0, esp=STACK, direct=False):
    owner, call, ret, raw = FORMS[form]
    assert image.primary().read(call, len(raw)) == raw
    uc = unicorn.Uc(unicorn.UC_ARCH_X86, unicorn.UC_MODE_32)
    for base in (0x519000, 0x944000, 0x948000, 0x949000, 0x967000, 0x7e6000,
                 0x10000000, 0x20000000, 0xfffff000):
        uc.mem_map(base, 0x1000)
    uc.mem_write(call, raw)
    uc.mem_write(ret, image.primary().read(ret, 16))
    native = jump(JUMP, call)
    uc.mem_write(JUMP, native)
    uc.mem_write(CONT, jump(CONT, END))
    uc.mem_write(TARGET, b'\xc3' if cleanup == 0 else b'\xc2'+struct.pack('<H', cleanup))
    uc.mem_write(IAT, struct.pack('<I', TARGET))
    uc.reg_write(REGISTER_FORMS.get(form,R.UC_X86_REG_EAX), TARGET)
    if form == 'parent-slot':
        uc.reg_write(R.UC_X86_REG_EDX, IAT - 0x44)
    uc.reg_write(R.UC_X86_REG_ESP, esp)
    bindings = [transport.Binding(owner, call, ret, 0, JUMP, CONT, 0,
                                  raw.hex(), native.hex(), 'indirect')]
    if direct:
        pair = b'\x68'+struct.pack('<I', 0x7e61aa)+jump(DPUSH+5, DTARGET)
        uc.mem_write(DPUSH, pair)
        uc.mem_write(DCONT, b'\xc3')
        uc.mem_write(DTARGET, b'\xc3')
        bindings.append(transport.Binding(0x7e6150, 0x7e61a5, 0x7e61aa, 0x48c650,
            DPUSH, DCONT, DTARGET, image.primary().read(0x7e61a5, 5).hex(), pair.hex()))
    runtime = transport.Runtime(uc, tuple(bindings))
    def hook(uc, at, size, data):
        if not runtime.step(at, size):
            uc.emu_stop()
    uc.hook_add(unicorn.UC_HOOK_CODE, hook)
    return uc, runtime


@pytest.mark.parametrize('form', FORMS)
@pytest.mark.parametrize('cleanup', [0, 4, 8, 24])
@pytest.mark.parametrize('esp', [STACK, 0])
def test_new_forms_observe_actual_cleanup_and_wrapping(transport, form, cleanup, esp):
    uc, rt = fixture(transport, form, cleanup, esp)
    uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is None
    assert rt.dispatches == 1 and not rt.frames
    assert uc.reg_read(R.UC_X86_REG_ESP) == (esp+cleanup) & 0xffffffff


@pytest.mark.parametrize('form', ['absolute', 'parent-slot'])
def test_memory_operand_is_read_before_aliased_return_write(transport, form):
    uc, rt = fixture(transport, form, esp=IAT+4)
    events = []
    uc.hook_add(unicorn.UC_HOOK_MEM_READ | unicorn.UC_HOOK_MEM_WRITE,
        lambda uc, kind, at, size, value, data: events.append((kind, at, size, value)))
    uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is None and rt.dispatches == 1
    relevant = [e for e in events if e[1] == IAT]
    assert relevant[0][:3] == (unicorn.UC_MEM_READ, IAT, 4)
    assert relevant[1] == (unicorn.UC_MEM_WRITE, IAT, 4, FORMS[form][2])


@pytest.mark.parametrize('form', FORMS)
def test_new_forms_recursively_own_distinct_slots(transport, form):
    uc, rt = fixture(transport, form)
    uc.reg_write(R.UC_X86_REG_ECX, 3)
    uc.mem_write(CONT, b'\xc3')
    uc.mem_write(TARGET, b'\x85\xc9\x74\x06\x49'+jump(TARGET+5,JUMP)+b'\xc3')
    uc.mem_write(STACK, struct.pack('<I',END))
    uc.emu_start(JUMP, END, count=1000)
    assert rt.rejection is None and rt.dispatches == 4 and not rt.frames


@pytest.mark.parametrize('form,fault', [(f,k) for f in FORMS
    for k in ('target-read','return-write','target-fetch')
    if not (f in REGISTER_FORMS and k == 'target-read')])
def test_new_forms_leave_faults_to_native_call(transport, form, fault):
    memory_form = form in ('absolute', 'parent-slot')
    uc, rt = fixture(transport, form, esp=IAT+4 if fault == 'return-write' and memory_form else STACK)
    if fault == 'target-read':
        uc.mem_unmap(IAT & ~0xfff, 0x1000)
    elif fault == 'return-write':
        uc.mem_protect((IAT if memory_form else STACK-4) & ~0xfff,
                       0x1000, unicorn.UC_PROT_READ)
    else:
        if form in REGISTER_FORMS:
            uc.reg_write(REGISTER_FORMS[form], 0xdead0000)
        else:
            uc.mem_write(IAT, struct.pack('<I', 0xdead0000))
    with pytest.raises(unicorn.UcError):
        uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is None and rt.dispatches == 0
    assert uc.reg_read(R.UC_X86_REG_EIP) == (0xdead0000 if fault == 'target-fetch' else FORMS[form][1])
    if fault == 'target-fetch':
        assert bytes(uc.mem_read(STACK-4, 4)) == struct.pack('<I', FORMS[form][2])


@pytest.mark.parametrize('form', FORMS)
@pytest.mark.parametrize('indirect_outer', [False, True])
def test_new_forms_and_direct_calls_share_lifo(transport, form, indirect_outer):
    uc, rt = fixture(transport, form, direct=True)
    if indirect_outer:
        start = JUMP
        uc.mem_write(TARGET, jump(TARGET, DPUSH))
    else:
        start = DPUSH
        uc.mem_write(DTARGET, jump(DTARGET, JUMP))
        uc.mem_write(CONT, b'\xc3')
        uc.mem_write(DCONT, jump(DCONT, END))
    uc.emu_start(start, END, count=1000)
    assert rt.rejection is None and rt.dispatches == 2 and not rt.frames


@pytest.mark.parametrize('form', FORMS)
@pytest.mark.parametrize('wrong_slot', [False, True])
def test_new_forms_reject_non_ret_or_wrong_slot(transport, form, wrong_slot):
    uc, rt = fixture(transport, form)
    ret = FORMS[form][2]
    body = (b'\x83\xc4\x04\xc7\x04\x24'+struct.pack('<I', ret)+b'\xc3'
            if wrong_slot else jump(TARGET, ret))
    uc.mem_write(TARGET, body)
    uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is not None and rt.dispatches == 0


@pytest.mark.parametrize('form,offset', [('absolute', 1), ('absolute', 2), ('absolute', 5),
                                      ('parent-slot', 1), ('parent-slot', 2)])
def test_full_operand_bytes_remain_owned(transport, form, offset):
    uc, rt = fixture(transport, form)
    call = FORMS[form][1]
    def mutate(uc, at, size, data):
        uc.mem_write(call+offset, bytes([uc.mem_read(call+offset,1)[0] ^ 1]))
    uc.hook_add(unicorn.UC_HOOK_CODE, mutate, begin=TARGET, end=TARGET)
    uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is not None and rt.dispatches == 0


@pytest.mark.parametrize('form', FORMS)
@pytest.mark.parametrize('intel', [False, True])
@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_pinned_new_macro_forms_redirect_selected_getter(transport, tmp_path, form, intel, opt):
    owner, call, ret, _ = FORMS[form]
    prefix = ('.intel_syntax noprefix\n mov ecx,0x03000400\n mov eax,0x00785830\n push 1\n'
              if intel else 'movl $0x03000400,%ecx\n movl $0x00785830,%eax\n pushl $1\n')
    register={'esi':'esi','ebp':'ebp'}.get(form,'eax')
    prefix=prefix.replace('mov eax,','mov '+register+',').replace(',%eax',',%'+register)
    if form == 'parent-slot':
        prefix += f'mov edx,0x{IAT-0x44:08x}\n' if intel else f'movl $0x{IAT-0x44:08x},%edx\n'
    suffix = 'ret\n .att_syntax prefix\n' if intel else 'ret\n'
    import json
    source = '#include "cmre.h"\nvoid __attribute__((naked)) cm_fn_'+f'{owner:08x}'+\
        '(void) { __asm__('+json.dumps(prefix)+\
        f'\nCM_ORIGINAL_INDIRECT_CALL({call:08x}, {ret:08x})\n'+json.dumps(suffix)+'); }\n'
    source += (env.C_DIR/'fn_00785830.c').read_text()
    path = tmp_path/'new-form.c';path.write_text(source)
    b = build.build(path, f'cm_fn_{owner:08x}', tmp_path, opt)
    arena = bytearray(emu.ARENA_SIZE)
    struct.pack_into('<I', arena, 0x409, 0xcafebabe)
    case = emu.Case({}, [], bytes(arena), {IAT:0x785830})
    m = emu.Machine(image.primary(), b.image)
    for va in (owner, 0x785830):
        m.uc.hook_add(unicorn.UC_HOOK_CODE, m._escape, begin=va, end=lift.discover(va).region_end-1)
    r = m.run(b.entry, case, 1000)
    assert r.outcome == 'return', (r.outcome,r.detail)
    assert r.regs[0] == 0xcafebabe and m.return_runtime.dispatches == 1
    for entry in (call, ret):
        assert m.run(entry, case, 1000).outcome == 'escape'


@pytest.mark.parametrize('raw', [b'\xff\x11', b'\xff\xd1', b'\xff\x15',
    b'\xff\x15\x00\x00\x00', b'\xff\x15\x00\x00\x00\x00\x00',
    b'\x66\xff\xd0', b'\xff\x50\x04'])
def test_new_operand_decoder_rejects_near_miss_forms(transport, raw):
    with pytest.raises(ValueError, match='unsupported'):
        transport._indirect_operand(raw)


@pytest.mark.parametrize('form', FORMS)
@pytest.mark.parametrize('change', ['return', 'owner-site', 'jump-target'])
def test_new_form_bindings_reject_malformed_declarations(transport, tmp_path, form, change):
    owner, call, ret, _ = FORMS[form]
    marker_call = call+1 if change == 'owner-site' else call
    marker_ret = ret+1 if change == 'return' else ret
    target = call+1 if change == 'jump-target' else call
    source = f'''#include "cmre.h"
void __attribute__((naked)) cm_fn_{owner:08x}(void) {{
 __asm__("cmre_indirect_{marker_call:08x}_{marker_ret:08x}:\\n"
 ".byte 0xe9\\n.long 0x{target:08x} - . - 4\\n"
 "cmre_return_{marker_call:08x}_{marker_ret:08x}:\\nret\\n");
}}'''
    path=tmp_path/'malformed.c';path.write_text(source)
    with pytest.raises(build.BuildError, match='return transport'):
        build.build(path, f'cm_fn_{owner:08x}', tmp_path)


@pytest.mark.parametrize('form', ['esi','ebp'])
@pytest.mark.parametrize('cleanup', [0,4,8])
def test_target_register_return_clobber_is_not_repaired(transport,form,cleanup):
    uc,rt=fixture(transport,form,cleanup)
    opcode={'esi':b'\xbe','ebp':b'\xbd'}[form]
    body=opcode+struct.pack('<I',0x12345678)+(b'\xc3' if not cleanup else b'\xc2'+struct.pack('<H',cleanup))
    uc.mem_write(TARGET,body)
    uc.emu_start(JUMP,END,count=100)
    assert rt.rejection is None and rt.dispatches==1 and not rt.frames
    assert uc.reg_read(REGISTER_FORMS[form])==0x12345678
    assert uc.reg_read(R.UC_X86_REG_ESP)==STACK+cleanup


@pytest.mark.parametrize('raw',[b'\xff\xd1',b'\xff\xd3',b'\xff\xd4',b'\xff\xd7',b'\x66\xff\xd6',b'\x66\xff\xd5'])
def test_other_register_and_prefixed_forms_stay_rejected(transport,raw):
    with pytest.raises(ValueError,match='unsupported'):
        transport._indirect_operand(raw)


def test_parent_slot_address_wraps_without_repairing_edx(transport):
    uc, rt = fixture(transport, 'parent-slot', cleanup=24)
    uc.mem_map(0, 0x1000)
    uc.reg_write(R.UC_X86_REG_EDX, 0xffffffd0)
    uc.mem_write(0x14, struct.pack('<I', TARGET))
    # The callee may clobber EDX; the continuation must preserve that result.
    uc.mem_write(TARGET, b'\xba\x78\x56\x34\x12\xc2\x18\x00')
    uc.emu_start(JUMP, END, count=100)
    assert rt.rejection is None and rt.dispatches == 1 and not rt.frames
    assert uc.reg_read(R.UC_X86_REG_EDX) == 0x12345678
    assert uc.reg_read(R.UC_X86_REG_ESP) == STACK+24


@pytest.mark.parametrize('raw', [bytes.fromhex(value) for value in
    ('ff5243', 'ff5245', 'ff52c4', 'ff9244000000', 'ff5144', '66ff5244', 'ff52')])
def test_other_displacement_forms_remain_rejected(transport, raw):
    with pytest.raises(ValueError, match='unsupported'):
        transport._indirect_operand(raw)
