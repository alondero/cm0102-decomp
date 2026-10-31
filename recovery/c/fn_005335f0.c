#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);

volatile u8 *CM_THISCALL cm_fn_005335f0(volatile u8 *self,
                                        const volatile u8 *source)
{
    volatile u32 scratch[2];
    u32 value32;
    u16 value16;
    u8 value8;

    value32 = *(const volatile u32 *)(source + 0x00);
    *(volatile u32 *)(self + 0x00) = value32;
    value32 = *(const volatile u32 *)(source + 0x04);
    *(volatile u32 *)(self + 0x04) = value32;
    value32 = *(const volatile u32 *)(source + 0x08);
    *(volatile u32 *)(self + 0x08) = value32;
    value32 = *(const volatile u32 *)(source + 0x0c);
    *(volatile u32 *)(self + 0x0c) = value32;
    cm_fn_00417c10((u32 *)(self + 0x10), (u32 *)scratch,
                   (u16 *)(source + 0x10));
    value16 = *(const volatile u16 *)(source + 0x18);
    *(volatile u16 *)(self + 0x18) = value16;
    value32 = *(const volatile u32 *)(source + 0x1a);
    *(volatile u32 *)(self + 0x1a) = value32;
    value32 = *(const volatile u32 *)(source + 0x1e);
    *(volatile u32 *)(self + 0x1e) = value32;
    value8 = source[0x22];
    self[0x22] = value8;
    value8 = source[0x23];
    self[0x23] = value8;
    value32 = *(const volatile u32 *)(source + 0x24);
    *(volatile u32 *)(self + 0x24) = value32;
    value8 = source[0x28];
    self[0x28] = value8;
    cm_fn_00417c10((u32 *)(self + 0x29), (u32 *)scratch,
                   (u16 *)(source + 0x29));
    cm_fn_00417c10((u32 *)(self + 0x31), (u32 *)scratch,
                   (u16 *)(source + 0x31));
    value32 = *(const volatile u32 *)(source + 0x39);
    *(volatile u32 *)(self + 0x39) = value32;
    value8 = source[0x3d];
    self[0x3d] = value8;
    cm_fn_00417c10((u32 *)(self + 0x3e), (u32 *)scratch,
                   (u16 *)(source + 0x3e));
    cm_fn_00417c10((u32 *)(self + 0x46), (u32 *)scratch,
                   (u16 *)(source + 0x46));
    value32 = *(const volatile u32 *)(source + 0x4e);
    *(volatile u32 *)(self + 0x4e) = value32;
    value32 = *(const volatile u32 *)(source + 0x52);
    *(volatile u32 *)(self + 0x52) = value32;
    value8 = source[0x56]; self[0x56] = value8;
    value8 = source[0x57]; self[0x57] = value8;
    value8 = source[0x58]; self[0x58] = value8;
    value8 = source[0x59]; self[0x59] = value8;
    value8 = source[0x5a]; self[0x5a] = value8;
    value8 = source[0x5b]; self[0x5b] = value8;
    value8 = source[0x5c]; self[0x5c] = value8;
    value8 = source[0x5d]; self[0x5d] = value8;
    value8 = source[0x5e]; self[0x5e] = value8;
    value8 = source[0x5f]; self[0x5f] = value8;
    value8 = source[0x60]; self[0x60] = value8;
    value32 = *(const volatile u32 *)(source + 0x61);
    *(volatile u32 *)(self + 0x61) = value32;
    value32 = *(const volatile u32 *)(source + 0x65);
    *(volatile u32 *)(self + 0x65) = value32;
    value32 = *(const volatile u32 *)(source + 0x69);
    *(volatile u32 *)(self + 0x69) = value32;
    self[0x6d] = source[0x6d];
    return self;
}
