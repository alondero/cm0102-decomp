#include "cmre.h"

extern s32 CM_CDECL cm_fn_00946aae(volatile u8 *file);
extern u32 CM_CDECL cm_fn_0094af9e(u32 *file);
extern unsigned long long CM_CDECL cm_fn_0094c552(u32 first, u32 second,
                                                   u32 mode);

s32 CM_CDECL cm_fn_00946a22(volatile u8 *file, u32 second, u32 mode)
{
    u32 flags = *(volatile u32 *)(file + 0x0c);
    if ((flags & 0x83u) == 0 || (mode != 0 && mode != 1 && mode != 2)) {
        CM_GLOBAL(volatile u32, 0x00de2f8cu) = 0x16u;
        return -1;
    }

    u32 stored_flags = (flags & 0xffffff00u) |
                       ((u32)(u8)flags & 0xefu);
    *(volatile u32 *)(file + 0x0c) = stored_flags;
    if (mode == 1) {
        s32 adjustment = cm_fn_00946aae(file);
        second += (u32)adjustment;
        mode = 0;
    }

    (void)cm_fn_0094af9e((u32 *)file);
    flags = *(volatile u32 *)(file + 0x0c);
    if ((flags & 0x80u) != 0) {
        *(volatile u32 *)(file + 0x0c) = flags & 0xfffffffcu;
    } else if ((flags & 1u) != 0 && (flags & 8u) != 0 &&
               (flags & 0x400u) == 0) {
        *(volatile u32 *)(file + 0x18) = 0x200u;
    }

    u32 first = *(volatile u32 *)(file + 0x10);
    u32 result = (u32)cm_fn_0094c552(first, second, mode);
    return result == 0xffffffffu ? -1 : 0;
}
