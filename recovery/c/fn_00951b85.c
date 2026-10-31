#include "cmre.h"

extern s32 CM_CDECL cm_fn_0094dede(u8 *first, u8 *second, u32 count);

u32 CM_CDECL cm_fn_00951b85(u8 *name, u32 length)
{
    u32 cursor = CM_GLOBAL(volatile u32, 0x00de2fb4);
    u32 entry = *(volatile u32 *)cursor;
    while (entry) {
        if (cm_fn_0094dede(name, (u8 *)entry, length) == 0) {
            u8 next;
            entry = *(volatile u32 *)cursor;
            next = *(volatile u8 *)(entry + length);
            if (next == '=' || next == 0) {
                u32 base = CM_GLOBAL(volatile u32, 0x00de2fb4);
                return (u32)((s32)(cursor - base) >> 2);
            }
        }
        entry = *(volatile u32 *)(cursor + 4);
        cursor += 4;
    }
    return 0u - (u32)((s32)(cursor - CM_GLOBAL(volatile u32, 0x00de2fb4)) >> 2);
}
