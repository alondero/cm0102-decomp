#include "cmre.h"
/* Preferred-image VA 0x0086c1d0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full EAX: diagnostic return, zero query result, or final list call.
   Fourth stack word is unused, but belongs to the original RET 0x10. */
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_STDCALL cm_fn_0076d940(u32);
extern u32 CM_THISCALL cm_fn_00774f90(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086c1d0(u32 self, u32 club, u32 other,
                             u32 value, u32 unused)
{
    volatile struct { u32 prev, handler, state; } frame;
    volatile u8 scratch[0x440];
    u32 local = (u32)scratch;
    u32 predecessor = CM_FS(u32, 0);
    u32 id, player, base, row, receiver, first, second, line, text, result;
    (void)unused;
    frame.state = 0xffffffffu;
    frame.handler = 0x00962cbb;
    frame.prev = predecessor;
    CM_FS(u32, 0) = (u32)&frame;
    if (club == 0) {
        first = local + 0x240u;
        second = local + 0x40u;
        line = 0x768;
        goto error;
    }
    cm_fn_0054a110(local);
    frame.state = 0;
    cm_fn_00417c10(local, local + 0x38u, 0x00ae2c90);
    id = D(club);
    B(local + 0xcu) = 7;
    D(local + 8u) = id;
    if (other != 0) id = D(other);
    else id = 0xffffffffu;
    D(local + 0xdu) = id;
    value = (u32)(s32)(s16)(u16)value;
    player = D(club + 0xcfu);
    D(local + 0x11u) = value;
    if (player != 0) {
        if (local != 0) {
            id = D(player);
            base = D(self + 0xcu);
            row = base + id * 8u;
            receiver = D(self);
            cm_fn_0058e340(receiver, row, row + 4u, local);
        } else {
            first = local + 0x340u;
            second = local + 0x140u;
            line = 0x1010;
error:
            cm_fn_00944cff(0x00a83b78, 0, 0, first, second);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
            result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            if (club == 0) goto done;
        }
    }
    receiver = D(0x00b74318);
    /* The original loads ECX here, but 0076d940 neither reads nor forwards it. */
    result = cm_fn_0076d940(other);
    if (result != 0) {
        receiver = D(0x00b74318);
        result = cm_fn_00774f90(receiver, club, other);
    }
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
