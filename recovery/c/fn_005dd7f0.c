#include "cmre.h"

/* Preferred-image VA 0x005dd7f0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Only the BYTE counter is initialized in its raw DWORD argument slot;
   00687430 consumes its signed low BYTE. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005dd7f0(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch, value, row, nation, competition, id, other;
    u32 scan = 0, removed, total, bound, table, index_word, text;
    u16 year, count;
    u8 index = 0;
    year = W(self + 0x40);
    W(self + 0x3e) = 0x13;
    if (year != 0x7d0) W(self + 0x3e) = 0x12;
    count = W(self + 0x3e);
    W(self + 0xa7) = 0xffff;
    B(local) = 0;
    value = cm_fn_00944e46((u32)(s32)(s16)count * 59u);
    D(self + 0xb1) = value;
    if (value == 0) {
        cm_fn_00944cff(0x009cd488, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x108, local + 8, 0x13b);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    removed = D(0x00ae2350);
    total = D(0x00ae235c);
    row = D(0x00ae23b4);
    bound = total - removed * 2u;
    if ((s32)bound <= 0) goto done;
    do {
        competition = D(row + 0x57);
        if (competition != 0) {
            nation = D(row + 0x60);
            if (nation != 0) {
                id = D(nation);
                other = D(0x009cf55c);
                if (id == other) {
                    id = D(competition);
                    other = D(0x009cf564);
                    if (id == other) {
                        if ((s16)(s8)index >= (s16)W(self + 0x3e)) {
                            cm_fn_00944cff(0x009cd488, 0, 0, local + 8, local + 0x108);
                            text = cm_fn_0090d130();
                            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 8, local + 0x108, 0x14c);
                            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                            D(0x00b67a34) = 0;
                            D(row + 0x57) = 0;
                            goto done;
                        }
                        index_word = D(local);
                        cm_fn_00687430(self, index_word, row, 0, 0);
                        value = cm_fn_00540a50(row, local + 4, 1);
                        if (value != 0 && D(local + 4) == 0) {
                            table = D(self + 0xb1);
                            B(table + (u32)(s32)(s8)index * 59u + 0x37) = 0xfc;
                        }
                        index = (u8)(index + 1);
                        B(local) = index;
                    }
                }
            }
        }
        removed = D(0x00ae2350);
        total = D(0x00ae235c);
        ++scan;
        row += 0x245;
        bound = total - removed * 2u;
    } while ((s32)scan < (s32)bound);
done:
    return (s16)(s8)index == (s16)W(self + 0x3e);
}
