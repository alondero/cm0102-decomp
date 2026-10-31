#include "cmre.h"
/* Preferred-image VA 0x00687430, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Normal exits retain the final sign-extended team-count load in full EAX.
   Keep the duplicate auxiliary byte store and every overlapping pointer reload. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00687430(u32 self, u8 index, u32 club,
                             u8 flag, u32 base)
{
    volatile u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 row, eax, text, position, allocated;
    u32 counter, distance, outer, inner, rows, pointer, value;
    s32 limit;
    u16 count, factor;
    u8 type;
    if (club != 0 && B(club + 0x64u) == 3) B(club + 0x64u) = 2;
    if (W(self + 0xa7u) == 0xffffu) {
        count = W(self + 0x3eu);
        if ((s16)count > 0) {
            if ((s16)count > 50) {
                cm_fn_00944cff(0x009f15f4, 0, 0, local + 0x108u, local + 8u);
                text = cm_fn_0090d130();
                cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                               local + 0x108u, local + 8u, 0xfe9);
                cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                D(0x00b67a34) = 0;
            }
            count = W(self + 0x3eu);
            count = (u16)(count - 1u);
            factor = W(self + 0x3cu);
            W(self + 0xa7u) = (u16)((u32)count * factor);
        }
    }
    if (base == 0) base = D(self + 0xb1u);
    row = base + (u32)(s32)(s8)index * 59u;
    D(row) = club;
    B(row + 4u) = index;
    position = row + 0x11u;
    counter = 3;
    do {
        D(position - 4u) = 0;
        B(position) = 0;
        B(position + 1u) = 0;
        B(position + 2u) = 0;
        B(position + 3u) = 0;
        W(position + 4u) = 0;
        W(position + 6u) = 0;
        W(position + 8u) = 0;
        position += 14u;
    } while (--counter != 0);
    B(row + 0x38u) = flag;
    B(row + 0x37u) = 0xff;
    B(row + 0x39u) = 1;
    type = B(self + 0x3eu);
    B(row + 0x3au) = type;
    D(row + 5u) = 0;
    if ((s16)W(self + 0x3cu) > 0) {
        eax = (u32)(s32)(s16)W(self + 0xa7u);
        allocated = (u32)cm_fn_00944e46(eax);
        D(row + 9u) = allocated;
        if (allocated == 0) {
            cm_fn_00944cff(0x009f15f4, 0, 0, local + 8u, local + 0x108u);
            text = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                           local + 8u, local + 0x108u, 0x100c);
            eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            return eax;
        }
    } else {
        D(row + 9u) = 0;
    }
    position = self + 0xc5u;
    for (;;) {
        type = B(position);
        if (type == 0) break;
        if (type == 4) {
            eax = (u32)(s32)(s16)W(self + 0x3eu);
            allocated = (u32)cm_fn_00944e46(eax * 14u - 14u);
            D(row + 5u) = allocated;
            limit = (s32)(s16)W(self + 0x3eu) - 1;
            counter = 0;
            if (limit > 0) {
                distance = 0;
                do {
                    pointer = D(row + 5u);
                    ++counter;
                    B(distance + pointer + 4u) = 0;
                    B(distance + D(row + 5u) + 5u) = 0;
                    B(distance + D(row + 5u) + 4u) = 0;
                    B(distance + D(row + 5u) + 6u) = 0;
                    B(distance + D(row + 5u) + 7u) = 0;
                    W(distance + D(row + 5u) + 8u) = 0;
                    W(distance + D(row + 5u) + 10u) = 0;
                    W(distance + D(row + 5u) + 12u) = 0;
                    distance += 14u;
                    limit = (s32)(s16)W(self + 0x3eu) - 1;
                } while ((s32)counter < limit);
            }
            break;
        }
        ++position;
        if ((s32)(position + (0xffffff3bu - self)) >= 4) break;
    }
    eax = (u32)(s32)(s16)W(self + 0x3eu);
    if ((u32)(s32)(s8)index == eax - 1u && D(row + 5u) != 0) {
        outer = 0;
        D(local + 4u) = 0;
        if ((s32)eax > 0) {
            pointer = base + 5u;
            D(local) = pointer;
            do {
                inner = 0;
                if ((s32)eax > 0) {
                    rows = base;
                    distance = 0;
                    do {
                        if (outer != inner) {
                            pointer = D(local);
                            value = D(rows);
                            distance += 14u;
                            pointer = D(pointer);
                            D(distance + pointer - 14u) = value;
                            outer = D(local + 4u);
                        }
                        limit = (s32)(s16)W(self + 0x3eu);
                        ++inner;
                        rows += 59u;
                    } while ((s32)inner < limit);
                }
                eax = (u32)(s32)(s16)W(self + 0x3eu);
                pointer = D(local);
                ++outer;
                pointer += 59u;
                D(local + 4u) = outer;
                D(local) = pointer;
            } while ((s32)outer < (s32)eax);
        }
    }
    return eax;
}
