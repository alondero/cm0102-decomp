#include "cmre.h"

extern void CM_CDECL cm_fn_00549c90(u32 input);
extern u8 CM_THISCALL cm_fn_007c6830(u32 self, u32 player, u32 club, u8 nation, u32 flag);

void CM_THISCALL cm_fn_00866ee0(u32 self, u32 player, u32 club)
{
    u32 index = *(volatile u32 *)self;
    u32 nested;
    *(volatile u8 *)(self + 0x44u) = 0;
    *(volatile u8 *)(self + 0x4eu) = 0;
    *(volatile u32 *)(self + 0x45u) = 0;
    cm_fn_00549c90(CM_GLOBAL(volatile u32, 0x00ae23bcu) + index * 110u);
    *(volatile u32 *)(self + 0x40u) = 0;
    *(volatile u8 *)(self + 0x4eu) = 4;
    *(volatile u8 *)(self + 0x3fu) = 0;
    *(volatile u8 *)(self + 0x3eu) = 0;
    if (club == 0) return;
    nested = *(volatile u32 *)(club + 0x53u);
    if (nested != 0) {
        u8 nation = *(volatile u8 *)(nested + 0x77u);
        s8 result = (s8)cm_fn_007c6830(CM_GLOBAL(volatile u32, 0x00b74350u), player, club, nation, 0);
        u8 value = *(volatile u8 *)(self + 0x4fu);
        u8 low = (u8)(result > 10 ? 10 : result);
        *(volatile u8 *)(self + 0x4fu) = (u8)(value ^ ((value ^ low) & 0x0fu));
    }
}
