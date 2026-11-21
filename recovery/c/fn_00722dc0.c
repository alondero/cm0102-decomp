#include "cmre.h"

extern void cm_fn_00721cd0(int, u32);
extern u32 CM_THISCALL cm_fn_007ff010(int, int);
extern void CM_THISCALL cm_fn_007ff270(int, int, int, int);

u32 cm_fn_00722dc0(short arg)
{
    u32 first = cm_fn_007ff010(0xb74450, 0);
    s8 value = (s8)cm_fn_007ff010(0xb74450, 0x1c);
    u32 offset;
    u32 base;
    u16 kind;

    if (CM_GLOBAL(volatile u32, 0xb6b7ac) != 0 &&
        (*(volatile u8 *)CM_GLOBAL(volatile u32, first + 0x5b) & 4) != 0 &&
        CM_GLOBAL(volatile u32, 0xb6b7b0) == 0) {
        cm_fn_00721cd0(0, 0xffffffff);
        CM_GLOBAL(volatile u32, 0xb6b7a8) = 1;
    }
    if (arg != -1) {
        offset = (u32)(s32)arg * 0x18c;
        base = CM_GLOBAL(volatile u32, 0xb74c4c + (u32)(s32)CM_GLOBAL(volatile s16, 0xb77c7a) * 0x300);
        kind = (u16)CM_GLOBAL(volatile u32, base + offset + 0xba966);
        if (!kind)
            kind = CM_GLOBAL(volatile u16, 0xdd6bde);
        if (kind == 9) {
            cm_fn_007ff270(0xb74450, 0x1c, (int)value - 1, 0);
            CM_GLOBAL(volatile u32, 0xb6b7b0) = 1;
        }
        base = CM_GLOBAL(volatile u32, 0xb74c4c + (u32)(s32)CM_GLOBAL(volatile s16, 0xb77c7a) * 0x300);
        kind = (u16)CM_GLOBAL(volatile u32, base + offset + 0xba966);
        if (!kind)
            kind = CM_GLOBAL(volatile u16, 0xdd6bde);
        if (kind == 10) {
            cm_fn_007ff270(0xb74450, 0x1c, (int)value + 1, 0);
            CM_GLOBAL(volatile u32, 0xb6b7b0) = 1;
        }
    }
    return 0;
}
