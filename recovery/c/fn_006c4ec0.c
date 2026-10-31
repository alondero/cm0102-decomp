#include "cmre.h"

/* Preferred-image VA 0x006c4ec0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   006f46b0 consumes BYTEs in both argument slots, including its late reloads. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_006f0970(u32);
extern void CM_THISCALL cm_fn_006f46b0(u32, u8, u8);
extern void CM_FASTCALL cm_fn_006c3aa0(u32);
extern u8 CM_THISCALL cm_fn_00714d30(u32);
extern u32 CM_THISCALL cm_fn_006f5b80(u32, u32);
extern void CM_THISCALL cm_fn_007160e0(u32, u32);
extern u32 CM_THISCALL cm_fn_006f0630(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_006c4ec0(u32 self)
{
    u8 scratch[0x214];
    u32 local = (u32)scratch, target, side, row, base, table, slot, offset;
    u32 remaining, result, text;
    u8 mode, position, packed, rating, which, gap, first, second;
    mode = B(self + 0x8eae);
    if (mode != 0) {
        target = D(self + 0xf5a2);
        D(local + 4) = target;
        if (mode == 1) {
            B(self + 0x8ea7) = 4;
            B(self + 0x8ea8) = 11;
        }
    } else {
        target = D(self + 0xf59e);
        B(self + 0x8ea7) = 4;
        D(local + 4) = target;
        B(self + 0x8ea8) = 0;
    }
    target = D(self + 0xf582);
    if (target != 0) cm_fn_006f0970(target);
    D(local + 0x10) = (u32)(s32)(s8)mode;
    base = self + 0x4796;
    D(local + 8) = base;
    for (side = 0; side < 2; ++side) {
        slot = D(local + 0x10);
        D(local + 0xc) = 20;
        B(local + 3) = slot == side;
        row = base;
        do {
            position = B(row + 0x19);
            if ((s8)position >= 0) {
                slot = B(row + 0x27) == 1;
                table = D(self + 0xf56e + slot * 4u);
                offset = (u32)(s32)(s8)position * 27u;
                packed = B(table + offset * 8u + 0xa2);
                cm_fn_006f46b0(row, packed & 15u, packed >> 4);
                cm_fn_006c3aa0(self);
                target = D(self + 0xf582);
                rating = B(row + 0x103);
                if (row != target) {
                    which = cm_fn_00714d30(row);
                    if (which == 1) gap = (u8)(B(self + 0x8eb0) - rating);
                    else gap = (u8)(rating - B(self + 0x8eaf));
                    if ((s8)gap < 0) goto after_choice;
                }
                if (B(local + 3) != 0) cm_fn_006f5b80(row, 1);
after_choice:
                cm_fn_007160e0(row, 0);
            }
            remaining = D(local + 0xc);
            row += 0x1be;
            --remaining;
            D(local + 0xc) = remaining;
        } while (remaining != 0);
        base = D(local + 8);
        base += 0x22d8;
        D(local + 8) = base;
    }
    target = D(local + 4);
    if (target != 0) {
        second = B(self + 0x8ea8);
        first = B(self + 0x8ea7);
        B(self + 0x8ea9) = 0;
        cm_fn_006f46b0(target, first, second);
        result = cm_fn_006f0630(target);
        W(target + 0x19c) = 200;
        second = B(self + 0x8ea8);
        W(target + 0x19a) = second != 0 ? 180 : 0;
        return result;
    }
    cm_fn_00944cff(0x00a15884, 0, 0, local + 0x114, local + 0x14);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x114, local + 0x14, 0x2621);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    B(self) = 1;
    return result;
}
