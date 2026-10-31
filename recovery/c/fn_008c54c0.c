#include "cmre.h"

extern s32 CM_THISCALL cm_fn_004cf5a0(s32 *self, s32 *row);
extern s32 CM_THISCALL cm_fn_004cf5f0(s32 *self, s32 *left, s32 *right);

u16 CM_STDCALL cm_fn_008c54c0(u32 *input)
{
    u32 index = *(volatile u32 *)((u8 *)input + 0x0cu);
    u32 receiver = CM_GLOBAL(volatile u32, 0x00ae1050u);
    u32 row_base = CM_GLOBAL(volatile u32, 0x00ae23bcu);
    u32 row = row_base + index * 0x6eu;
    s32 found = cm_fn_004cf5a0((s32 *)receiver, (s32 *)row);
    CM_GLOBAL(volatile u32, 0x00ae1080u) = (u32)found;

    u32 candidate = 0;
    if (found != 0) {
        u32 slot = *(volatile u32 *)((u32)found + 4u);
        u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4u);
        candidate = base + slot * 0x245u;
    }

    receiver = CM_GLOBAL(volatile u32, 0x00ae1050u);
    s32 result = cm_fn_004cf5f0((s32 *)receiver, (s32 *)row, (s32 *)candidate);
    if (result == 0)
        return 0;

    u8 raw = *(volatile u8 *)((u32)result + 0x4fu);
    u32 selector = (u32)(s32)(s8)(raw >> 4) - 1u;
    if (selector > 4u)
        return 0;

    volatile u32 target = *(volatile u32 *)(0x008c554cu + selector * 4u);
    (void)target;
    switch (selector) {
    case 0: return 0x0200u;
    case 1:
    case 2: return 0x0400u;
    case 3: return 0;
    case 4: return 0x2000u;
    }
    return 0;
}
