#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00540a50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_FASTCALL cm_fn_008921b0(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch;
    u32 value, nation_total, club_total, club, index, limit, competition;
    u32 pointer, id, slot, row, text, name, extension, line;
    u8 selected = 0;
    W(self + 0x3e) = 12;
    B(local) = 0;
    value = (u32)cm_fn_00944e46(0x2c4);
    index = 0;
    D(self + 0xb1) = value;
    if (value == 0) {
        name = local + 0x108;
        extension = local + 8;
        line = 0x198;
        goto allocation_error;
    }
    nation_total = D(0x00ae2350);
    club_total = D(0x00ae235c);
    club = D(0x00ae23b4);
    limit = club_total - nation_total * 2u;
    if ((s32)limit > 0) {
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                pointer = D(club + 0x60);
                if (pointer != 0) {
                    id = D(pointer);
                    value = D(0x009cf5f8);
                    if (id == value && competition == D(self + 4)) {
                        if ((s16)(s8)selected >= (s16)W(self + 0x3e)) {
                            name = local + 8;
                            extension = local + 0x108;
                            line = 0x1a9;
                            goto selection_error;
                        }
                        /* The pushed slot is a DWORD; original006874fb consumes
                           only its low byte as the index. */
                        slot = D(local);
                        cm_fn_00687430(self, (u8)slot, club, 0, 0);
                        value = cm_fn_00540a50(club, local + 4, 1);
                        if (value != 0 && D(local + 4) == 0) {
                            row = (u32)(s32)(s8)selected * 59u;
                            pointer = D(self + 0xb1);
                            B(row + pointer + 0x37) = 0xfc;
                        }
                        selected = (u8)(selected + 1u);
                        B(local) = selected;
                    }
                }
            }
            nation_total = D(0x00ae2350);
            club_total = D(0x00ae235c);
            ++index;
            club += 581;
            limit = club_total - nation_total * 2u;
        } while ((s32)index < (s32)limit);
    }
    goto done;
selection_error:
    cm_fn_00944cff(0x00a8b5b0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    D(club + 0x57) = 0;
done:
    return (u16)(s16)(s8)selected == W(self + 0x3e);
allocation_error:
    cm_fn_00944cff(0x00a8b5b0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
