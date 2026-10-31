/* Preferred-image VA 005eb120, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   006831a0 reads only stack arguments and RET12; incoming ECX is dead. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32);
extern void CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005eb120(u32 self __attribute__((unused)))
{
    u8 scratch[0x204];
    u32 table = D(0x00adadfc);
    u32 id = D(0x009cf870);
    u32 stage = D(table + id * 4);
    u32 total = D(0x00ae235c);
    u32 buffer = (u32)cm_fn_00944e46(total * 4);
    if (!buffer) {
        cm_fn_00944cff(0x009cdae4, 0, 0, scratch + 0x104, scratch + 4);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x104, scratch + 4, 0x2d5);
        u32 result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    total = D(0x00ae235c);
    u32 club = D(0x00ae23b4);
    u32 scanned = 0;
    u32 found = 0;
    if ((s32)total > 0) {
        do {
            u32 competition = D(club + 0x57);
            if (competition) {
                u32 number = D(competition);
                u32 target = D(0x009cf874);
                if (number == target) {
                    u32 offset = (u32)(s32)(s16)found * 4u;
                    ++found;
                    D(buffer + offset) = club;
                }
            }
            total = D(0x00ae235c);
            ++scanned;
            club += 0x245;
        } while ((s32)scanned < (s32)total);
        if ((s16)found >= 2) {
            cm_fn_004afbd0(buffer, found, 3);
            u32 cursor = buffer;
            for (u32 i = 0; i < 2; ++i, cursor += 4) {
                id = D(0x009cf870);
                u32 base = D(0x00ae23d0);
                club = D(cursor);
                cm_fn_006830b0(club, base + id * 107u, 1);
            }
            u32 index = 0;
            if ((s16)W(stage + 0x3e) > 0) {
                do {
                    u32 entry = cm_fn_006826d0(stage, index);
                    if (B(entry + 0x37) == 3) {
                        id = D(0x009cf874);
                        u32 base = D(0x00ae23d0);
                        u32 competition = base + id * 107u;
                        entry = cm_fn_006826d0(stage, index);
                        club = D(entry);
                        cm_fn_006831a0(club, competition, 1);
                    }
                    s32 count = (s16)W(stage + 0x3e);
                    ++index;
                    if ((s32)index >= count) break;
                } while (1);
            }
        }
    }
    return (u32)cm_fn_009452ca(buffer);
}
