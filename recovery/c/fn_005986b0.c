#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);

/* Object fields by byte offset from self (ECX): 0x00 low dword and 0x04 high
   dword of a signed 64-bit value, 0x08 index. Record at
   global(0x00ae23b4) + index * 0x245, with a 16-bit field at +0x80. */
#define SELF_U32(off) (*(volatile u32 *)(self + (off)))

/* Signed 64-bit comparison of (hi:lo) with the 32-bit value t sign-extended,
   as the original cmp/jl/jg/jbe chain does: high dword signed, low dword
   unsigned. Returns -1, 0 or 1 for less, equal, greater. */
static s32 cm_cmp_s64_with_s32(s32 hi, u32 lo, u32 t)
{
    s32 t_hi = ((s32)t < 0) ? -1 : 0;

    if (hi < t_hi) {
        return -1;
    }
    if (hi > t_hi) {
        return 1;
    }
    if (lo < t) {
        return -1;
    }
    if (lo > t) {
        return 1;
    }
    return 0;
}

u8 CM_THISCALL cm_fn_005986b0(u32 self)
{
    u32 global_base;
    u32 index;
    u32 edx_value;
    u32 ecx_value;
    u32 record;
    u16 field80;
    u32 c;
    u32 lo;
    s32 hi;
    u32 t;

    global_base = CM_GLOBAL(volatile u32, 0xae23b4);
    index = SELF_U32(0x08);
    edx_value = global_base + index;
    ecx_value = ((index * 9u) << 4) + index;
    record = edx_value + ecx_value * 4u;

    if (cm_fn_0053b900((u32 *)record) != 0u) {
        return 0u;
    }

    field80 = *(volatile u16 *)(record + 0x80u);
    lo = SELF_U32(0x00);
    hi = (s32)SELF_U32(0x04);
    c = (u32)(s32)(s16)field80;

    t = (c * 2500u) + 5000000u;
    if (cm_cmp_s64_with_s32(hi, lo, t) > 0) {
        return 2u;
    }

    t = (c * 1000u) + 3500000u;
    if (cm_cmp_s64_with_s32(hi, lo, t) > 0) {
        return 1u;
    }

    t = 0xffd9da60u - (c * 1500u);
    if (cm_cmp_s64_with_s32(hi, lo, t) < 0) {
        return 0xfeu;
    }

    if ((s16)field80 >= (s16)5000) {
        t = (3750u - c) * 1000u;
        if (cm_cmp_s64_with_s32(hi, lo, t) < 0) {
            return 0xffu;
        }
        return 0u;
    }

    t = 0u - (c * 250u);
    if (cm_cmp_s64_with_s32(hi, lo, t) < 0) {
        return 0xffu;
    }
    return 0u;
}
