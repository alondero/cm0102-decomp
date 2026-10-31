#include "cmre.h"
/* Preferred-image VA 0x004739d0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The owned-club branch returns the second argument left in EAX. */
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))

u32 CM_CDECL cm_fn_004739d0(u32 record, u32 value)
{
    u8 scratch[512];
    u32 index, club, manager, current, owned, eax;
    char *version;
    if (!record) {
        cm_fn_00944cff(0x0098e598, 0, 0, scratch + 256, scratch);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 256, scratch, 0x3624);
        eax = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return eax;
    }
    index = (u32)(s32)(s16)W(0x00b77c7a);
    club = D(record);
    manager = D(club + 0xcf);
    current = D(0x00b74c26 + index * 768u);
    owned = manager == current;
    eax = cm_fn_007fe600(0x00b74450, 0x00473ad0, 0x00473fb0, owned, 0, 0);
    if (!eax) return eax;
    cm_fn_007ff270(0x00b74450, 0, record, 0);
    eax = (u32)cm_fn_007ff270(0x00b74450, 1, owned, 0);
    if (owned) {
        D(record + 0x29) = 1;
        D(record + 0x2d) = value;
        eax = value;
    }
    return eax;
}
