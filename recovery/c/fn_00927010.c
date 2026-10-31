#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00945501(u32, u32);
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern void CM_CDECL cm_fn_004afb40(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00927010(u32 self)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 selected, allocation, total, club, scanned, competition, id, target;
    u32 group, name, extension, line, text;
    selected = 0;
    D(self + 0xf2) = 18;
    allocation = cm_fn_00945501(4, 18);
    D(self + 0xee) = allocation;
    if (allocation == 0) {
        name = local + 0x100;
        extension = local;
        line = 0x2cd;
        goto error;
    }
    total = D(0x00ae235c);
    club = D(0x00ae23b4);
    scanned = 0;
    if ((s32)total > 0) {
        do {
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf94c);
                if (id == target) {
                    target = D(self + 0xee);
                    ++selected;
                    D(target + selected * 4u - 4u) = club;
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 581;
        } while ((s32)(s16)scanned < (s32)total);
    }
    target = D(self + 0xee);
    cm_fn_00945333(target, selected, 4, 0x004b07d0);
    for (group = 0; group != 6; ++group) {
        target = D(self + 0xee);
        cm_fn_004afb40(target + group * 12u, 3);
    }
    total = D(self + 0xf2);
    if ((s32)selected >= (s32)total)
        return 1;
    name = local;
    extension = local + 0x100;
    line = 0x2e7;
error:
    cm_fn_00944cff(0x00ad56f4, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    /* An insufficient selection reports an error and still returns one. */
    return line == 0x2cd ? 0 : 1;
}
