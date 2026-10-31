#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern void *CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_006399c0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00639670(u32 self)
{
    u8 scratch[0x230];
    u32 index, count, club, target, competition, rank, table, selected, value, text;
    W(self + 0x3e) = 2;
    for (index = 0; index != 12; ++index)
        D((u32)scratch + index * 4u) = 0;
    count = D(0x00ae235c);
    if ((s32)count > 0) {
        club = D(0x00ae23b4);
        target = D(0x009cf80c);
        do {
            competition = D(club + 0x5b);
            if (competition != 0 && D(competition) == target) {
                rank = B(club + 0x5f);
                if ((s8)rank >= 1 && (s8)rank <= 12)
                    D((u32)scratch + ((u32)(s32)(s8)rank - 1u) * 4u) = club;
            }
            club += 0x245;
            --count;
        } while (count != 0);
    }
    table = (u32)cm_fn_00945501(2, 6);
    D(self + 0xa7) = table;
    if (table == 0) {
        cm_fn_00944cff(0x009ea000, 0, 0, (u32)scratch + 0x130, (u32)scratch + 0x30);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x130, (u32)scratch + 0x30, 0xd9);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    for (index = 0; index != 12; ++index) {
        selected = D((u32)scratch + index * 4u);
        if (selected != 0 && cm_fn_006399c0(selected) != 0) {
            table = D(self + 0xa7);
            if (D(table) == 0) {
                D(table) = selected;
                D((u32)scratch + index * 4u) = 0;
            } else {
                table = D(self + 0xa7);
                value = D((u32)scratch + index * 4u);
                D(table + 6) = value;
                D((u32)scratch + index * 4u) = 0;
                break;
            }
        }
    }
    table = D(self + 0xa7);
    if (D(table + 6) == 0) {
        for (index = 0; index != 12; ++index) {
            selected = D((u32)scratch + index * 4u);
            if (selected != 0) {
                table = D(self + 0xa7);
                if (D(table) == 0) {
                    D(table) = selected;
                } else {
                    table = D(self + 0xa7);
                    value = D((u32)scratch + index * 4u);
                    D(table + 6) = value;
                    break;
                }
            }
        }
    }
    table = D(self + 0xa7);
    B(table + 4) = 0;
    table = D(self + 0xa7);
    B(table + 5) = 0;
    table = D(self + 0xa7);
    B(table + 0xa) = 0;
    table = D(self + 0xa7);
    B(table + 0xb) = 0;
    return 1;
}
