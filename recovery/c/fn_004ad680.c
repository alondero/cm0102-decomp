/* Preferred-image VA 004ad680, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_CDECL cm_fn_00945333(u32 base, u32 count, u32 size, u32 comparator);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 pointer);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_004ad680(u32 self)
{
    u8 work[0x208];
    u32 table, total, old, allocation, bytes, position, i, offset = 0, seen = 0, changed = 0;
    u32 competition, value, player, id, second_id, previous, counter, version;
    u16 tally;
    u8 limit;
    table = D(0x00ae23d4u);
    total = D(0x00ae237cu);
    old = D(self + 0x27u);
    D((u32)work + 4u) = old;
    cm_fn_00945333(table, total, 26, 0x00540780u);
    total = D(0x00ae235cu);
    allocation = (u32)cm_fn_00944cf1(total * 2u);
    if (allocation == 0) {
        cm_fn_00944cff(0x0099ca94u, 0, 0, work + 0x108, work + 8);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x19dbu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    bytes = D(0x00ae235cu) * 2u;
    position = allocation;
    for (i = bytes >> 2; i != 0; --i) {
        D(position) = 0;
        position += 4u;
    }
    for (i = bytes & 3u; i != 0; --i) {
        B(position) = 0;
        position += 1u;
    }
    total = D(0x00ae237cu);
    D((u32)work) = 0;
    if ((s32)total > 0) {
        do {
            table = D(0x00ae23d4u);
            competition = D(self + 0x502u);
            value = D(table + offset + 4u);
            if (value == D(competition + 4u)) {
                table = D(0x00ae23d4u);
                player = D(table + offset + 10u);
                if (player != 0) {
                    id = D(player);
                    tally = W(allocation + id * 2u);
                    W(allocation + id * 2u) = (u16)(tally + 1u);
                    second_id = D(player);
                    tally = W(allocation + second_id * 2u);
                    limit = B(self + 0x26u);
                    if (limit == 0xffu || (s16)tally > (s16)(s8)limit) {
                        B(self + 0x26u) = (u8)tally;
                        id = D(player);
                        D(self + 0x27u) = id;
                        previous = D((u32)work + 4u);
                        id = D(player);
                        changed = id != previous;
                    }
                }
                seen = 1;
            } else if (seen != 0) {
                break;
            }
            counter = D((u32)work);
            total = D(0x00ae237cu);
            counter += 1u;
            offset += 26u;
            D((u32)work) = counter;
        } while ((s32)counter < (s32)total);
    }
    cm_fn_00944c94(allocation);
    return changed;
}
