/* Preferred-image VA 0063af60, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native0068a1c0 returns ECX=p7 at0068a3bc and EDX's high16 from
   base+65*index (0068a38a..0068a3b1). Preserve the later CX/DX writes. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_0068a160(u32 table, u32 index, u32 day, u32 month, u32 zero, u32 kind, u32 teams, u32 season, u32 last);
extern void CM_CDECL cm_fn_0068a1c0(u32 table, u32 index, u32 zero, u32 first, u32 second, u32 third, u32 last);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0063af60(u32 self, u8 kind, volatile u32 second,
                              volatile u32 third, volatile u32 fourth)
{
    u8 work[0x200];
    u32 optional, first, last, allocation, season, version;
    if (kind != 0xffu)
        return 0;
    optional = fourth;
    if (optional != 0)
        D(optional) = 1;
    first = second;
    last = third;
    W(first) = 3;
    W(last) = 0;
    allocation = (u32)cm_fn_00944e46(0xc3u);
    if (allocation == 0) {
        cm_fn_00944cff(0x009ea0b8u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x91u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    season = (allocation & 0xffff0000u) | W(self + 0x40u);
    cm_fn_0068a160(allocation, 0, 27, 5, 0, 2, 2, season, 0);
    cm_fn_0068a1c0(allocation, 0, 0, 0xffffffffu, 0xffffffffu, 0xffffffffu, 0);
    season = W(self + 0x40u);
    cm_fn_0068a160(allocation, 1, 29, 5, 0, 4, 2, season, 0);
    cm_fn_0068a1c0(allocation, 1, 0, 0xffffffffu, 0xffffffffu, 0xffffffffu, 0);
    season = ((allocation + 65u) & 0xffff0000u) | W(self + 0x40u);
    cm_fn_0068a160(allocation, 2, 1, 6, 0, 6, 1, season, 0);
    cm_fn_0068a1c0(allocation, 2, 0, 0xffffffffu, 0xffffffffu, 0xffffffffu, 0);
    return allocation;
}
