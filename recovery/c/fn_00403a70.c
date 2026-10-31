#include "cmre.h"

/* Preferred-image VA 0x00403a70, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

static void area_error(u32 name, u32 ext, u32 line)
{
    u32 text;
    cm_fn_00944cff(0x009873c0, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_00403a70(u32 self, u32 x, u32 y)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 left, top, right, bottom, dx, dy;
    left = D(self);
    top = D(self + 4);
    dx = x - left;
    dy = y - top;
    if ((s32)(left + dx) < 0) {
        area_error(local + 0x100, local, 0x337);
        dx = 0u - D(self);
    }
    right = D(self + 8);
    if ((s32)(right + dx) > 799) {
        area_error(local, local + 0x100, 0x33c);
        dx = 799u - D(self + 8);
    }
    top = D(self + 4);
    if ((s32)(top + dy) < 0) {
        area_error(local, local + 0x100, 0x341);
        dy = 0u - D(self + 4);
    }
    bottom = D(self + 0xc);
    if ((s32)(bottom + dy) > 599) {
        area_error(local, local + 0x100, 0x346);
        bottom = D(self + 0xc);
        dy = 599u - bottom;
    }
    if (dx != 0 || dy != 0) {
        left = D(self);
        top = D(self + 4);
        right = D(self + 8);
        left += dx;
        top += dy;
        right += dx;
        bottom += dy;
        D(self) = left;
        D(self + 4) = top;
        D(self + 8) = right;
        D(self + 0xc) = bottom;
    }
    return bottom;
}
