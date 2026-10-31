#include "cmre.h"
/* Preferred-image VA 0x0086f5b0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   A mismatched tag resets the old-profile slot using the cached record base,
   then restarts selection; that slot can differ from the selected new slot.
   Keep the original 210-byte reset and its possible repeated restart. */
extern unsigned long long CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00549f70(u32, u32, u32, u32, u32);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086f5b0(u32 self, u32 player, u32 mode)
{
    volatile u8 scratch[0x210];
    u32 local = (u32)scratch;
    u32 count, id, base, profile, index, record, tag, reset, offset, text;
    u32 club, pointer, value, previous, date0, date1, date, cached;
    u16 year, checkpoint_year;
    s32 elapsed;
    if (player == 0) {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x110u, local + 0x10u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x110u, local + 0x10u, 0xf6e);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    for (;;) {
        count = D(0x00ae2364);
        id = D(player);
        if ((s32)id >= (s32)(count - 16u) && (s32)id < (s32)count) {
            u8 flag = B(player + 0x5fu);
            if (flag == 0 || flag == 5) return 0;
            base = D(self + 8u);
            index = id - count;
            count = D(0x00ae2368);
            index += count + 16u;
            record = base + index * 210u;
        } else {
            profile = D(player + 0x69u);
            if (profile == 0) return 0;
            index = D(profile);
            base = D(self + 8u);
            record = base + index * 210u;
        }
        tag = D(record + 0x86u);
        if (tag == 0xffffffffu) {
            if (mode == 0) return 0;
            id = D(player);
            D(record + 0x86u) = id;
            break;
        }
        if (tag == id) break;
        profile = D(player + 0x69u);
        index = D(profile);
        reset = base + index * 210u;
        for (offset = 0; offset < 0xd0u; offset += 4u) D(reset + offset) = 0;
        W(reset + 0xd0u) = 0;
        D(reset + 0x86u) = 0xffffffffu;
        D(reset + 0xc2u) = 0xffffffffu;
        D(reset + 0xc6u) = 0xffffffffu;
        D(reset + 0x4au) = 0xffffffffu;
        D(reset + 0x5eu) = 0xffffffffu;
        D(reset + 0x72u) = 0xffffffffu;
    }
    club = D(player + 0x39u);
    if (club != 0) {
        pointer = D(club + 0x53u);
        if (pointer != 0) {
            value = D(pointer);
            previous = D(record + 0xc2u);
            if (value != previous) {
                D(record + 0xc2u) = value;
                D(record + 0xbau) = 0;
            }
        }
    }
    club = D(player + 0x24u);
    if (club != 0) {
        pointer = D(club + 0x53u);
        if (pointer != 0) {
            value = D(pointer);
            previous = D(record + 0xc6u);
            if (value != previous) {
                D(record + 0xc6u) = value;
                D(record + 0xbeu) = 0;
            }
        }
    }
    date0 = D(0x00dd6ec8);
    date1 = D(0x00dd6ecc);
    year = W(0x00ae2c92);
    D(local + 8u) = date0;
    D(local + 0xcu) = date1;
    checkpoint_year = W(local + 0xau);
    if (checkpoint_year > year) return record;
    if (checkpoint_year == year && (s16)(u16)date0 > (s16)W(0x00ae2c90))
        return record;
    if (D(record + 0x4au) == 0xffffffffu) {
        club = D(player + 0x39u);
        if (club != 0) {
            value = D(club);
            D(record + 0x4au) = value;
            cm_fn_00417c10(record + 0x4eu, local + 8u, player + 0x3eu);
            date = cm_fn_00549f70(local + 8u, 13, 11, 0x7b7, 0xffffffffu);
            cm_fn_00417c10(record + 0x56u, local, date);
        }
    }
    value = D(record + 0x5eu);
    if (value == 0xffffffffu) {
        value = D(record + 0x4au);
        D(record + 0x5eu) = value;
        cm_fn_00417c10(record + 0x62u, local, record + 0x4eu);
        cm_fn_00417c10(record + 0x6au, local, 0x00ae2c90);
    } else if (D(record + 0x4au) != 0xffffffffu) {
        date1 = D(record + 0x66u);
        date0 = D(record + 0x62u);
        elapsed = cm_fn_0054cdb0(record + 0x6au, date0, date1);
        date1 = D(record + 0x52u);
        date0 = D(record + 0x4eu);
        if (cm_fn_0054cdb0(0x00ae2c90, date0, date1) > elapsed) {
            value = D(record + 0x4au);
            D(record + 0x5eu) = value;
            cm_fn_00417c10(record + 0x62u, local, record + 0x4eu);
            cm_fn_00417c10(record + 0x6au, local, 0x00ae2c90);
        }
    }
    value = D(record + 0x72u);
    if (value == 0xffffffffu) {
        value = D(record + 0x4au);
        D(record + 0x72u) = value;
        cm_fn_00417c10(record + 0x76u, local, record + 0x4eu);
        cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
    } else {
        cached = D(record + 0x4au);
        if (value == cached) {
            date0 = D(record + 0x4eu);
            date1 = D(record + 0x52u);
            D(local + 8u) = date0;
            D(local + 0xcu) = date1;
            year = W(local + 0xau);
            if (year == W(record + 0x78u) && (u16)date0 == W(record + 0x76u)) {
                cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
                return record;
            }
        }
        if (cached != 0xffffffffu) {
            date1 = D(record + 0x7au);
            date0 = D(record + 0x76u);
            elapsed = cm_fn_0054cdb0(record + 0x7eu, date0, date1);
            date1 = D(record + 0x52u);
            date0 = D(record + 0x4eu);
            if (cm_fn_0054cdb0(0x00ae2c90, date0, date1) < elapsed) {
                value = D(record + 0x4au);
                D(record + 0x72u) = value;
                cm_fn_00417c10(record + 0x76u, local, record + 0x4eu);
                cm_fn_00417c10(record + 0x7eu, local, 0x00ae2c90);
            }
        }
    }
    return record;
}
