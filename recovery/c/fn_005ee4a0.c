#include "cmre.h"

void CM_THISCALL cm_fn_005ee4a0(
    volatile u8 *self,
    volatile u32 *out_0c, volatile u32 *out_10, volatile u32 *out_14,
    volatile u32 *out_18, volatile u32 *out_1c, volatile u32 *out_20,
    volatile u32 *out_24, volatile u32 *out_38,
    volatile u16 *out_72, volatile u16 *out_74, volatile u32 *out_3c,
    volatile u16 *out_76, volatile u16 *out_78, volatile u32 *out_80,
    volatile u32 *out_40, volatile u32 *out_08, volatile u32 *out_48,
    volatile u16 *out_7a, volatile u32 *out_54)
{
    u32 value32;
    u16 value16;
    u32 index;
    u32 base;
    u32 offset;

    value32 = *(volatile u32 *)(self + 0x0c); *out_0c = value32;
    value32 = *(volatile u32 *)(self + 0x10); *out_10 = value32;
    value32 = *(volatile u32 *)(self + 0x14); *out_14 = value32;
    value32 = *(volatile u32 *)(self + 0x18); *out_18 = value32;
    value32 = *(volatile u32 *)(self + 0x1c); *out_1c = value32;
    value32 = *(volatile u32 *)(self + 0x20); *out_20 = value32;
    value32 = *(volatile u32 *)(self + 0x24); *out_24 = value32;
    value32 = *(volatile u32 *)(self + 0x38); *out_38 = value32;
    value16 = *(volatile u16 *)(self + 0x72); *out_72 = value16;
    value16 = *(volatile u16 *)(self + 0x74); *out_74 = value16;
    value32 = *(volatile u32 *)(self + 0x3c); *out_3c = value32;
    value16 = *(volatile u16 *)(self + 0x76); *out_76 = value16;
    value16 = *(volatile u16 *)(self + 0x78); *out_78 = value16;
    *out_80 = (u32)(self + 0x80);
    value32 = *(volatile u32 *)(self + 0x40); *out_40 = value32;
    value32 = *(volatile u32 *)(self + 0x08); *out_08 = value32;
    value32 = *(volatile u32 *)(self + 0x48); *out_48 = value32;
    value16 = *(volatile u16 *)(self + 0x7a); *out_7a = value16;
    value32 = *(volatile u32 *)(self + 0x54); *out_54 = value32;

    value16 = *out_7a;
    if (value16 != 0xffffu) {
        index = (u32)(s32)(s16)value16;
        offset = index * 3u;
        offset <<= 6;
        offset -= index;
        offset <<= 4;
        offset += index;
        base = *(volatile u32 *)self;
        if (*(volatile u16 *)(base + offset + 0x20eu) != 0xffffu)
            *out_80 = (u32)(self + 0x85);
    }
}
