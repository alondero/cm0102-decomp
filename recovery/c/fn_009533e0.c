#include "cmre.h"

extern s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags, const char *source, s32 count, void *destination, s32 capacity, u32 code_page, s32 option);

u32 CM_CDECL cm_fn_009533e0(u32 value)
{
    if (value <= 0xffu) {
        u8 flags = *(volatile u8 *)(0x00de4521u + value);
        if ((flags & 0x20u) != 0)
            return *(volatile u8 *)(0x00de4420u + value);
    } else {
        u8 source[2], destination[2];
        source[0] = (u8)(value >> 8);
        source[1] = (u8)value;
        if ((*(volatile u8 *)(0x00de4521u + source[0]) & 4u) != 0 &&
            cm_fn_0094c0fe(CM_GLOBAL(volatile u32, 0x00de4624u), 0x200u,
                           (const char *)source, 2, destination, 2,
                           CM_GLOBAL(volatile u32, 0x00de4408u), 1) != 0)
            return ((u32)destination[0] << 8) + destination[1];
    }
    return value;
}
