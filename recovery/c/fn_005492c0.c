#include "cmre.h"
/* Preferred-image VA 0x005492c0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Division precedes the floating comparison. The sole caller sign-extends AL. */
extern u32 CM_THISCALL cm_fn_004a4850(u32, u32);
extern u32 CM_THISCALL cm_fn_0062c400(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u8 CM_CDECL cm_fn_005492c0(u32 club)
{
    u32 pointer, index, base, receiver, value, quotient, player, limit, cursor;
    u8 count = 0, divisor;
    s32 threshold;
    if (!club) return 0;
    pointer = D(club + 0x57);
    if (pointer) {
        index = D(pointer);
        base = D(0x00adadfc);
        pointer = D(base + index * 4u);
        if (pointer) {
            receiver = D(pointer + 8);
            if (receiver) {
                pointer = cm_fn_004a4850(receiver, club);
                if (pointer) {
                    divisor = B(pointer + 4);
                    if (divisor) {
                        value = B(pointer + 0x69);
                        quotient = value / divisor;
                        if (!((long double)(s32)quotient >=
                            (long double)*(volatile double *)0x00968940)) return 0;
                    }
                }
            }
        }
    }
    threshold = (s16)W(club + 0x80) / 65 + 40;
    if (threshold < 40) threshold = 40;
    else if (threshold > 150) threshold = 150;
    cursor = club + 0xd7;
    for (index = 0; index < 50; ++index) {
        player = D(cursor);
        if (!player) goto next;
        pointer = D(player + 0x61);
        if (!pointer) goto next;
        if (B(pointer + 0x15) != 20) goto next;
        receiver = D(0x00b63d28);
        if ((u8)cm_fn_0062c400(receiver, player) == 2) goto next;
        value = D(player);
        limit = D(0x00ae2364);
        if ((s32)value >= (s32)limit) goto next;
        pointer = D(player + 0x61);
        if ((s16)W(pointer + 5) > threshold) ++count;
next:
        cursor += 4;
    }
    return (s8)count > 1;
}
