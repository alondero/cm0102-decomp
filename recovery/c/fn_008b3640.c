#include "cmre.h"
/* Preferred-image VA 0x008b3640, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep the original 008b2be0 EAX at this boundary, including untouched bits. */
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

u32 CM_THISCALL cm_fn_008b3640(u32 self, u32 player, u32 restore)
{
    volatile u8 scratch[0x208];
    u32 profile, row, id, pointer, text, result;
    u8 a, b;
    if (player != 0) {
        profile = D(player + 0x61u);
        if (profile != 0) {
            id = D(profile);
            pointer = D(self + 0x651du);
            row = pointer + id * 6u;
            if (restore != 0) {
                a = B(0x00de1f3f);
                b = B(0x00de1f40);
                scratch[0] = a;
                a = B(0x00de1f3d);
                scratch[1] = b;
                b = B(0x00de1f3c);
                scratch[2] = a;
                a = B(0x00de1f3e);
                scratch[3] = b;
            } else {
                B(0x00de1f3f) = (u8)low_nibble(B(row + 3u));
                B(0x00de1f40) = (u8)high_nibble(B(row + 3u));
                B(0x00de1f3d) = (u8)low_nibble(B(row + 4u));
                B(0x00de1f3c) = (u8)high_nibble(B(row + 4u));
                B(0x00de1f3e) = (u8)low_nibble(B(row + 5u));
                scratch[0] = (u8)-low_nibble(B(row + 3u));
                scratch[1] = (u8)-high_nibble(B(row + 3u));
                scratch[2] = (u8)-low_nibble(B(row + 4u));
                scratch[3] = (u8)-high_nibble(B(row + 4u));
                a = (u8)-low_nibble(B(row + 5u));
            }
            scratch[4] = a;
            return cm_fn_008b2be0(self, player, (u32)scratch);
        }
    }
    cm_fn_00944cff(0x00a8ccd0, 0, 0,
                   (u32)(scratch + 0x108), (u32)(scratch + 8));
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   (u32)(scratch + 0x108), (u32)(scratch + 8), 0x621);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
