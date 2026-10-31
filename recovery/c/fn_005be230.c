#include "cmre.h"
/* Preferred-image VA 0x005be230, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_THISCALL cm_fn_005be230(u32 self, u32 clubs)
{
    u8 scratch[512];
    u32 allocation, index = 0, club, person, id, wanted;
    u8 selected = 0, previous, flag;
    char *version;
    W(self + 0x3e) = 20;
    allocation = (u32)cm_fn_00944e46(0x49c);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x009ccbd0, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x175);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if ((s16)W(self + 0x3e) > 0) {
        do {
            club = D(clubs + (u32)(s32)(s16)index * 4u);
            person = D(club + 0x57);
            id = D(person);
            wanted = D(0x009cf5d4);
            previous = selected;
            flag = id == wanted;
            ++selected;
            cm_fn_00687430(self, previous, club, flag, 0);
            ++index;
        } while ((s16)index < (s16)W(self + 0x3e));
    }
    return (u16)(s16)(s8)selected == W(self + 0x3e);
}
