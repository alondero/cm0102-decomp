#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afb40(u32, u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0042dc90(u32 self)
{
    u8 scratch[0x200];
    u32 old, allocated, count, total, club, scanned, selected = 0, competition;
    u32 target, table, index, value, name, extension, line, text, result;
    old = D(self + 0xee);
    W(self + 0x3e) = 7;
    if (old != 0)
        cm_fn_009452ca(old);
    allocated = (u32)cm_fn_00944e46(0xe0);
    D(self + 0xee) = allocated;
    if (allocated == 0) {
        name = (u32)scratch + 0x100;
        extension = (u32)scratch;
        line = 0x18f;
        result = 1;
        goto error;
    }
    count = (u32)(s32)(s16)W(self + 0x3e);
    allocated = (u32)cm_fn_00944e46(count * 59u);
    D(self + 0xb1) = allocated;
    if (allocated == 0) {
        name = (u32)scratch;
        extension = (u32)scratch + 0x100;
        line = 0x196;
        result = 0;
        goto error;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)total > 0) {
        do {
            if ((s8)selected >= 56)
                break;
            competition = D(club + 0x57);
            target = D(self + 4);
            if (competition == target) {
                table = D(self + 0xee);
                index = (u32)(s32)(s8)selected;
                ++selected;
                D(table + index * 4u) = club;
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)scanned < (s32)total);
    }
    table = D(self + 0xee);
    count = (club & 0xffff0000u) | (u16)(s16)(s8)selected;
    cm_fn_004afb40(table, count);
    for (index = 0; index != 7; ++index) {
        table = D(self + 0xee);
        value = D(table + index * 4u);
        cm_fn_00687430(self, (u8)index, value, 0, 0);
    }
    return 1;
error:
    cm_fn_00944cff(0x0098aba4, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
