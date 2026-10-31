#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* All ten indexed callers overwrite full EAX after cleanup. */
void CM_CDECL cm_fn_005a0650(volatile s8 first, volatile u32 second,
                           volatile u32 third, volatile s8 fourth)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 value, other, text;
    if (cm_fn_007fe600(0x00b74450, 0x005a0840, 0x005a2780, 0, 0, 0) == 0)
        return;
    value = second;
    if (value == 0) {
        cm_fn_00944cff(0x009c9eb4, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      local + 0x100, local, 0x7a);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = value;
    }
    cm_fn_007ff270(0x00b74450, 0, (u32)(s32)first, 0);
    cm_fn_007ff270(0x00b74450, 1, value, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, 0, 0);
    cm_fn_007ff270(0x00b74450, 4, value, 0);
    cm_fn_007ff270(0x00b74450, 5, 0, 0);
    cm_fn_007ff270(0x00b74450, 6, 0, 0);
    cm_fn_007ff270(0x00b74450, 7, value, 0);
    cm_fn_007ff270(0x00b74450, 8, 0, 0);
    cm_fn_007ff270(0x00b74450, 9, 0, 0);
    other = third;
    cm_fn_007ff270(0x00b74450, 0xa, other, 0);
    cm_fn_007ff270(0x00b74450, 0xb, 0, 0);
    cm_fn_007ff270(0x00b74450, 0xc, 0, 0);
    cm_fn_007ff270(0x00b74450, 0xd, other, 0);
    cm_fn_007ff270(0x00b74450, 0xe, 0, 0);
    cm_fn_007ff270(0x00b74450, 0xf, 0, 0);
    cm_fn_007ff270(0x00b74450, 0x10, value, 0);
    cm_fn_007ff270(0x00b74450, 0x11, 0, 0);
    cm_fn_007ff270(0x00b74450, 0x12, 0, 0);
    cm_fn_007ff270(0x00b74450, 0x13, (u32)(s32)fourth, 0);
}
