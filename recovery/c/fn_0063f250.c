#include "cmre.h"
/* Preferred-image VA 0x0063f250, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Only the low index byte is initialized; retain the original DWORD read. */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_00687430(u32, u8, u32, u8, u32);
extern void CM_FASTCALL cm_fn_00684230(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_FASTCALL cm_fn_0063f250(u32 self)
{
    u8 scratch[0x204];
    u32 work = (u32)scratch, allocation, base, index, offset, club;
    u8 selected;
    u16 limit;
    char *version;
    W(self + 0x3e) = 4;
    allocation = (u32)cm_fn_00944e46(0xec);
    D(self + 0xb1) = allocation;
    if (!allocation) {
        cm_fn_00944cff(0x009ea23c, 0, 0, scratch + 0x104, scratch + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, scratch + 0x104, scratch + 4, 0x19e);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    selected = 0;
    limit = W(self + 0x3e);
    B(work) = 0;
    if ((s16)limit > 0) {
        do {
            base = D(self + 0xee);
            index = D(work);
            offset = (u32)(s32)(s8)selected * 4u;
            club = D(base + offset);
            cm_fn_00687430(self, (u8)index, club, 0, 0);
            base = D(self + 0xee);
            ++selected;
            D(base + offset) = 0;
            limit = W(self + 0x3e);
            B(work) = selected;
        } while ((s16)(s8)selected < (s16)limit);
    }
    cm_fn_00684230(self);
    return 1;
}
