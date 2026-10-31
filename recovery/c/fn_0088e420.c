/* Preferred-image VA 0088e420, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller VA0088cdc7 replaces full EAX at0088cdcc. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_CDECL cm_fn_00945333(u32 table, u32 count, u32 size, u32 comparator);
extern void CM_THISCALL cm_fn_004a2540(u32 self, u32 club, u32 raw_rank);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_0088e420(u32 self)
{
    u8 work[0x218];
    u32 pointer, index = 1, count, table, receiver, club, slot, remaining, raw_rank, version;
    s32 group;
    u8 rank;
    B((u32)work + 3u) = 1;
    B((u32)work + 4u) = 1;
    receiver = D(self + 8u);
    B((u32)work + 5u) = 2;
    B((u32)work + 6u) = 4;
    B((u32)work + 7u) = 8;
    B((u32)work + 8u) = 16;
    B((u32)work + 9u) = 32;
    if (receiver == 0) {
        cm_fn_00944cff(0x00a8b4a0u, 0, 0, work + 0x118, work + 0x18);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x118, work + 0x18, 0x408u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return;
    }
    count = (u32)(s32)(s16)W(self + 0xb6u);
    table = D(self + 0xb2u);
    cm_fn_00945333(table, count, 4, 0x004b07d0u);
    D((u32)work + 12u) = (u32)work + 9u;
    D((u32)work + 16u) = 6;
    do {
        pointer = D((u32)work + 12u);
        rank = B((u32)work + 3u);
        B((u32)work + 20u) = rank;
        group = (s8)B(pointer);
        raw_rank = D((u32)work + 20u);
        if (group > 0) {
            B((u32)work + 3u) = (u8)(rank + (u8)group);
            do {
                table = D(self + 0xb2u);
                slot = (u32)(s32)(s16)index;
                ++index;
                club = D(table + slot * 4u);
                receiver = D(self + 8u);
                cm_fn_004a2540(receiver, club, raw_rank);
                --group;
            } while (group != 0);
        }
        pointer = D((u32)work + 12u);
        remaining = D((u32)work + 16u);
        --pointer;
        --remaining;
        D((u32)work + 12u) = pointer;
        D((u32)work + 16u) = remaining;
    } while (remaining != 0);
    if ((s16)index < (s16)W(self + 0xb6u)) {
        do {
            table = D(self + 0xb2u);
            slot = (u32)(s32)(s16)index;
            club = D(table + slot * 4u);
            receiver = D(self + 8u);
            cm_fn_004a2540(receiver, club, raw_rank);
            ++index;
        } while ((s16)index < (s16)W(self + 0xb6u));
    }
}
