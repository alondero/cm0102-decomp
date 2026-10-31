#include "cmre.h"
/* Preferred-image VA00417c50/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/full EAX. Empty path writes only AX, leaving the upper half
   from the initial AE2350 load. SI comparisons do not narrow ESI itself. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern void CM_CDECL cm_fn_00945333(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00600a50(u32,u32,u8,u32,u32);

static __attribute__((always_inline)) inline void increment(u32 self, u32 *counter)
{
    u32 chosen, table, address;
    chosen = cm_fn_0090cfc0((u32)(s32)(s16)*counter);
    table = D(self);
    address = table + (u32)(s32)(s16)chosen * 14u + 12u;
    W(address) = (u16)(W(address) + 1u);
    *counter += 1u;
    if ((s16)*counter >= (s16)W(self + 4)) *counter = 0;
}

u32 CM_THISCALL cm_fn_00417c50(u32 self, volatile u32 output_arg,
                              volatile s16 limit_arg)
{
    u32 first, remaining, counter = 0, eax, table, club = 0, mode;
    u32 total, count, limit = 0, pointer, id, wanted, match, offset;
    u32 output, index, value, base;
    first = D(0x00ae2350);
    remaining = D(0x00ae235c);
    eax = (first & 0xffff0000u) | W(self + 4);
    remaining -= first * 2u;
    if ((u16)eax == 0) return eax;
    count = (u32)(s32)(s16)eax;
    table = D(self);
    cm_fn_00945333(table, count, 14, 0x004183f0);
    mode = B(self + 0x28);
    if (mode <= 2) {
        club = D(0x00ae23b4);
        if ((s32)remaining <= 0) goto sorted;
    } else if (mode == 3) {
        base = D(0x00ae23b4);
        total = D(0x00ae235c);
        club = base + remaining * 581u;
        first = D(0x00ae2350);
        limit = total - first;
        if ((s32)remaining >= (s32)limit) goto sorted;
    } else if (mode == 4) {
        if ((s32)remaining <= 0) goto sorted;
    } else if (mode == 5) {
        total = D(0x00ae235c);
        first = D(0x00ae2350);
        limit = total - first;
        if ((s32)remaining >= (s32)limit) goto sorted;
    } else if (mode == 6) {
        total = D(0x00ae235c);
        remaining = 0;
        if ((s32)total <= 0) goto sorted;
    } else goto sorted;
    for (;;) {
        match = mode >= 4;
        if (mode <= 3) {
            pointer = D(club + (mode == 0 ? 0x57u : 0x53u));
            if (pointer) {
                if (mode >= 2) pointer = D(pointer + 0x71);
                if (pointer) {
                    id = D(pointer);
                    wanted = D(self + (mode == 0 ? 0x29u : (mode == 1 ? 0x2du : 0x31u)));
                    if (id == wanted)
                        match = cm_fn_00600a50(club, 0, 1, 0, 0) == 0;
                }
            }
        }
        if (match) increment(self, &counter);
        if (mode <= 2) {
            club += 581u;
            remaining--;
            if (!remaining) break;
        } else if (mode == 3) {
            total = D(0x00ae235c);
            first = D(0x00ae2350);
            remaining++;
            limit = total - first;
            club += 581u;
            if ((s32)remaining >= (s32)limit) break;
        } else if (mode == 4) {
            remaining--;
            if (!remaining) break;
        } else if (mode == 5) {
            total = D(0x00ae235c);
            first = D(0x00ae2350);
            remaining++;
            if ((s32)remaining >= (s32)(total - first)) break;
        } else {
            total = D(0x00ae235c);
            remaining++;
            if ((s32)remaining >= (s32)total) break;
        }
    }
sorted:
    count = (u32)(s32)(s16)W(self + 4);
    table = D(self);
    cm_fn_00945333(table, count, 14, 0x00418420);
    limit = (u32)(s32)limit_arg;
    index = 0;
    eax = 0;
    if ((s32)limit <= 0) return eax;
    output = output_arg;
    offset = 0;
    for (;;) {
        count = (u32)(s32)(s16)W(self + 4);
        if ((s32)index >= (s32)count) break;
        table = D(self);
        offset += 14u;
        value = D(table + offset - 14u);
        base = D(0x00ae23bc);
        value = base + value * 110u;
        pointer = output + (u32)(s32)(s16)eax * 4u;
        eax++;
        index++;
        D(pointer) = value;
        limit = (u32)(s32)limit_arg;
        if ((s32)index >= (s32)limit) break;
    }
    return eax;
}
