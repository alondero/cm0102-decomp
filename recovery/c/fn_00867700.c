#include "cmre.h"

/* Memory access helpers. Volatile keeps the listing's load/store order and
   the byte, word and dword widths for every access. */
#define P8(a) (*(volatile u8 *)(u32)(a))
#define P16(a) (*(volatile u16 *)(u32)(a))
#define P32(a) (*(volatile u32 *)(u32)(a))
#define S8(a) ((s32)(s8)P8(a))
#define S16(a) ((s32)(s16)P16(a))

extern u32 CM_THISCALL cm_fn_00866f60(u32 self);
extern u32 CM_THISCALL cm_fn_008cca10(u32 self, u32 arg2, u32 arg3);
extern u32 CM_THISCALL cm_fn_008c1870(u32 self, u32 arg2);
extern void CM_THISCALL cm_fn_004d8610(u32 self, u32 arg1);
extern u32 cm_fn_00600a50(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 arg1);
extern void CM_CDECL cm_fn_00547af0(u32 object, u32 value, u32 selector);
extern void CM_CDECL cm_fn_00549cf0(u32 p1, u32 p2, u32 val);

/* Thiscall (this in ECX, two stack arguments popped by the callee).
   Record size 0x6e (110) bytes at G2 + 110 * [this], the record used here
   is P_rec. Globals: 0x00ae23b4 (base added to [this+4] * 581),
   0x00ae23bc (record table), 0x00de1f54 (object), 0x00ae1050 (object).
   Void: no direct caller reads EAX after the call (cmre semantic.eax_readers). */
void CM_THISCALL cm_fn_00867700(u32 self, u32 arg1, u32 arg2)
{
    u32 esi;
    u32 ebx;
    u32 ebp;
    u32 edi;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 cl;
    u32 r;
    unsigned long long prod;
    s32 t2;
    s32 t3;

    esi = self;
    edx = P32(0x00ae23b4u);
    eax = P32(esi + 4);
    edx = edx + eax;
    ecx = eax * 145u;
    eax = P32(esi);
    edi = edx + ecx * 4u;
    ecx = eax * 5u;
    ecx = eax + ecx * 2u;
    edx = ecx * 5u;
    ecx = P32(0x00ae23bcu);
    cl = P8(ecx + edx * 2u + 0x3du);
    if (cl == 0x0bu) {
        goto set_path;
    }
    if (cl == 0x0fu) {
        goto set_path;
    }
    if (cl != 0x0du) {
        return;
    }

set_path:
    ebx = arg1;
    ebp = P32(esi + 0x45);
    ebp = ebp | ebx;
    P32(esi + 0x45) = ebp;
    ecx = P32(edi + 0xcf);
    if (ecx == 0) {
        goto chk_d3;
    }
    if (eax == P32(ecx)) {
        goto accept_call;
    }

chk_d3:
    ecx = P32(edi + 0xd3);
    if (ecx == 0) {
        goto chk_44;
    }
    if (eax == P32(ecx)) {
        goto accept_call;
    }

chk_44:
    edx = (u32)S8(esi + 0x44);
    if ((edx & 0x10000u) != 0) {
        goto accept_call;
    }
    eax = (eax & 0xffffff00u) | P8(esi + 0x49);
    if ((u8)eax != 0) {
        goto accept_call;
    }
    ecx = esi;
    eax = cm_fn_00866f60(ecx);
    if (eax == 0) {
        goto accept_call;
    }

    /* Record P_rec = G2 + 110 * [esi] (the listing's 0x8677a0 block). */
    eax = P32(esi);
    ecx = eax * 5u;
    eax = eax + ecx * 2u;
    edx = eax * 5u;
    eax = P32(0x00ae23bcu);
    ecx = eax + edx * 2u;
    /* push 0 first, then push P_rec; ECX = [0x00de1f54]. */
    eax = cm_fn_008cca10(P32(0x00de1f54u), ecx, 0u);
    if (eax != 0) {
        goto accept_call;
    }
    if ((P8(esi + 0x4e) & 9u) != 0) {
        goto accept_call;
    }
    /* 0x8677c9: P_rec again, then ECX = [0x00de1f54] and push P_rec. */
    eax = P32(esi);
    ecx = P32(0x00ae23bcu);
    edx = eax * 5u;
    eax = eax + edx * 2u;
    eax = eax * 5u;
    edx = ecx + eax * 2u;
    eax = cm_fn_008c1870(P32(0x00de1f54u), edx);
    if (eax != 0) {
        goto accept_call;
    }

    eax = P32(esi);
    ecx = eax * 5u;
    eax = eax + ecx * 2u;
    edx = eax * 5u;
    eax = P32(0x00ae23bcu);
    cl = P8(eax + edx * 2u + 0x18u);
    eax = eax + edx * 2u;
    if ((s8)cl > 0x14) {
        goto call_4d8610;
    }
    ecx = P32(eax + 0x61u);
    if ((s16)P16(ecx + 0xbu) <= 0x109a) {
        goto accept_call;
    }

call_4d8610:
    ecx = P32(0x00ae1050u);
    cm_fn_004d8610(ecx, esi);

accept_call:
    eax = cm_fn_00600a50(edi, 0u, 1u, 0u, 0u);
    if (eax == 0) {
        goto zero_path;
    }
    if ((ebx & 0x16288u) == 0) {
        goto exit_918;
    }
    edx = P32(edi + 0xcf);
    if (S8(edx + 0x59) <= 1) {
        goto exit_918;
    }
    eax = (u32)cm_fn_0090cfc0(5);
    if (eax != 0) {
        goto exit_918;
    }
    eax = P32(edi + 0xcf);
    P8(eax + 0x59) = (u8)(P8(eax + 0x59) - 1u);
    goto exit_918;

zero_path:
    eax = cm_fn_00600a50(edi, 1u, 1u, 0u, 0u);
    if (eax != 0) {
        goto exit_918;
    }
    if ((ebx & 0x16288u) == 0) {
        goto exit_918;
    }
    eax = P32(edi + 0xcf);
    if (eax == 0) {
        goto exit_918;
    }
    ecx = P32(eax + 0x69u);
    if (ecx == 0) {
        goto exit_918;
    }
    eax = (u32)S8(eax + 0x5d);
    eax = (u32)cm_fn_0090cfc0((s32)eax);
    if (eax == 0) {
        goto after_8f3;
    }
    ecx = (u32)S8(esi + 0x44);
    /* mov eax, 0x88888889; imul ecx: EDX:EAX = EAX * ECX (signed). */
    prod = (unsigned long long)((long long)(s32)0x88888889u * (long long)(s32)ecx);
    edx = (u32)(prod >> 32);
    edx = edx + ecx;
    edx = (u32)(((s32)edx) >> 3);
    ecx = edx >> 31;
    edx = edx + ecx + 0xau;
    ebp = (u32)cm_fn_0090cfc0((s32)edx);
    eax = P32(edi + 0xcf);
    ecx = P32(eax + 0x69u);
    edx = (u32)S8(ecx + 0x14);
    t3 = cm_fn_0090cfc0((s32)edx);
    eax = (u32)t3;
    t2 = (s32)ebp;
    if (t3 <= t2) {
        goto exit_918;
    }

after_8f3:
    eax = P32(esi);
    ecx = eax * 5u;
    eax = eax + ecx * 2u;
    edx = eax * 5u;
    eax = P32(0x00ae23bcu);
    ecx = eax + edx * 2u;
    edx = P32(edi + 0xcf);
    cm_fn_00547af0(edx, ecx, 1u);

exit_918:
    if ((ebx & 0x20001u) == 0) {
        goto after_968;
    }
    eax = P32(edi + 0xbfu);
    if (eax == 0) {
        goto after_968;
    }
    ecx = P32(eax + 0x69u);
    if (ecx == 0) {
        goto after_968;
    }
    edi = eax;
    r = (u32)cm_fn_0090cfc0(0x1e);
    ecx = P32(edi + 0x69u);
    eax = r;
    edx = (u32)S8(ecx + 0x16);
    if ((s32)edx <= (s32)eax) {
        goto after_968;
    }
    eax = P32(esi);
    ecx = eax * 5u;
    eax = eax + ecx * 2u;
    edx = eax * 5u;
    eax = P32(0x00ae23bcu);
    ecx = eax + edx * 2u;
    cm_fn_00547af0(edi, ecx, 1u);

after_968:
    eax = arg2;
    if (eax == 0) {
        return;
    }
    if ((ebx & 0xe0000000u) == 0) {
        return;
    }
    esi = P32(esi);
    ecx = P32(0x00ae23bcu);
    edx = esi * 5u;
    eax = esi + edx * 2u;
    eax = eax * 5u;
    edx = ecx + eax * 2u;
    cm_fn_00549cf0(edx, arg2, ebx);
}
