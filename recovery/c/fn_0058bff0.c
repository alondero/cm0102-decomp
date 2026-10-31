#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004a1c50(u32 self, volatile u32 save_arg);
extern void CM_THISCALL cm_fn_00687970(u8 *self, u8 *buffer);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
typedef u32 (CM_THISCALL *slot_one)(u32, u32);
typedef u32 (CM_THISCALL *slot_none)(u32);

u32 CM_THISCALL cm_fn_0058bff0(u32 self)
{
    u32 p, table, i = 0;
    s32 count;
    *(volatile u32 *)(self + 0x4c) = 0;
    cm_fn_00687970((u8 *)self, (u8 *)0);
    p = *(volatile u32 *)(self + 0xba);
    if (p) {
        cm_fn_009452ca(p);
        *(volatile u32 *)(self + 0xba) = 0;
    }
    p = *(volatile u32 *)(self + 8);
    if (p)
        cm_fn_004a1c50(p, 1);
    count = *(volatile s32 *)(self + 0x30);
    while ((s32)i <= count) {
        table = *(volatile u32 *)(self + 0xc);
        p = *(volatile u32 *)(table + i * 4);
        if (p) {
            table = *(volatile u32 *)p;
            ((slot_one)*(volatile u32 *)table)(p, 1);
        }
        count = *(volatile s32 *)(self + 0x30);
        ++i;
    }
    table = *(volatile u32 *)self;
    *(volatile u16 *)(self + 0x40) = (u16)(*(volatile u16 *)(self + 0x40) + 2);
    *(volatile u32 *)(self + 0x30) = 0xffffffff;
    *(volatile u32 *)(self + 0x45) = 0;
    return ((slot_none)*(volatile u32 *)(table + 0x8c))(self);
}
