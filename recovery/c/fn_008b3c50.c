#include "cmre.h"
/* Preferred-image VA 0x008b3c50, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_008b3c50(u32 self)
{
    volatile u8 scratch[0x200];
    u32 count = D(0x00ae236c);
    u32 pointer = cm_fn_00945501(count, 6);
    u32 text, result, player, index, id;
    s8 age;
    D(self + 0x651du) = pointer;
    if (pointer == 0) {
        cm_fn_00944cff(0x00a8ccd0, 0, 0,
                       (u32)(scratch + 0x100), (u32)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x6e3);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    count = D(0x00ae236c);
    player = D(0x00ae23c4);
    index = 0;
    if ((s32)count > 0) {
        do {
            id = D(player);
            pointer = D(self + 0x651du);
            B(pointer + id * 6u + 5u) &= 0xdfu;
            age = (s8)B(player + 0xfu);
            pointer = D(self + 0x651du);
            id = D(player);
            B(pointer + id * 6u) = age < 15 ? 1 : 6;
            count = D(0x00ae236c);
            ++index;
            player += 0x46u;
        } while ((s32)index < (s32)count);
    }
    return player;
}
