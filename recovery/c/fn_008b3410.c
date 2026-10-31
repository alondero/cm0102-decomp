#include "cmre.h"
/* Preferred-image VA 0x008b3410, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00541f90(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_008b3410(u32 self, u32 club, u32 output)
{
    volatile u8 scratch[0x2d4];
    u32 local = (u32)scratch;
    u32 text, result, source, player, id, count, profile, other, receiver, pointer;
    D(local + 4u) = self;
    if (club == 0) {
        cm_fn_00944cff(0x00a8ccd0, 0, 0, local + 0xcu, local + 0x10cu);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0xcu, local + 0x10cu, 0x5c9);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    result = cm_fn_00540a50(club, 0, 1);
    D(local + 8u) = result;
    cm_fn_00541f90(club, local + 0x20cu, 0);
    source = local + 0x20cu;
    D(local) = 50;
    do {
        player = D(source);
        if (player == 0) goto empty;
        id = D(player);
        count = D(0x00ae2364);
        if ((s32)id >= (s32)count) goto empty;
        profile = D(player + 0x61u);
        if (profile == 0) {
            cm_fn_00944cff(0x00a8ccd0, 0, 0, local + 0x10cu, local + 0xcu);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           local + 0x10cu, local + 0xcu, 0x5df);
            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            goto empty;
        }
        other = D(player + 0x39u);
        if (other != club) {
            receiver = D(local + 8u);
            if (receiver == 0 || receiver != other) goto empty;
        }
        D(output) = player;
        profile = D(player + 0x61u);
        id = D(profile);
        receiver = D(local + 4u);
        pointer = D(receiver + 0x651du);
        B(output + 4u) = B(pointer + id * 6u);
        goto advance;
empty:
        D(output) = 0;
advance:
        count = D(local);
        source += 4u;
        output += 5u;
        --count;
        D(local) = count;
    } while (count != 0);
    return count;
}
