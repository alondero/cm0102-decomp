#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern void CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_00850530(void)
{
    u8 scratch[0x34c];
    u32 local = (u32)scratch;
    u32 a, b, c, d, base, nations, club, total, limit, competition, id, target;
    u32 selected, scanned, index, value, text, result;
    b = D(0x009cf630) * 107u;
    a = D(0x009cf62c) * 107u;
    D(local + 4) = 0;
    c = D(0x009cf634) * 107u;
    D(local + 8) = 0;
    d = D(0x009cf638) * 107u;
    base = D(0x00ae23d0);
    a += base;
    nations = D(0x00ae2350);
    b += base;
    c += base;
    d += base;
    club = D(0x00ae23b4);
    D(local) = club;
    total = D(0x00ae235c);
    limit = total - nations * 2u;
    if ((s32)limit > 0) {
        do {
            club = D(local);
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                target = D(0x009cf7b8);
                if (id == target) {
                    index = (u32)(s32)(s16)W(local + 4);
                    club = D(local);
                    D(local + 0xc + index * 4u) = club;
                    selected = D(local + 4) + 1u;
                    D(local + 4) = selected;
                    if ((u16)selected == 80)
                        goto ready;
                }
            }
            scanned = D(local + 8);
            club = D(local);
            ++scanned;
            club += 581;
            D(local + 8) = scanned;
            D(local) = club;
        } while ((s32)scanned < (s32)limit);
        if (W(local + 4) == 80)
            goto ready;
    }
    cm_fn_00944cff(0x00a82844, 0, 0, local + 0x14c, local + 0x24c);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x14c, local + 0x24c, 0x3f7);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
ready:
    cm_fn_00945333(local + 0xc, 80, 4, 0x0053fd40);
    cm_fn_00945333(local + 0x5c, 60, 4, 0x0053fe50);
    for (index = 0; index != 80; ++index) {
        value = D(local + 0xc + index * 4u);
        target = index < 20 ? d : index < 40 ? a : index < 60 ? b : c;
        D(value + 0x60) = target;
    }
    return 80;
}
