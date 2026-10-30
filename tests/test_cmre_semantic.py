"""Semantic C profile: game-observable effects only (ADR 0002).

Each relaxation has an equivalent case and a near miss that must still fail.
"""
import struct

import pytest

from cmre import difftest, emu, recover, semantic

REGS = dict(zip(emu.REG_NAMES, range(len(emu.REG_NAMES))))


def regs(**values):
    out = [0x1000 + i for i in range(len(emu.REG_NAMES))]
    for name, value in values.items():
        out[REGS[name]] = value
    return tuple(out)


def same(a, b, returns="int", fp_a=(), fp_b=()):
    na = difftest._normalize(a, None, "semantic", returns=returns, fp=fp_a)
    nb = difftest._normalize(b, None, "semantic", returns=returns, fp=fp_b)
    return difftest._diff(na, nb) == {}


def ret(r, memory=None, events=()):
    return emu.Result("return", (), r, list(events), memory or {}, set())


def fault(address, eip, r, memory=None):
    return emu.Result("fault", ((21, address, 4), eip), r, [], memory or {}, set())


def test_fault_ignores_register_allocation_and_faulting_instruction():
    a = fault(emu.ARENA + 8, 0x00450A70, regs(esi=1, esp=emu.STACK_TOP - 12))
    b = fault(emu.ARENA + 8, 0x00450A90, regs(esi=7, esp=emu.STACK_TOP - 40))
    assert same(a, b)


def test_fault_on_a_different_data_address_still_fails():
    assert not same(fault(emu.ARENA + 8, 0x00450A70, regs()), fault(emu.ARENA + 12, 0x00450A70, regs()))


def test_return_ignores_scratch_registers_for_an_int_return():
    assert same(ret(regs(ecx=1, edx=2)), ret(regs(ecx=3, edx=4)))


def test_void_return_ignores_eax_and_edx_but_preserves_observable_effects():
    a = ret(regs(eax=1, edx=2), {emu.ARENA: bytes(emu.PAGE)})
    b = ret(regs(eax=3, edx=4), {emu.ARENA: bytes(emu.PAGE)})
    assert same(a, b, returns="void")
    assert not same(a, ret(regs(eax=3, edx=4, ebx=5), b.memory), returns="void")
    assert not same(a, ret(b.regs, {emu.ARENA: b"\x01" * emu.PAGE}), returns="void")
    assert not same(a, b, returns="void", fp_a=(0x37F, 0), fp_b=(0x27F, 0))
    assert not same(a, b, returns="void", fp_a=(0x37F, 0), fp_b=(0x37F, 7))


@pytest.mark.parametrize("name", ["eax", "ebx", "esi", "edi", "ebp", "esp"])
def test_return_value_and_callee_saved_registers_still_fail(name):
    assert not same(ret(regs()), ret(regs(**{name: 0xDEAD})))


def test_declared_64_bit_return_compares_edx():
    assert not same(ret(regs(edx=2)), ret(regs(edx=4)), returns="int64")


@pytest.mark.parametrize("returns, mask", [("int8", 0xFF), ("int16", 0xFFFF)])
def test_narrow_return_compares_only_its_width(returns, mask):
    assert same(ret(regs(eax=0x03000300 & ~mask | 0x12)), ret(regs(eax=0x12)), returns=returns)
    assert not same(ret(regs(eax=0x13)), ret(regs(eax=0x12)), returns=returns)


def test_float_return_compares_st0_not_eax():
    assert same(ret(regs(eax=1)), ret(regs(eax=2)), returns="float", fp_a=(0x37F, 7, 5), fp_b=(0x37F, 7, 5))
    assert not same(ret(regs()), ret(regs()), returns="float", fp_a=(0x37F, 7, 5), fp_b=(0x37F, 7, 6))


def test_fpu_control_word_and_stack_depth_are_compared():
    assert not same(ret(regs()), ret(regs()), fp_a=(0x37F, 0), fp_b=(0x27F, 0))
    assert not same(ret(regs()), ret(regs()), fp_a=(0x37F, 0), fp_b=(0x37F, 7))


def test_dead_and_argument_stack_bytes_are_ignored():
    page = emu.STACK_TOP & ~(emu.PAGE - 1)
    assert same(ret(regs(), {page: b"\x01" * emu.PAGE}), ret(regs(), {page: b"\x02" * emu.PAGE}))


def test_arena_memory_still_fails():
    assert not same(ret(regs(), {emu.ARENA: b"\x01" * emu.PAGE}), ret(regs(), {emu.ARENA: b"\x02" * emu.PAGE}))


def test_stack_addresses_stored_as_values_are_layout_dependent():
    # An SEH registration links the thread block to a frame-dependent stack slot.
    def stored(value):
        return {emu.ARENA: struct.pack("<I", value) + bytes(emu.PAGE - 4)}
    assert same(ret(regs(), stored(emu.STACK_TOP - 0x20)), ret(regs(), stored(emu.STACK_TOP - 0x48)))
    assert not same(ret(regs(), stored(emu.STACK_TOP - 0x20)), ret(regs(), stored(emu.ARENA + 0x20)))


def test_import_call_arguments_still_fail():
    call = ("c", "KERNEL32.dll!HeapFree", 3, (0, 0, emu.ARENA))
    other = ("c", "KERNEL32.dll!HeapFree", 3, (0, 0, emu.ARENA + 4))
    assert same(ret(regs(), events=[call]), ret(regs(), events=[call]))
    assert not same(ret(regs(), events=[call]), ret(regs(), events=[other]))
    assert not same(ret(regs(), events=[call]), ret(regs()))


@pytest.mark.parametrize("text, kind", [
    ("u32 CM_THISCALL cm_fn_00450a70(void *self)\n{", "int"),
    ("void *CM_THISCALL\ncm_fn_00450a70(void *self, u32 flags) {", "int"),
    ("void CM_THISCALL cm_fn_00450a70(void *self) {", "void"),
    ("/* Sets a field. */\nvoid cm_fn_00450a70(void) {", "void"),
    ("/* Sets {a field}. */\nvoid cm_fn_00450a70(void) {", "void"),
    ("// Original returns void.\nu32 cm_fn_00450a70(void) {", "int"),
    ("/* void cm_fn_00450a70(void) { } */\nu32 cm_fn_00450a70(void) {", "int"),
    ("unsigned long long CM_CDECL cm_fn_00450a70(void)\n{", "int64"),
    ("static s64 cm_fn_00450a70(u32 a) {", "int64"),
    ("double CM_CDECL cm_fn_00450a70(void) {", "float"),
    ("u8 CM_THISCALL cm_fn_00450a70(void *self) {", "int8"),
    ("char cm_fn_00450a70(void) {", "int8"),
    ("unsigned short CM_THISCALL cm_fn_00450a70(void *self) {", "int16"),
    ("s16 cm_fn_00450a70(void) {", "int16"),
    ("u8 *cm_fn_00450a70(void) {", "int"),
    ("extern u64 cm_fn_00450a70(void);\nfloat CM_STDCALL cm_fn_00450a70(void) {", "float"),
    # A backslash-continued macro ends at its last physical line (fn_0053a8b0).
    ("#define COPY(d, s) \\\n    (*(u32 *)(d) = *(u32 *)(s))\nvoid cm_fn_00450a70(void *d) {", "void"),
    ("#define ZERO(d) \\\n    ((void)(d))\nu32 cm_fn_00450a70(void) {", "int"),
    ("#define STEP(t) \\\n  __asm__ volatile(t : \"+a\"(x) \\\n  : : \"memory\")\nu8 cm_fn_00450a70(void) {", "int8"),
])
def test_declared_return_kind_comes_from_the_definition(text, kind):
    assert semantic.declared_return(text, 0x00450A70) == kind


def test_declared_return_requires_a_definition():
    with pytest.raises(ValueError):
        semantic.declared_return("extern u32 cm_fn_00450a70(void);", 0x00450A70)


@pytest.mark.parametrize("code, reads", [
    (b"\x89\xd1", True),                       # mov ecx, edx
    (b"\x31\xd2\x89\xd1", False),              # xor edx, edx ; mov ecx, edx
    (b"\xc3", False),                          # ret: EDX is dead for a 32-bit caller
    (b"\x52", True),                           # push edx
    (b"\x8b\x10\x89\xd1", False),              # mov edx, [eax] ; mov ecx, edx
    (b"\x89\x10", True),                       # mov [eax], edx
    (b"\xe8\x00\x00\x00\x00\x89\xd1", False),  # a later call clobbers EDX first
    (b"\x8a\x56\x43\x52", False),              # mov dl, [esi+0x43] ; push edx: a char argument (0x00686646)
    (b"\x8a\x56\x43\x89\xd1", True),           # mov dl, [esi+0x43] ; mov ecx, edx: upper bits observed
    (bytes.fromhex("66 8b 57 02 8b 46 04 66 42 8b 48 18 52"), False),  # VA 0x005a9843 + 5
    (bytes.fromhex("8a 50 37 8b 44 24 2c 5f 5e 5d 88 10"), False),  # VA 0x0048cdcc + 5
    (bytes.fromhex("66 8b 17 88 d1 88 f1"), False),  # DX rewritten, then DL and DH read
    (bytes.fromhex("8a 17 88 f1"), True),  # DL rewritten, DH still belongs to callee
    (bytes.fromhex("30 d2 88 d1"), False),  # xor dl, dl ; mov cl, dl
    (bytes.fromhex("8a 17 66 31 d2 89 d1"), True),  # xor dx, dx leaves upper EDX live
    (bytes.fromhex("b2 00 ff 32"), True),  # mov dl,0 ; push [edx]: address needs upper EDX
])
def test_caller_edx_scan(code, reads):
    assert semantic.reads_edx(code, 0x00401000) == reads


@pytest.mark.parametrize("code, width, wider", [
    (b"\x84\xc0", 1, False),           # test al, al
    (b"\x0f\xb6\xc0", 1, False),       # movzx eax, al
    (b"\x85\xc0", 1, True),            # test eax, eax
    (b"\x66\x85\xc0", 2, False),       # test ax, ax
    (b"\x8a\xcc", 1, True),            # mov cl, ah: AH lies outside an 8-bit return
    (b"\x89\xc1", 2, True),            # mov ecx, eax
    (b"\x31\xc0\x85\xc0", 1, False),   # xor eax, eax first
    (bytes.fromhex("66 8b 07 66 40 88 e1"), 1, False),  # AX rewritten before INC AX and AH read
    (bytes.fromhex("8a 27 88 e1"), 1, False),  # AH rewritten before AH read
    (bytes.fromhex("8a 07 89 c1"), 1, True),  # AL rewritten, upper EAX still live
    (bytes.fromhex("b0 00 50"), 1, False),  # narrow argument in EAX
    (bytes.fromhex("b0 00 ff 30"), 1, True),  # push [eax] still reads upper address bits
])
def test_caller_wide_eax_scan(code, width, wider):
    assert semantic.reads_wider_eax(code, 0x00401000, width) == wider


@pytest.mark.parametrize("code, refused", [
    (bytes.fromhex("89 c1"), True),       # mov ecx, eax
    (bytes.fromhex("88 c1"), True),       # mov cl, al
    (bytes.fromhex("8a 07 88 c1"), False),  # rewritten AL
    (bytes.fromhex("31 c0 89 c1"), False),  # rewritten EAX
    (bytes.fromhex("b0 00 ff 30"), True),  # push [eax] uses unwritten upper EAX
])
def test_void_contract_refuses_a_caller_reading_eax(monkeypatch, code, refused):
    va, site = 0x00450A70, 0x00401000
    monkeypatch.setattr(semantic, "_call_sites", lambda: {va: (site,)})
    class CallerImage:
        def read(self, address, size):
            assert address == site + 5
            return code
    monkeypatch.setattr(semantic.image, "primary", CallerImage)
    returns, problems = recover._semantic_contract(va, "void cm_fn_00450a70(void) {")
    assert returns == "void"
    assert bool(problems) == refused
    if refused:
        assert "EAX" in problems[0] and "0x00401000" in problems[0]


SCALAR_DTOR = """#include "cmre.h"
extern void CM_THISCALL cm_fn_007e6150(void *self);
extern void CM_CDECL cm_fn_00944c94(void *p);
void *CM_THISCALL cm_fn_007e6130(void *self, u32 flags)
{
    cm_fn_007e6150(self);
    if (flags & 1)
        cm_fn_00944c94(self);
    return self;
}
"""


@pytest.mark.parametrize("source, status", [
    (SCALAR_DTOR, "pass"),
    (SCALAR_DTOR.replace("flags & 1", "flags & 2"), "fail"),
    (SCALAR_DTOR.replace("return self;", "return 0;"), "fail"),
])
def test_idiomatic_scalar_deleting_destructor(tmp_path, source, status):
    path = tmp_path / "fn_007e6130.c"
    path.write_text(source, encoding="utf-8")
    report = recover.test_c(0x007E6130, fresh=True, source=path, profile="semantic")
    assert report["profile"] == "semantic"
    assert report["status"] == status, report["builds"]


def test_existing_abi_record_keeps_its_profile():
    assert recover.c_profile(0x007E6130) == "abi"


def test_new_c_defaults_to_semantic():
    assert recover.c_profile(0x005A53C0) == "semantic"


# Indirect CALL/JMP targets that cannot be compared (interior original code, or no code).

def listing(hexcode: str, va: int = 0x00450A70):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, bytes(i.bytes)) for i in md.disasm(bytes.fromhex(hexcode), va)]


def shape_at(hexcode: str, site: int, labels=()):
    insns = listing(hexcode)
    return semantic.indirect_calls(insns, insns[0][0], set(labels))[insns[site][0]]


@pytest.mark.parametrize("hexcode, site, labels, ecx, args", [
    # push esi; mov esi, ecx; push edi; mov edi, [esi]; push 0; lea ecx, [esi+0x10]; push ecx;
    # mov ecx, esi; call [edi+0x3c]: saved registers are not arguments; ECX is the object.
    ("568bf1578b3e6a008d4e10518bceff573c5f5ec3", 8, (), True, 2),
    # mov eax, [ecx]; mov ecx, [ecx+8]; push ecx; call [eax+0xc]: ECX only carried an argument.
    ("8b018b490851ff500cc3", 3, (), False, 1),
    # push 1; mov ecx, [ecx]; call [ecx+8]: ECX addresses the slot, so it is not the object.
    ("6a018b09ff5108c3", 2, (), False, 1),
    # push 1; call rel; push 2; mov ecx, esi; call eax: pushes stop at the previous CALL.
    ("6a01e8000000006a028bceffd0c3", 4, (), True, 1),
    # push 1; push 2 (a branch target); call eax: pushes before a label may be another path's.
    ("6a016a02ffd0c3", 2, (0x00450A72,), False, 1),
    # push 1; call rel; add esp, 4; push 2; call eax: cdecl cleanup ends the window.
    ("6a01e80000000083c4046a02ffd0c3", 4, (), False, 1),
    # push ebp; mov ebp, esp; sub esp, 8; push esi; mov esi, ecx; push 0; mov ecx, esi; call [esi+4]
    ("558bec83ec08568bf16a008bceff5604c3", 7, (), True, 1),
    # push 1; jmp eax: a tail transfer has the same argument shape.
    ("6a01ffe0", 1, (), False, 1),
])
def test_indirect_call_shape_comes_from_the_listing(hexcode, site, labels, ecx, args):
    assert shape_at(hexcode, site, labels) == semantic.IndirectCall(ecx, args)


@pytest.mark.parametrize("hexcode, site", [
    ("516a00ffd0c3", 2),          # push ecx (unwritten scratch: a local reservation); push 0; call eax
    ("6a0183ec08ffd0c3", 2),      # push 1; sub esp, 8; call eax: arguments stored, not pushed
])
def test_indirect_call_arguments_unknown_when_the_listing_does_not_show_them(hexcode, site):
    assert shape_at(hexcode, site).args is None


OBJECT = emu.ARENA + 0x88


def stop(target, site, ecx, words, shape=None, memory=None, outcome="interior"):
    return emu.Result(outcome, (target, site, ecx, tuple(words), shape), regs(), [], memory or {}, set())


def same_stop(a, b):
    na = difftest._normalize(a, None, "semantic")
    nb = difftest._normalize(b, None, "semantic", shape=difftest.stop_shape(a))
    return difftest._diff(na, nb) == {}


THISCALL_4 = semantic.IndirectCall(True, 4)


def test_indirect_stop_ignores_site_scratch_registers_and_words_beyond_the_arguments():
    a = stop(0x00873401, 0x00925CD5, OBJECT, (0xFFFFFFFF, OBJECT + 0xA9, OBJECT + 0x3A, 0, 5), THISCALL_4)
    b = stop(0x00873401, 0x10001234, OBJECT, (0xFFFFFFFF, OBJECT + 0xA9, OBJECT + 0x3A, 0, 9))
    assert same_stop(a, b)


def test_indirect_stop_canonicalises_stack_addresses_passed():
    a = stop(0x00873401, 0, emu.STACK_TOP - 0x20, (emu.STACK_TOP - 0x10,), semantic.IndirectCall(True, 1))
    b = stop(0x00873401, 0, emu.STACK_TOP - 0x48, (emu.STACK_TOP - 0x30,))
    assert same_stop(a, b)


@pytest.mark.parametrize("change", ["target", "ecx", "argument", "memory", "outcome"])
def test_indirect_stop_near_misses_fail(change):
    words = (0xFFFFFFFF, OBJECT + 0xA9, OBJECT + 0x3A, 0)
    a = stop(0x00873401, 0, OBJECT, words, THISCALL_4, {emu.ARENA: bytes(emu.PAGE)})
    b = stop(0x00873401, 0, OBJECT, words, None, {emu.ARENA: bytes(emu.PAGE)})
    if change == "target":
        b = stop(0x00873405, 0, OBJECT, words, None, b.memory)
    elif change == "ecx":  # a vtable slot written as stdcall without `this` (fn_005f3e70)
        b = stop(0x00873401, 0, 0x1234, words, None, b.memory)
    elif change == "argument":
        b = stop(0x00873401, 0, OBJECT, words[:3] + (1,), None, b.memory)
    elif change == "memory":
        b = stop(0x00873401, 0, OBJECT, words, None, {emu.ARENA: b"\x01" * emu.PAGE})
    else:
        b = stop(0x00873401, 0, OBJECT, words, None, b.memory, outcome="bad-target")
    assert not same_stop(a, b)


def test_indirect_stop_ecx_ignored_when_the_listing_does_not_pass_it():
    a = stop(0x00000010, 0, OBJECT, (1,), semantic.IndirectCall(False, 1), outcome="bad-target")
    b = stop(0x00000010, 0, 0x1234, (1,), outcome="bad-target")
    assert same_stop(a, b)


SLOT_INIT = """#include "cmre.h"
typedef u32 (CM_THISCALL *slot_t)(void *self, s32 a, void *b, void *c, s32 d);
u32 CM_THISCALL cm_fn_00925c50(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u8 *vtable = *(u8 *volatile *)base;

    *(volatile u16 *)(base + 0xab) = 0;
    *(volatile u16 *)(base + 0xdb) = 0;
    base[0xbe] = 0;
    base[0xbf] = 0;
    base[0xc0] = 0;
    base[0xc1] = 0;
    base[0x52] = 0;
    base[0xc2] = 3;
    base[0xc7] = 3;
    base[0x4a] = 3;
    *(volatile u16 *)(base + 0x3c) = 2;
    base[0xc4] = 2;
    base[0xc6] = 2;
    base[0xc3] = 1;
    base[0x42] = 1;
    base[0xc5] = 1;
    *(volatile u32 *)(base + 0x1c) = 0xffffffff;
    *(volatile u32 *)(base + 0x20) = 0xffffffff;
    base[0x49] = 5;
    *(volatile u32 *)(base + 0xba) = (*(slot_t *)(vtable + 0x3c))(
        self, -1, (void *)(base + 0xa9), (void *)(base + 0x3a), 0);
    return 1;
}
"""
STDCALL_SLOT = (SLOT_INIT.replace("(CM_THISCALL *slot_t)(void *self, ", "(CM_STDCALL *slot_t)(")
                .replace("(\n        self, -1,", "(\n        -1,"))


@pytest.mark.parametrize("source, status", [(SLOT_INIT, "pass"), (STDCALL_SLOT, "fail")])
def test_object_initialiser_calling_an_interior_slot_target(tmp_path, source, status):
    # Case 72: the vtable aliases the object, so the +0xab store makes the slot 0x00873401.
    path = tmp_path / "fn_00925c50.c"
    path.write_text(source, encoding="utf-8")
    report = recover.test_c(0x00925C50, fresh=True, source=path, profile="semantic")
    assert report["status"] == status, report["builds"]


SEH_FRAMED = """#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, void *arg1, u32 arg2);

struct seh_registration { void *prev; u32 handler; u32 state; };

void CM_STDCALL cm_fn_005a02f0(s32 param_1)
{
    u8 buffer[224];
    volatile struct seh_registration frame;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095918bu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    cm_fn_0077bbe0(buffer, 0x1783u, 0u);
    frame.state = 0u;
    cm_fn_00785790(buffer, 0u, (u32)param_1);
    cm_fn_007861e0(CM_GLOBAL(void *, 0x00b7431c), buffer,
                   (u32)CM_GLOBAL(s32, 0x00ae23b4) + (u32)param_1 * 0x245u);
    CM_FS(void *, 0) = frame.prev;
}
"""


@pytest.mark.parametrize("source, status", [
    (SEH_FRAMED, "pass"),
    # The frame stays linked after return: the thread block keeps a dead stack address.
    (SEH_FRAMED.replace("    CM_FS(void *, 0) = frame.prev;\n", ""), "fail"),
])
def test_seh_registration_through_the_fs_segment(tmp_path, source, status):
    path = tmp_path / "fn_005a02f0.c"
    path.write_text(source, encoding="utf-8")
    report = recover.test_c(0x005A02F0, fresh=True, source=path, profile="semantic")
    assert report["status"] == status, report["builds"]
