#include "cmre.h"

/* Preferred-image VA 0x0068a1c0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Cdecl, seven stack arguments. Diagnostics do not prevent the original
   final writes, and the full EAX result is the computed record address. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void field_error(u32 name, u32 ext, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x009f15f4, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_CDECL cm_fn_0068a1c0(u32 base, u32 outer, u32 inner, u32 first,
                           u32 second, u32 third, u32 value)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, row;
    s16 selected = (s16)inner;
    s16 checked;
    s8 byte_checked, saved_third;
    if (selected < 0 || selected >= 8)
        field_error(local + 0x100, local, 0x16e2);
    checked = (s16)first;
    if (checked < -1 || checked > 6)
        field_error(local, local + 0x100, 0x16e6);
    byte_checked = (s8)second;
    if (byte_checked < -1 || byte_checked >= 3)
        field_error(local, local + 0x100, 0x16ea);
    saved_third = (s8)third;
    if (saved_third < -1 || saved_third > 4)
        field_error(local, local + 0x100, 0x16ee);
    row = base + (u32)(s32)(s16)outer * 65u + (u32)(s32)selected * 7u;
    B(row + 9) = (u8)first;
    B(row + 10) = (u8)second;
    B(row + 11) = (u8)saved_third;
    D(row + 5) = value;
    return row;
}
