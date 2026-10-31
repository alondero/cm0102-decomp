#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0078d800(u32 self)
{
    u8 scratch[0x204];
    u32 allocation, count, club, field, target, id, table, index, stop, scanned, text;
    D((u32)scratch) = self;
    D(self + 0xf2) = 0x14;
    allocation = cm_fn_00945501(4, 0x14);
    D(self + 0xee) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00a69698, 0, 0, (u32)scratch + 0x104, (u32)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                      (u32)scratch + 0x104, (u32)scratch + 4, 0x2b3);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    count = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)count > 0) {
        do {
            field = D(club + 0x53);
            if (field != 0) {
                target = D(0x009cf3e4);
                id = D(field);
                if (id == target) {
                    field = D(club + 0x60);
                    if (field != 0) {
                        id = D(field);
                        if (id != 0) {
                            if (id == D(0x009cf82c)) {
                                index = 0;
                                stop = 5;
                            } else if (id == D(0x009cf830)) {
                                index = 5;
                                stop = 10;
                            } else if (id == D(0x009cf834)) {
                                index = 10;
                                stop = 15;
                            } else if (id == D(0x009cf838)) {
                                index = 15;
                                stop = 20;
                            } else {
                                goto first_next;
                            }
                            table = D(self + 0xee);
                            do {
                                if (D(table + (u32)(s32)(s16)index * 4u) == 0) {
                                    D(table + (u32)(s32)(s16)index * 4u) = club;
                                    break;
                                }
                                ++index;
                            } while ((s16)index < (s16)stop);
                            self = D((u32)scratch);
                        }
                    }
                }
            }
first_next:
            count = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)(s16)scanned < (s32)count);
    }
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)count > 0) {
        do {
            field = D(club + 0x53);
            if (field != 0) {
                target = D(0x009cf3e4);
                id = D(field);
                if (id == target) {
                    field = D(club + 0x60);
                    if (field == 0) {
                        table = D(self + 0xee);
                        index = 0;
                        do {
                            if (D(table + (u32)(s32)(s16)index * 4u) == 0) {
                                D(table + (u32)(s32)(s16)index * 4u) = club;
                                break;
                            }
                            ++index;
                        } while ((s16)index < 20);
                    }
                }
            }
            count = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)(s16)scanned < (s32)count);
    }
    return 1;
}
