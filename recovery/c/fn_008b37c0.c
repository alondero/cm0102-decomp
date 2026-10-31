#include "cmre.h"
/* Preferred-image VA 0x008b37c0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   008b38a6 replaces only AL; 008b386a replaces only BL. Preserve both
   registers' high bytes in the final full-width subtraction. */
extern u8 CM_CDECL cm_fn_005431e0(u32, u32);
extern u32 CM_THISCALL cm_fn_008b2be0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

static s32 low_nibble(u8 value)
{
    return (s32)(value & 15u) - ((value & 8u) ? 16 : 0);
}

static s32 high_nibble(u8 value)
{
    return (s32)(value >> 4) - ((value & 0x80u) ? 16 : 0);
}

u32 CM_THISCALL cm_fn_008b37c0(u32 self, u32 player, u32 argument)
{
    volatile u8 scratch[0x208];
    u32 profile, id, pointer, row, text, result, saved_bx;
    u8 first, second, a, b, c, d, e;
    if (player != 0) {
        profile = D(player + 0x61u);
        if (profile != 0) {
            id = D(profile);
            pointer = D(self + 0x651du);
            row = pointer + id * 6u;
            first = cm_fn_005431e0(player, argument);
            a = B(row + 3u);
            scratch[7] = first;
            scratch[0] = (u8)-low_nibble(a);
            scratch[1] = (u8)-high_nibble(B(row + 3u));
            scratch[2] = (u8)-low_nibble(B(row + 4u));
            scratch[3] = (u8)-high_nibble(B(row + 4u));
            a = B(row + 5u);
            scratch[4] = (u8)-low_nibble(a);
            cm_fn_008b2be0(self, player, (u32)scratch);
            second = cm_fn_005431e0(player, argument);
            a = scratch[0];
            c = scratch[2];
            saved_bx = (argument & 0xffffff00u) | second;
            b = scratch[1];
            a = (u8)(0u - a);
            b = (u8)(0u - b);
            c = (u8)(0u - c);
            scratch[0] = a;
            d = scratch[3];
            scratch[1] = b;
            e = scratch[4];
            scratch[2] = c;
            d = (u8)(0u - d);
            e = (u8)(0u - e);
            scratch[3] = d;
            scratch[4] = e;
            result = cm_fn_008b2be0(self, player, (u32)scratch);
            a = scratch[7];
            return ((result & 0xffffff00u) | a) - saved_bx;
        }
    }
    cm_fn_00944cff(0x00a8ccd0, 0, 0,
                   (u32)(scratch + 0x108), (u32)(scratch + 8));
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   (u32)(scratch + 0x108), (u32)(scratch + 8), 0x657);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result & 0xffffff00u;
}
