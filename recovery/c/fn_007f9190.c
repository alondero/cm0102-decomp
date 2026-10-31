#include "cmre.h"
/* Preferred-image VA 0x007f9190, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_STDCALL cm_fn_007f9190(u32 nation, u32 club)
{
    u8 scratch[512];
    u32 index, person, receiver, pointer, club_nation;
    s32 high;
    u8 first, second;
    char *version;
    if (!club) {
        index = (u32)(s32)(s16)W(0x00b77c7a);
        person = D(0x00b74c26 + index * 768u);
        club = D(person + 0x39);
        if (!club) {
            cm_fn_00944cff(0x00a7b094, 0, 0, scratch + 256, scratch);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x6bb);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
            D(0x00b67a34) = 0;
            return 0;
        }
    }
    receiver = D(0x00ae2a38);
    pointer = cm_fn_005a0590(receiver, club);
    if (!pointer) return 1;
    high = (s32)D(pointer + 4);
    if (high > 0) return 1;
    if (high == 0 && D(pointer) >= 1000000u) return 1;
    if ((s16)W(club + 0x80) >= 5000) return 1;
    club_nation = D(club + 0x53);
    first = B(club_nation + 0x76);
    second = B(nation + 0x76);
    return first == second;
}
