/* Preferred-image VA0048f150/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full0-or1. The original caches its target in EBP, then uses
   the actual argument slot's low BYTE for the signed outer selector.
   Keep that overwrite/reload distinct from the cached pointer. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007e6f20(u32 receiver, u32 club);
extern unsigned long long CM_THISCALL cm_fn_005c6710(u32 receiver, u32 club);
extern u32 CM_THISCALL cm_fn_004a4850(u32 receiver, u32 target);

u32 CM_THISCALL cm_fn_0048f150(u32 self, volatile u32 target_arg)
{
    volatile u32 saved_self = self;
    u32 receiver, club, found, target, object, list, i, row;
    s16 count;
    u8 selector, kind, left, right;
    receiver = D(0x00b74434);
    if (receiver == 0) return 0;
    club = D(self + 4u);
    if (cm_fn_007e6f20(receiver, club) != 0) return 0;
    receiver = D(0x00ae2c74);
    if (receiver == 0) return 0;
    club = D(self + 4u);
    if ((u32)cm_fn_005c6710(receiver, club) != 0) return 0;
    if (B(self + 0x44u) != 0xff) return 1;
    receiver = D(self + 8u);
    target = target_arg;
    if (receiver != 0) {
        found = cm_fn_004a4850(receiver, target);
        if (found != 0) {
            left = B(found + 0x74u);
            right = B(found + 0x75u);
            if (left == right && left != 0) return 0;
        }
    }
    selector = B(self + 0x30u);
    *(volatile u8 *)&target_arg = selector;
    if ((s8)selector < -1) return 1;
    do {
        if (selector != 0xff) {
            list = D(self + 0xcu);
            object = D(list + (u32)(s32)(s8)selector * 4u);
        } else object = self;
        kind = B(object + 0x43u);
        if (kind == 1) {
            count = (s16)W(object + 0x3eu);
            i = 0;
            if (count > 0) {
                list = D(object + 0xa7u);
                do {
                    if (list != 0 && D(list + (u32)(s32)(s16)(u16)i * 6u) == target)
                        return 1;
                    ++i;
                } while ((s16)(u16)i < count);
            }
        } else if (kind == 2) {
            count = (s16)W(object + 0x3eu);
            i = 0;
            if (count > 0) {
                list = D(object + 0xb1u);
                do {
                    if (list != 0) {
                        if (D(list + (u32)(s32)(s16)(u16)i * 59u) == target) {
                            list = D(object + 0xb1u);
                            row = list + (u32)(s32)(s16)(u16)i * 59u;
                            kind = B(row + 0x37u);
                            if (kind == 0xfe || kind == 7 || kind == 6 || kind == 5) return 0;
                            left = B(row + 0x39u);
                            right = B(row + 0x3au);
                            return left != right;
                        }
                        selector = *(volatile u8 *)&target_arg;
                    }
                    ++i;
                } while ((s16)(u16)i < count);
            }
            self = saved_self;
        }
        selector = (u8)(selector - 1u);
        *(volatile u8 *)&target_arg = selector;
    } while ((s8)selector >= -1);
    return 1;
}
