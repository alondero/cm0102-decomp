#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_STDCALL cm_fn_004c6280(u32 output)
{
    u32 total = D(0x00ae235c);
    u8 scratch[0x204];
    u32 allocation, count, club, scanned, nation, target, value, row, high, shifted, competition, text, result, sort_count;
    long long product;
    count = 0;
    allocation = (u32)cm_fn_00944e46(total * 12u);
    D(output) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009a7ed8, 0, 0, (u32)scratch + 0x104, (u32)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x104, (u32)scratch + 4, 0xbb);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result & 0xffff0000u;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    D((u32)scratch) = 0;
    if ((s32)total > 0) {
        do {
            nation = D(club + 0x53);
            if (nation == 0 || cm_fn_0053b900(club) != 0)
                goto next;
            nation = D(club + 0x53);
            target = D(0x009cf3d0);
            value = D(nation);
            if (value != target) {
                nation = D(nation + 0x71);
                if (nation == 0)
                    goto next;
                value = D(nation);
                target = D(0x009cfa1c);
                if (value != target)
                    goto next;
            }
            D(club + 0x1db) = 0xffffffffu;
            row = allocation + (u32)(s32)(s16)count * 12u;
            D(row) = club;
            nation = D(club + 0x53);
            value = D(nation);
            D(row + 4) = value;
            /* Original signed IMUL high half, SAR by five,
               then addition of its sign bit. Keep the literal operations. */
            product = (long long)(s32)(s16)W(club + 0x80) * 0x10624dd3ll;
            high = (u32)((unsigned long long)product >> 32);
            shifted = (high >> 5) | ((high & 0x80000000u) != 0 ? 0xf8000000u : 0);
            value = shifted + (shifted >> 31);
            B(row + 8) = (u8)value;
            competition = D(club + 0x5b);
            if (competition != 0) {
                value = W(competition + 0x69);
                W(row + 9) = (u16)value;
                value = B(club + 0x5f);
                B(row + 0xb) = (u8)value;
                competition = D(club + 0x5b);
                value = D(competition + 0x5d);
                if (value == 0 || B(value + 0x11c) == 0) {
                    D(club + 0x5b) = 0;
                    B(club + 0x5f) = 0;
                }
            } else {
                W(row + 9) = 0xffff;
                B(row + 0xb) = 0x7f;
            }
            ++count;
next:
            scanned = D((u32)scratch);
            club += 0x245;
            ++scanned;
            D((u32)scratch) = scanned;
            total = D(0x00ae235c);
        } while ((s32)(s16)scanned < (s32)total);
    }
    sort_count = (u32)(s32)(s16)count;
    result = cm_fn_00945333(allocation, sort_count, 12, 0x00905ce0);
    /* 00945342/345 exits for unsigned count<2 without writing EAX.
       At the original call EAX is the last AE235C load (6317 or 63fc).
       Other sort paths overwrite full EAX before any use. */
    if (sort_count < 2u)
        result = total;
    return (result & 0xffff0000u) | (u16)count;
}
