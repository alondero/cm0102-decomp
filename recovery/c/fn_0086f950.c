#include "cmre.h"
/* Preferred-image VA 0x0086f950, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_0054cab0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0086f5b0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_0086f950(u32 self, u32 id)
{
    volatile u8 scratch[0x220];
    u32 local = (u32)scratch;
    u32 table, entry, text, offset, base, club, player, date, date0, date1;
    u32 receiver, result, value, best, index;
    u16 year, other_year;
    s16 count;
    D(local + 0xcu) = self;
    table = D(0x00adadfc);
    entry = D(table + id * 4u);
    D(local + 4u) = 0;
    D(local + 8u) = 0;
    if (B(entry + 0x43u) != 2) {
        cm_fn_00944cff(0x00a83b78, 0, 0, local + 0x120u, local + 0x20u);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x120u, local + 0x20u, 0x104e);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    count = (s16)W(entry + 0x3eu);
    D(local) = 0;
    if (count > 0) {
        offset = 0;
        do {
            base = D(entry + 0xb1u);
            club = D(base + offset);
            player = D(club + 0xcfu);
            if (player == 0) goto advance;
            if (D(player + 0x39u) == 0) goto advance;
            date = cm_fn_0054cab0(0x00ae2c90, local + 0x18u, 2);
            date0 = D(date);
            date1 = D(date + 4u);
            D(local + 0x14u) = date1;
            player = D(club + 0xcfu);
            D(local + 0x10u) = date0;
            year = W(local + 0x12u);
            other_year = W(player + 0x40u);
            if (year < other_year) goto advance;
            if (year == other_year && (s16)(u16)date0 < (s16)W(player + 0x3eu))
                goto advance;
            receiver = D(local + 0xcu);
            result = cm_fn_0086f5b0(receiver, player, 1);
            if (result == 0) goto advance;
            value = D(result + 0xceu);
            best = D(local + 4u);
            if ((s32)value <= (s32)best) goto advance;
            player = D(club + 0xcfu);
            D(local + 4u) = value;
            D(local + 8u) = player;
advance:
            index = D(local);
            offset += 0x3bu;
            count = (s16)W(entry + 0x3eu);
            ++index;
            D(local) = index;
        } while ((s32)index < (s32)count);
    }
    return D(local + 8u);
}
