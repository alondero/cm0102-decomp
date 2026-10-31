/* WIP: coverage0.0926 after one canonical failure; retained real-vtable
   fixtures stop at0096a108 ->00583470, absent from pinned inventory. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006831a0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006830b0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Sole direct caller invokes0068a980, which never reads incoming EAX,
   then overwrites full EAX at00405753. */
void CM_FASTCALL cm_fn_004068d0(u32 self)
{
    u8 scratch[0x20c];
    u32 local = (u32)scratch;
    u32 base, id, competition, table, target, allocation, text;
    u32 selected = 0, scanned, club, pointer, value, limit;
    u32 row, offset, index, nation, context, position;
    u8 removed;
    base = D(0x00adadfc);
    D(local + 4) = self;
    id = D(0x009cf658);
    competition = D(base + id * 4u);
    table = D(competition);
    D(local) = competition;
    target = D(table + 0x34);
    ((u32 (CM_THISCALL *)(u32, u32))target)(competition, 1);
    allocation = (u32)cm_fn_00944e46(0xa0);
    D(local + 8) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009873e8, 0, 0, local + 0x10c, local + 0xc);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       local + 0x10c, local + 0xc, 0x4b8);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return;
    }
    limit = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)limit > 0) {
        do {
            if ((s16)selected >= 40)
                break;
            pointer = D(club + 0x57);
            if (pointer != 0) {
                value = D(club + 0x53);
                if (value != 0) {
                    value = D(club + 0x60);
                    if (value != 0) {
                        id = D(pointer);
                        value = D(0x009cf688);
                        if (id == value) {
                            position = (u32)(s32)(s16)selected;
                            ++selected;
                            D(allocation + position * 4u) = club;
                        }
                    }
                }
            }
            limit = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)scanned < (s32)limit);
    }
    if ((s16)selected == 0) {
        cm_fn_009452ca(allocation);
        return;
    }
    cm_fn_004afbd0(allocation, selected, 6);
    index = 0;
    removed = 0;
    if ((s16)W(competition + 0x3e) > 0) {
        offset = 0;
        do {
            if ((s8)removed >= 4)
                break;
            competition = D(local);
            base = D(competition + 0xb1);
            row = base + offset;
            value = B(row + 0x37);
            if ((u8)value == 3) {
                id = D(0x009cf688);
                club = D(row);
                removed = (u8)(removed + 1u);
                base = D(0x00ae23d0);
                nation = id * 107u + base;
                context = D(local + 4);
                cm_fn_006831a0(context, club, nation, 1);
            }
            competition = D(local);
            ++index;
            offset += 59;
            limit = (u32)(s32)(s16)W(competition + 0x3e);
        } while ((s32)index < (s32)limit);
    }
    limit = (u32)(s32)(s16)selected;
    index = 0;
    if ((s32)limit > 0) {
        position = D(local + 8);
        do {
            if (removed == 0)
                break;
            id = D(0x009cf658);
            base = D(0x00ae23d0);
            club = D(position);
            nation = id * 107u + base;
            context = D(local + 4);
            cm_fn_006830b0(context, club, nation, 1);
            removed = (u8)(removed - 1u);
            D(position) = 0;
            ++index;
            position += 4;
        } while ((s32)index < (s32)limit);
    }
}
