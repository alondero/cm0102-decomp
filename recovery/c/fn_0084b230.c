#include "cmre.h"

s32 CM_STDCALL cm_fn_0084b230(u32 arg1, u32 arg2, s8 mode)
{
    volatile u8 *object = (volatile u8 *)arg1;
    s32 selector = (s32)mode - 1;
    s32 value;
    u32 type;

    (void)arg2;
    if ((u32)selector > 5u || selector < 3)
        return -1;
    if (selector == 4) {
        value = *(volatile s16 *)(object + 0x80);
        return (s32)((u32)value * 0x226u);
    }
    if (selector == 5) {
        value = *(volatile s16 *)(object + 0x80);
        return (s32)((u32)value * 0x6eu);
    }

    {
        u32 pointer = *(volatile u32 *)(object + 0x53);
        if (pointer != 0) {
            type = *(volatile u32 *)(u8 *)pointer;
            if (type == (u32)CM_GLOBAL(volatile s32, 0x009cf360) ||
                type == (u32)CM_GLOBAL(volatile s32, 0x009cf2e4) ||
                type == (u32)CM_GLOBAL(volatile s32, 0x009cf488)) {
                value = *(volatile s16 *)(object + 0x80);
                return (s32)((u32)value * 0x226u);
            }
            type = *(volatile u32 *)(u8 *)pointer;
            if (type == (u32)CM_GLOBAL(volatile s32, 0x009cf314) ||
                type == (u32)CM_GLOBAL(volatile s32, 0x009cf300)) {
                value = *(volatile s16 *)(object + 0x80);
                return (s32)((u32)value * 0x1dbu);
            }
        }
    }

    value = *(volatile s16 *)(object + 0x80);
    return (s32)((u32)value * 0x14au);
}
