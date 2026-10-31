#include "cmre.h"
typedef u32 (CM_THISCALL *slot_fn)(u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_00766db0(u32 self)
{
    u32 table = *(volatile u32 *)self;
    u32 target, value;
    *(volatile u16 *)(self + 0xdb) = 0;
    *(volatile u8 *)(self + 0xbe) = 0;
    *(volatile u8 *)(self + 0xbf) = 0;
    *(volatile u8 *)(self + 0xc0) = 0;
    *(volatile u8 *)(self + 0xc1) = 0;
    *(volatile u8 *)(self + 0x49) = 9;
    *(volatile u8 *)(self + 0x4a) = 9;
    *(volatile u16 *)(self + 0x3c) = 1;
    target = *(volatile u32 *)(table + 0x3c);
    value = ((slot_fn)target)(self, 0xffffffff, self + 0xa9, self + 0x3a, 0);
    *(volatile u32 *)(self + 0xba) = value;
    return 1;
}
