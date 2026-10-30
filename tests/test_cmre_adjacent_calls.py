"""Adjacent pinned IAT CALLs: return versus next-call provenance, not API policy."""
from dataclasses import replace
from types import SimpleNamespace

import pytest
import unicorn
from unicorn import x86_const as R

from cmre import build, emu, image, lift, return_transport

OWNER, FIRST, SHARED, LAST = 0x94fe81, 0x950045, 0x95004b, 0x950051


def chain(tmp_path, opt='-O0'):
    assert image.primary().read(FIRST, 12) == bytes.fromhex('ff15ec709600ff15e8709600')
    source = tmp_path/'adjacent.c'
    source.write_text('''#include "cmre.h"
void __attribute__((naked)) cm_fn_0094fe81(void) {
 __asm__("pushl $0\\n"
 CM_ORIGINAL_INDIRECT_CALL(00950045,0095004b)
 CM_ORIGINAL_INDIRECT_CALL(0095004b,00950051)
 "ret\\n");
}
''')
    built = build.build(source, 'cm_fn_0094fe81', tmp_path, opt)
    return built, emu.Machine(image.primary(), built.image)


def case():
    return emu.Case({name:(0x602 if name=='eflags' else 0)
                     for name in emu.REG_NAMES},[0]*10,bytes(emu.ARENA_SIZE))


@pytest.mark.parametrize('opt', ['-O0', '-O2'])
def test_adjacent_pinned_calls_dispatch_both_real_returns(tmp_path, opt):
    built, vm = chain(tmp_path, opt)
    result = vm.run(built.entry, case(), 1000)
    assert result.outcome == 'return', result.detail
    assert vm.return_runtime.dispatches == 2
    assert not vm.return_runtime.frames and vm.return_runtime.pending is None
    assert result.regs[4] == emu.STACK_TOP+4
    calls = [event for event in result.events if event[0]=='c']
    assert calls == [('c','KERNEL32.dll!CloseHandle',1,(0,)),
                     ('c','KERNEL32.dll!GetLastError',0,())]
    # Each native operand read precedes its original numeric return-word write.
    events = result.events
    for iat,slot,word in [(0x9670ec,emu.STACK_TOP-8,SHARED),
                          (0x9670e8,emu.STACK_TOP-4,LAST)]:
        read = next(n for n,e in enumerate(events) if e[:3]==('r',iat,4))
        write = next(n for n,e in enumerate(events) if e==('w',slot,4,word))
        assert read < write


@pytest.mark.parametrize('at', [FIRST, SHARED, LAST])
def test_adjacent_original_arrivals_without_owned_transfer_reject(tmp_path, at):
    built, vm = chain(tmp_path)
    result = vm.run(at, case(), 1000)
    assert result.outcome == 'escape'
    assert vm.return_runtime.rejection is not None
    assert vm.return_runtime.dispatches == 0


@pytest.mark.parametrize('fault', ['operand-read','return-write'])
def test_second_adjacent_call_keeps_native_fault_and_first_dispatch(tmp_path, fault):
    built, vm = chain(tmp_path)
    second = next(b for b in built.image.return_bindings if b.original_call==SHARED)
    def protect(uc, at, size, data):
        page = 0x967000 if fault=='operand-read' else (emu.STACK_TOP-1)&~0xfff
        permissions = unicorn.UC_PROT_EXEC|(unicorn.UC_PROT_READ if fault=='return-write' else 0)
        uc.mem_protect(page,0x1000,permissions)
    vm.uc.hook_add(unicorn.UC_HOOK_CODE,protect,
                   begin=second.compiled_push,end=second.compiled_push)
    result=vm.run(built.entry,case(),1000)
    assert result.outcome=='fault'
    assert vm.uc.reg_read(R.UC_X86_REG_EIP)==SHARED
    assert vm.return_runtime.rejection is None
    assert vm.return_runtime.dispatches==1


def test_shared_address_cannot_hide_different_pending_owner(tmp_path):
    built, vm=chain(tmp_path)
    original=vm.return_runtime.indirect_jumps.copy()
    for pc,binding in original.items():
        if binding.original_call==SHARED:
            vm.return_runtime.indirect_jumps[pc]=replace(binding,owner=0x94b2a0)
    result=vm.run(built.entry,case(),1000)
    assert result.outcome=='escape'
    assert vm.return_runtime.dispatches==1
    assert vm.return_runtime.rejection=='original continuation has no matching active call'


def test_adjacent_binding_rejects_cross_owner_collision(tmp_path,monkeypatch):
    built,_=chain(tmp_path)
    routine=lift.discover(OWNER)
    # Deliberate false ownership split of the pinned bytes is a negative control.
    primary=dict(routine.insns);second=primary.pop(SHARED)
    def split(va):
        return SimpleNamespace(insns=primary if va==OWNER else {SHARED:second})
    monkeypatch.setattr(lift,'discover',split)
    with pytest.raises(ValueError,match='another owner continuation'):
        return_transport.bind(built.image,built.symbols,
                              {OWNER:built.entry,0x94b2a0:built.entry+1})


def test_pinned_direct_return_can_be_next_indirect_call(tmp_path):
    # RET-only stand-in tests transport, not the original callee's behavior.
    assert image.primary().read(0x947a82,11)==bytes.fromhex('e8654b0000ff15cc709600')
    source=tmp_path/'direct-adjacent.c'
    source.write_text('''#include "cmre.h"
void __attribute__((naked)) cm_fn_0094c5ec(void) { __asm__("ret\\n"); }
void __attribute__((naked)) cm_fn_00947a13(void) {
 __asm__(CM_ORIGINAL_CALL(00947a82,00947a87,0094c5ec)
 CM_ORIGINAL_INDIRECT_CALL(00947a87,00947a8d) "ret\\n");
}
''')
    built=build.build(source,'cm_fn_00947a13',tmp_path)
    vm=emu.Machine(image.primary(),built.image)
    result=vm.run(built.entry,case(),1000)
    assert result.outcome=='return',result.detail
    assert vm.return_runtime.dispatches==2
    assert not vm.return_runtime.frames and vm.return_runtime.pending is None
    assert [event for event in result.events if event[0]=='c']==[
        ('c','KERNEL32.dll!GetCommandLineA',0,())]
    words=[event[3] for event in result.events
           if event[:3]==('w',emu.STACK_TOP-4,4)]
    assert words==[0x947a87,0x947a8d]
