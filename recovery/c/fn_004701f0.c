#include "cmre.h"

/* Preferred-image VA 0x004701f0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both indexed direct callers overwrite EAX after stack cleanup. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

void CM_CDECL cm_fn_004701f0(volatile u32 first, volatile u32 second, volatile u32 third)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, value, other, options, text, flag;
    u8 mode;
    value = first;
    if (value == 0) goto error;
    mode = *(volatile u8 *)&second;
    other = third;
    if (mode == 11) {
        if (other == 0) goto error;
        flag = 0;
    } else flag = 1;
    if (cm_fn_007fe600(0x00b74450, 0x00470400, 0x00470fe0, flag, 0x004718f0, 0) == 0) return;
    options = 0;
    if (mode == 10) {
        options = cm_fn_00944cf1(0xd1);
        cm_fn_0066f4e0(options, 0x00990cb4);
        B(options + 0xc8) = 2;
        B(options + 0xc9) = 3;
        B(options + 0xca) = 1;
        W(options + 0xcb) = 0;
        W(options + 0xcd) = 0;
        W(options + 0xcf) = 4;
    }
    value = first;
    cm_fn_007ff270(0x00b74450, 0, value, 0);
    cm_fn_007ff270(0x00b74450, 1, 1, 0);
    cm_fn_007ff270(0x00b74450, 2, 0, 0);
    cm_fn_007ff270(0x00b74450, 3, 0, 0);
    cm_fn_007ff270(0x00b74450, 4, (u32)(s32)(s8)mode, 0);
    cm_fn_007ff270(0x00b74450, 5, 0, 0);
    cm_fn_007ff270(0x00b74450, 6, 0, 0);
    cm_fn_007ff270(0x00b74450, 7, 20, 0);
    cm_fn_007ff270(0x00b74450, 8, other, 0);
    cm_fn_007ff270(0x00b74450, 9, options, 0);
    cm_fn_007ff270(0x00b74450, 10, options, 0);
    return;
error:
    cm_fn_00944cff(0x0098e598, 0, 0, local + 0x100, local);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x2fe1);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}
