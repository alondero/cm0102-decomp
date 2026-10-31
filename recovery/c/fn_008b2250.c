#include "cmre.h"
/* Preferred-image VA 0x008b2250, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller 008b0bb5 discards EAX: after ADD ESI,5 / DEC EDI,
   either MOV EAX,[ESI] at 008b0ba4 or MOV EAX,[EBX+806] at 008b0bc0
   overwrites it. This includes the original qsort's untouched EAX return
   for counts below two. The swapped row is skipped by the original loop. */
extern void CM_THISCALL cm_fn_008b1750(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008b39f0(u32, u32);
extern u32 CM_THISCALL cm_fn_008b4f60(u32);
extern void CM_THISCALL cm_fn_0062c660(u32, u32, u32);
extern void CM_THISCALL cm_fn_008b42d0(u32, u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

void CM_THISCALL cm_fn_008b2250(u32 self, u32 player, u32 argument)
{
    volatile u8 scratch[0x200];
    u32 profile, id, base, row, value, receiver, count, old_count, index;
    u32 text;
    u8 kind = (u8)argument;
    if (player != 0) {
        profile = D(player + 0x61u);
        if (profile != 0) {
            id = D(profile);
            base = D(self + 0x651du);
            row = base + id * 6u;
            if (kind != B(row)) {
                cm_fn_008b1750(self, player, 0);
                B(row) = kind;
            }
            value = cm_fn_008b39f0(self, player);
            value = cm_fn_008b4f60(value);
            receiver = D(0x00b63d28);
            cm_fn_0062c660(receiver, player, value);
            cm_fn_008b42d0(self, player);
            count = D(self + 0x6525u);
            old_count = count;
            index = 0;
            if ((s32)count > 0) {
                do {
                    base = D(self + 0x6521u);
                    row = base + index * 8u;
                    if (B(row + 7u) == 0) {
                        value = D(base + count * 8u - 8u);
                        D(row) = value;
                        value = D(base + count * 8u - 4u);
                        D(row + 4u) = value;
                        count = D(self + 0x6525u);
                        --count;
                        D(self + 0x6525u) = count;
                    }
                    count = D(self + 0x6525u);
                    ++index;
                } while ((s32)index < (s32)count);
            }
            count = D(self + 0x6525u);
            if (old_count != count) {
                base = D(self + 0x6521u);
                cm_fn_00945333(base, count, 8, 0x008b4e00);
            }
            return;
        }
    }
    cm_fn_00944cff(0x00a8ccd0, 0, 0,
                   (u32)(scratch + 0x100), (u32)scratch);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                   (u32)(scratch + 0x100), (u32)scratch, 0x2eb);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}
