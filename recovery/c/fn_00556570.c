#include "cmre.h"

/* Object fields by byte offset from ECX (this):
   +0x00 DWORD table base (indexed by a signed byte times 4),
   +0x14 DWORD base of a 29-byte-stride record table (bytes and DWORDs are written
         at computed offsets from it),
   +0x1c a 10-byte sub-record whose DWORD at +0x1e is copied to +0x26 before the
         first call through 0x0068d1b0.
   The stack argument is a pointer to a DWORD key; its DWORD at +0 is compared
   with the DWORD at +0 of the lookup results.
   Globals: 0x00ae1050 is the receiver of the first thiscall, 0x00ae23b4 is the
   base for records of stride 581 * index, 0x00ae1080 receives the first result.
   slots holds four DWORD flags (frame offsets 0x10..0x1f in the listing). */

#define FIELD_U8(base, off) (*(volatile u8 *)((base) + (u32)(off)))
#define FIELD_U16(base, off) (*(volatile u16 *)((base) + (u32)(off)))
#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (u32)(off)))

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 param_1, u32 param_2);
extern void CM_THISCALL cm_fn_00556d00(u32 self, u32 param_2, u32 param_3);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 param_1);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 param_2, s32 param_3);

void CM_THISCALL cm_fn_00556570(u32 self, u32 arg1)
{
    u8 slots[0x10];
    u32 slot_base;
    u32 this_ptr;
    u32 argp;
    u32 v0;
    u32 vcur;
    u32 base14;
    u32 ebp_rec;
    u32 ptr_slot;
    u32 idx5;
    u32 t_ptr;
    u32 esi_v;
    u32 ebx_v;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 obj;
    u32 combined;
    u32 k;
    u32 addr;
    u8 al;
    u8 i8;
    s32 sk;

    slot_base = (u32)&slots[0];
    this_ptr = self;
    argp = arg1;

    /* 0x00556577..0x0055657f */
    esi_v = 0;
    ecx = argp;
    FIELD_U32(slot_base, 0) = 0;

    /* 0x00556583..0x005565a9 */
    base14 = FIELD_U32(this_ptr, 0x14);
    v0 = FIELD_U32(ecx, 0);
    ecx = CM_GLOBAL(volatile u32, 0x00ae1050);
    FIELD_U32(slot_base, 4) = 0;
    FIELD_U32(slot_base, 8) = 0;
    FIELD_U32(slot_base, 0xc) = 0;
    edx = v0 * 8u;
    edx = edx - v0;
    ebp_rec = base14 + v0 + edx * 4u;
    ptr_slot = ebp_rec;

    /* 0x005565ad..0x005565d7 */
    eax = cm_fn_004cf5a0(ecx, argp);
    CM_GLOBAL(volatile u32, 0x00ae1080) = eax;
    if (eax == 0) {
        t_ptr = 0;
    } else {
        eax = FIELD_U32(eax, 4);
        edx = CM_GLOBAL(volatile u32, 0x00ae23b4);
        ecx = eax * 145u;
        edx = edx + eax;
        t_ptr = edx + ecx * 4u;
    }

    /* 0x005565db..0x005565e6 */
    ecx = FIELD_U32(this_ptr, 0x1e);
    ebx_v = this_ptr + 0x1cu;
    FIELD_U32(ebx_v, 0xa) = ecx;
    ecx = ebx_v;
    esi_v = cm_fn_0068d1b0(ecx);
    if (esi_v == 0) {
        goto after_loop1;
    }

loop1_head:
    /* 0x005565f5..0x005565fa */
    al = FIELD_U8(ebp_rec, 0x1c);
    if ((s8)al <= 0) {
        goto after_loop1;
    }
    /* 0x00556600..0x00556608 */
    edx = argp;
    eax = FIELD_U32(esi_v, 0);
    vcur = FIELD_U32(edx, 0);
    if (eax != vcur) {
        goto loop1_next;
    }

    /* 0x00556610..0x0055661b */
    sk = (s32)(s8)FIELD_U8(esi_v, 4);
    k = (u32)sk;
    ecx = ebp_rec + k * 4u;
    addr = ecx + k;
    al = FIELD_U8(addr, 0);
    if (al == 0xffu) {
        goto loop1_store;
    }

    /* 0x0055661f..0x0055662f */
    ecx = FIELD_U32(this_ptr, 0);
    eax = (u32)(s32)(s8)al;
    ecx = FIELD_U32(ecx, eax * 4u);
    eax = cm_fn_007e82e0(ecx, vcur, 0);
    ebp_rec = eax;
    if (ebp_rec == 0) {
        goto loop1_restore;
    }

    /* 0x00556635..0x00556649 */
    /* mov ax, [ebp+6]: one read, tested and pushed with EAX's upper half. */
    combined = (ebp_rec & 0xffff0000u) | FIELD_U16(ebp_rec, 6);
    if ((s16)combined > -1) {
        cm_fn_00556d00(this_ptr, FIELD_U32(argp, 0), combined);
    }

    /* 0x0055664e..0x0055667d */
    edx = argp;
    ecx = (u32)(s32)(s8)FIELD_U8(esi_v, 4);
    eax = FIELD_U32(edx, 0);
    ecx = ecx * 5u;
    edx = eax * 8u;
    edx = edx - eax;
    eax = eax + edx * 4u;
    eax = eax + ecx;
    ecx = FIELD_U32(this_ptr, 0x14);
    FIELD_U8(ecx + eax, 0) = 0xffu;
    edx = FIELD_U32(this_ptr, 0x14);
    FIELD_U32(edx + eax + 1u, 0) = 0xffffffffu;
    FIELD_U32(ebp_rec, 0) = 0xffffffffu;

loop1_restore:
    /* 0x00556684: ebp comes back from the saved record pointer */
    ebp_rec = ptr_slot;

loop1_store:
    /* 0x00556688..0x005566af: flag the slot, copy 5 bytes, then 0x0068d1f0 */
    sk = (s32)(s8)FIELD_U8(esi_v, 4);
    ecx = esi_v + 5u;
    k = (u32)sk;
    edx = ebp_rec + k * 4u;
    FIELD_U32(slot_base, k * 4u) = 1;
    eax = k + edx;
    edx = FIELD_U32(ecx, 0);
    FIELD_U32(eax, 0) = edx;
    FIELD_U8(eax, 4) = FIELD_U8(ecx, 4);
    ecx = ebx_v;
    (void)cm_fn_0068d1f0(ecx, esi_v);
    FIELD_U8(ebp_rec, 0x1c) = (u8)(FIELD_U8(ebp_rec, 0x1c) - 1u);

loop1_next:
    /* 0x005566b2..0x005566bd */
    ecx = ebx_v;
    esi_v = cm_fn_0068d1b0(ecx);
    if (esi_v != 0) {
        goto loop1_head;
    }

after_loop1:
    /* 0x005566c3..0x005566d5 */
    al = 0;
    i8 = al;
    idx5 = 0;
    ptr_slot = slot_base;

loop2_head:
    /* 0x005566d9..0x005566e6 */
    ecx = ptr_slot;
    if (FIELD_U32(ecx, 0) != 0) {
        goto loop2_next;
    }
    al = i8;
    if (al == 3) {
        goto loop2_next;
    }

    /* 0x005566ee..0x005566f7 */
    edx = idx5;
    al = FIELD_U8(edx + ebp_rec, 0);
    if (al == 0xffu) {
        goto loop2_next;
    }

    /* 0x005566fd..0x00556722 */
    ecx = FIELD_U32(this_ptr, 0);
    eax = (u32)(s32)(s8)al;
    ecx = FIELD_U32(ecx, eax * 4u);
    obj = ecx;
    eax = FIELD_U32(ecx, 0x17);
    if (eax == 0xffffffffu) {
        goto case_minus_one;
    }
    edx = t_ptr;
    if (edx == 0) {
        goto call_second;
    }
    edx = FIELD_U32(edx, 0x53);
    if (edx == 0) {
        goto call_second;
    }
    if (FIELD_U32(edx, 0) == eax) {
        goto loop2_next;
    }
    goto call_second;

case_minus_one:
    /* 0x00556722..0x00556742 */
    edx = FIELD_U32(obj, 0x1b);
    if (edx == 0xffffffffu) {
        goto call_second;
    }
    eax = t_ptr;
    if (eax == 0) {
        goto call_second;
    }
    eax = FIELD_U32(eax, 0x53);
    if (eax == 0) {
        goto call_second;
    }
    eax = FIELD_U32(eax, 0x71);
    if (eax == 0) {
        goto call_second;
    }
    if (FIELD_U32(eax, 0) == edx) {
        goto loop2_next;
    }

call_second:
    /* 0x00556744..0x00556756 */
    ebx_v = argp;
    esi_v = cm_fn_007e82e0(obj, FIELD_U32(ebx_v, 0), 0);
    if (esi_v == 0) {
        goto loop2_next;
    }
    /* 0x00556758..0x0055676b */
    combined = (esi_v & 0xffff0000u) | FIELD_U16(esi_v, 6);
    if ((s16)combined > -1) {
        cm_fn_00556d00(this_ptr, FIELD_U32(ebx_v, 0), combined);
    }

    /* 0x0055676d..0x00556793 */
    eax = FIELD_U32(ebx_v, 0);
    edx = idx5;
    ecx = eax * 8u;
    ecx = ecx - eax;
    edx = edx + eax;
    eax = edx + ecx * 4u;
    ecx = FIELD_U32(this_ptr, 0x14);
    FIELD_U8(ecx + eax, 0) = 0xffu;
    edx = FIELD_U32(this_ptr, 0x14);
    FIELD_U32(edx + eax + 1u, 0) = 0xffffffffu;
    FIELD_U32(esi_v, 0) = 0xffffffffu;

loop2_next:
    /* 0x00556799..0x005567af */
    al = i8;
    ptr_slot = ptr_slot + 4u;
    idx5 = idx5 + 5u;
    al = (u8)(al + 1u);
    i8 = al;
    if ((s8)al < 4) {
        goto loop2_head;
    }
}
