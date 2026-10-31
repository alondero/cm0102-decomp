#include "cmre.h"

/* Preferred-image VA 0x005baa50, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Six ordered selection passes; the first pass has no capacity guard. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

/* Every field write reloads the array pointer, including possible overlap. */
#define APPEND(offset, club) do { \
    u32 destination = D(self + 0xa7); \
    ++selected; \
    D(destination + (offset)) = (club); \
    destination = D(self + 0xa7); \
    B(destination + (offset) + 4) = 0; \
    destination = D(self + 0xa7); \
    B(destination + (offset) + 5) = 0; \
} while (0)

u32 CM_THISCALL cm_fn_005baa50(u32 self)
{
    u8 scratch[0x204];
    u32 allocation, count, club, offset, value, other, limit;
    u32 index = 0, selected = 0;

    W(self + 0x3e) = 0xde;
    allocation = (u32)cm_fn_00944e46(0x534);
    D(self + 0xa7) = allocation;
    if (!allocation) {
        u32 extension = (u32)scratch + 4;
        u32 filename = (u32)scratch + 0x104;
        cm_fn_00944cff(0x009ccb28, 0, 0, (void *)filename, (void *)extension);
        value = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, filename, extension, 0x16f);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }

    count = D(0x00ae235c);
    club = D(0x00ae23b4);
    if ((s32)count > 0) {
        offset = 0;
        do {
            value = D(club + 0x57);
            if (value) {
                value = D(value);
                other = D(0x009cf5d4);
                if (value == other) {
                    APPEND(offset, club);
                    count = D(0x00ae235c);
                    offset += 6;
                }
            }
            ++index;
            club += 0x245;
        } while ((s32)(s16)index < (s32)count);
    }

    club = D(0x00ae23b4);
    index = 0;
    if ((s32)count > 0) {
        offset = selected * 6u;
        do {
            value = D(club + 0x57);
            if (!value) goto next_second;
            other = D(0x009cf764);
            value = D(value);
            if (value != other) goto next_second;
            value = D(club + 0x60);
            if (!value) goto next_second;
            value = D(value);
            other = D(0x009cf5d8);
            if (value != other) goto next_second;
            value = W(self + 0x40);
            limit = (u16)(value - 0x7d1u) ? 0xb6u : 0xb8u;
            if ((s32)selected >= (s32)limit) goto next_second;
            APPEND(offset, club);
            offset += 6;
            value = (u32)(s32)(s16)W(self + 0x3e);
            count = D(0x00ae235c);
            if (selected == value) break;
next_second:
            club += 0x245;
            ++index;
        } while ((s32)(s16)index < (s32)count);
    }

    club = D(0x00ae23b4);
    index = 0;
    if ((s32)count > 0) {
        offset = selected * 6u;
        do {
            value = D(club + 0x53);
            if (!value) goto next_third;
            value = D(value);
            other = D(0x009cf300);
            if (value != other) goto next_third;
            value = D(club + 0x57);
            if (!value) goto next_third;
            other = D(0x009cf764);
            value = D(value);
            if (value != other) goto next_third;
            value = D(club + 0x60);
            if (value) goto next_third;
            value = W(self + 0x40);
            limit = (u16)(value - 0x7d1u) ? 0xb6u : 0xb8u;
            if ((s32)selected >= (s32)limit) goto next_third;
            APPEND(offset, club);
            offset += 6;
            value = (u32)(s32)(s16)W(self + 0x3e);
            count = D(0x00ae235c);
            if (selected == value) break;
next_third:
            club += 0x245;
            ++index;
        } while ((s32)(s16)index < (s32)count);
    }

    value = D(0x00ae2350);
    club = D(0x00ae23b4);
    index = 0;
    limit = count - value * 2u;
    if ((s32)limit > 0) {
        offset = selected * 6u;
        do {
            value = D(club + 0x53);
            if (!value) goto next_fourth;
            value = cm_fn_0053b900((u32 *)club);
            if (value) goto reload_fourth;
            value = D(club + 0x57);
            if (value) goto reload_fourth;
            other = D(club + 0x53);
            value = D(0x009cf300);
            count = D(other);
            if (count != value) goto reload_fourth;
            value = W(self + 0x40);
            limit = (u16)(value - 0x7d1u) ? 0xb6u : 0xb8u;
            if ((s32)selected >= (s32)limit) goto reload_fourth;
            APPEND(offset, club);
            offset += 6;
            value = (u32)(s32)(s16)W(self + 0x3e);
            if (selected == value) {
                count = D(0x00ae235c);
                break;
            }
reload_fourth:
            count = D(0x00ae235c);
next_fourth:
            value = D(0x00ae2350);
            ++index;
            club += 0x245;
            limit = count - value * 2u;
        } while ((s32)(s16)index < (s32)limit);
    }

    club = D(0x00ae23b4);
    index = 0;
    if ((s32)count > 0) {
        offset = selected * 6u;
        do {
            value = D(club + 0x57);
            if (!value) goto next_fifth;
            value = D(value);
            other = D(0x009cf5d0);
            if (value != other) goto next_fifth;
            value = (u32)(s32)(s16)W(self + 0x3e);
            if ((s32)selected >= (s32)value) goto next_fifth;
            APPEND(offset, club);
            offset += 6;
            value = (u32)(s32)(s16)W(self + 0x3e);
            count = D(0x00ae235c);
            if (selected == value) break;
next_fifth:
            ++index;
            club += 0x245;
        } while ((s32)(s16)index < (s32)count);
    }

    club = D(0x00ae23b4);
    index = 0;
    if ((s32)count > 0) {
        offset = selected * 6u;
        do {
            value = D(club + 0x57);
            if (!value) goto next_sixth;
            value = D(value);
            other = D(0x009cf5cc);
            if (value != other) goto next_sixth;
            value = (u32)(s32)(s16)W(self + 0x3e);
            if ((s32)selected >= (s32)value) goto next_sixth;
            APPEND(offset, club);
            offset += 6;
            value = (u32)(s32)(s16)W(self + 0x3e);
            if (selected == value) break;
            count = D(0x00ae235c);
next_sixth:
            ++index;
            club += 0x245;
        } while ((s32)(s16)index < (s32)count);
    }
    value = (u32)(s32)(s16)W(self + 0x3e);
    return selected == value;
}
