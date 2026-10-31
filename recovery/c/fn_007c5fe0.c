#include "cmre.h"
/* Preferred-image VA 0x007c5fe0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern s32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern void CM_THISCALL cm_fn_0058e340(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0058f120(u32, u32, u32);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern s8 CM_CDECL cm_fn_00669340(u32);
extern u16 CM_CDECL cm_fn_00669950(u32);
extern void CM_FASTCALL cm_fn_0068d040(u32);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_007c5fe0(u32 self, u32 person, u32 field,
                              s8 flag3, s8 flag4, s32 filter, u32 flag6)
{
    /* L is ESP immediately after the original sub esp,0x23c.  The packed
       record begins at L+0xc; its 32-byte payload starts at L+0x12. */
    u8 scratch[0x23c];
    u32 local = (u32)scratch;
    u32 club, pointer, slots, index, row, offset, source, destination, n;
    u32 receiver, right, left, history, item, date, base;
    u16 year, value;
    u8 flags, byte;
    s8 country;
    char *version;

    D(local + 0x38u) = 0;
    B(local + 2u) = 0;
    if (flag3 != 0) B(local + 2u) = 2;
    else if (flag4 != 0) B(local + 2u) = 1;
    else if (flag6 != 0) B(local + 2u) = 3;
    if (person == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x13c, scratch + 0x3c);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x13c, scratch + 0x3c, 0xf28);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    club = D(person + 0x39u);
    pointer = D(person + 0x61u);
    D(local + 8u) = club;
    if (pointer == 0) return 0;
    index = D(pointer);
    source = D(self + 0x30u);
    slots = source + index * 39u;

    /* Six listing blocks differ only in slot and the upper five flag bits.
       Reload each slot, the payload base, and the receiver at its own site. */
    for (row = 0; row < 6; ++row) {
        index = D(slots + row * 4u);
        if (index == 0xffffffffu) continue;
        value = cm_fn_00669950(club);
        W(local + 0xcu) = value;
        if (club != 0) D(local + 0xeu) = D(club);
        else D(local + 0xeu) = 0xffffffffu;
        flags = B(local + 0x36u);
        flags = (u8)((flags & 7u) | ((row + 1u) << 3));
        offset = index << 5;
        if ((row & 1u) == 0) {
            source = D(self + 4u) + offset;
            B(local + 0x36u) = flags;
        } else {
            B(local + 0x36u) = flags;
            source = D(self + 4u) + offset;
        }
        if (row == 0) {
            byte = B(local + 2u);
            for (n = 0; n < 8; ++n) {
                pointer = D(source + n * 4u);
                D(local + 0x12u + n * 4u) = pointer;
            }
            D(local + 0x32u) = field;
            flags = B(local + 0x36u);
            flags = (u8)(flags ^ ((flags ^ byte) & 7u));
        } else {
            for (n = 0; n < 8; ++n) {
                pointer = D(source + n * 4u);
                D(local + 0x12u + n * 4u) = pointer;
            }
            flags = B(local + 0x36u);
            D(local + 0x32u) = field;
            byte = B(local + 2u);
            flags = (u8)(flags ^ ((flags ^ byte) & 7u));
        }
        B(local + 0x36u) = flags;
        receiver = D(self);
        cm_fn_0058e340(receiver, slots + 0x18u, slots + 0x1cu, local + 0xcu);
        destination = D(self + 4u) + offset;
        if (row == 5) D(local + 0x38u) = 1;
        for (n = 0; n < 8; ++n) D(destination + n * 4u) = 0;
        club = D(local + 8u);
    }

    B(local + 3u) = 0;
    if (club == 0) goto outside_dates;
    pointer = D(club + 0x53u);
    if (pointer == 0) goto outside_dates;
    country = cm_fn_00669340(pointer);
    if (country < 0 || country >= 35) goto outside_dates;
    date = D(0x00ae2c90);
    year = W(0x00ae2c92);
    base = (u32)(s32)country * 72u;
    if ((s16)date > (s16)W(0x00b63d8au + base)) {
        if (year == W(0x00b63d8cu + base)) goto after_start;
    }
    if (year <= W(0x00b63d8cu + base)) goto dates_done;
after_start:
    if ((s16)date < (s16)W(0x00b63d99u + base)) {
        if (year == W(0x00b63d9bu + base)) goto outside_dates;
    }
    if (year >= W(0x00b63d9bu + base)) goto dates_done;
outside_dates:
    B(local + 3u) = 1;
dates_done:
    if (D(local + 0x38u) == 0) {
        if ((s32)field > -1 || flag4 != 0) goto add_empty;
        if (filter <= -1) goto field_test;
        if (B(local + 3u) == 0) goto add_empty;
        right = D(person + 0x42u);
        left = D(person + 0x3eu);
        if (cm_fn_0054cdb0(0x00ae2c90, left, right) >= 0x16d)
            goto after_records;
add_empty:
        W(local + 0xcu) = cm_fn_00669950(club);
        if (club != 0) D(local + 0xeu) = D(club);
        else D(local + 0xeu) = 0xffffffffu;
        flags = B(local + 0x36u);
        byte = B(local + 2u);
        flags = (u8)((flags & 7u) | 0x30u);
        D(local + 0x32u) = field;
        flags = (u8)(flags ^ ((flags ^ byte) & 7u));
        B(local + 0x36u) = flags;
        for (n = 0; n < 8; ++n) D(local + 0x12u + n * 4u) = 0;
        receiver = D(self);
        cm_fn_0058e340(receiver, slots + 0x18u, slots + 0x1cu, local + 0xcu);
    }
after_records:
    if (filter > -1) goto aggregate;
field_test:
    if ((s32)field <= -1) return 1;
aggregate:
    right = D(slots + 0x1cu);
    W(slots + 0x20u) = 0;
    W(slots + 0x22u) = 0;
    left = D(slots + 0x18u);
    receiver = D(self);
    history = cm_fn_0058f120(receiver, left, right);
    if (history != 0) {
        pointer = D(history + 2u);
        D(history + 0xau) = pointer;
        item = cm_fn_0068d1b0(history);
        while (item != 0) {
            pointer = D(item + 2u);
            if (pointer == (u32)filter) {
                byte = B(item + 0x2au);
                if ((byte & 0xf8u) == 0x10u) {
                    value = B(item + 6u);
                    W(slots + 0x20u) = (u16)(W(slots + 0x20u) + value);
                    value = B(item + 8u);
                    W(slots + 0x22u) = (u16)(W(slots + 0x22u) + value);
                }
            }
            item = cm_fn_0068d1b0(history);
        }
        cm_fn_0068d040(history);
        cm_fn_00944c94((void *)history);
    }
    return 1;
}
