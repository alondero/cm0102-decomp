#include "cmre.h"

/* Preferred-image VAs, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   8228fb-82291c leaves the first zero pending across 7ff010 and 803320;
   822993-82299a passes one stack argument to 7ff010. The three 7ff270
   calls use ECX=b74450 and three stack arguments. The 803320 argument is
   pushed as EAX at 822909. 822a41-822a54 pass the full incoming ESI to
   the cdecl boundaries 7615d0 and 7643d0. These remain original calls;
   no filesystem, UI, or shutdown behavior is substituted. */
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern u32 CM_THISCALL cm_fn_00803320(u32, u32);
extern void CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_007615d0(u32);
extern u32 CM_CDECL cm_fn_007643d0(u32);

static u32 field(u32 selection, u32 offset)
{
    u32 group = (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a);
    u32 index = (u32)(s32)(s16)selection;
    u32 base = CM_GLOBAL(volatile u32, 0x00b74c4c + group * 768u);
    return CM_GLOBAL(volatile u32, base + offset + index * 396u);
}

u32 CM_CDECL cm_fn_008228f0(u32 selection)
{
    u32 value, current, result;
    if ((u16)selection != 0xffffu) {
        value = cm_fn_007ff010(0x00b74450, 2);
        value = cm_fn_00803320(0x00b74450, value);
        cm_fn_007ff270(0x00b74450, 1, value, 0);
        value = field(selection, 0xba966);
    } else {
        value = 0;
    }
    if ((u16)value == 0)
        value = (value & 0xffff0000u) | (u16)CM_GLOBAL(volatile u16, 0x00dd6bde);
    if ((u16)value == 0x1f) {
        current = (u16)selection == 0xffffu ? 0 : field(selection, 0xba9a6);
        result = cm_fn_007ff010(0x00b74450, 0);
        if (current != result) {
            value = (u16)selection == 0xffffu ? 0 : field(selection, 0xba9a6);
            cm_fn_007ff270(0x00b74450, 0, value, 0);
            cm_fn_007ff270(0x00b74450, 1, 0, 0);
            return 0xfffffff5u;
        }
    }
    value = (u16)selection == 0xffffu ? 0 : field(selection, 0xba966);
    if ((u16)value == 0)
        value = (value & 0xffff0000u) | (u16)CM_GLOBAL(volatile u16, 0x00dd6bde);
    if ((u16)value == 0xfffdu)
        return 0;
    result = cm_fn_007615d0(selection);
    if (!result)
        result = cm_fn_007643d0(selection);
    return result;
}
