/* Preferred-image VA 0056d640, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00521e60(u32 object, u32 zero, u32 ten, u32 nine, u32 flag, u32 five, u32 season);
extern unsigned long long CM_CDECL cm_fn_00521eb0(u32 object, u32 zero, u32 kind, u32 flag, u32 one, u32 two, u32 other_two, u32 season, u32 last);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0056d640(u32 self, u8 kind, u32 first, u32 second, u32 optional)
{
    u8 work[0x200];
    u32 allocation, version, season;
    if (kind != 0xffu)
        return 0;
    if (optional != 0)
        D(optional) = 0;
    W(first) = 1;
    W(second) = 0;
    allocation = (u32)cm_fn_00944e46(0x68u);
    if (allocation == 0) {
        cm_fn_00944cff(0x009c425cu, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x9cu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    season = (u32)(s32)(s16)W(self + 0x40u);
    cm_fn_00521e60(allocation, 0, 5, 6, 0, 5, season);
    season = (u32)(s32)(s16)W(self + 0x40u);
    cm_fn_00521eb0(allocation, 0, 13, 7, 0, 6, 1, season, 4);
    W(allocation + 0x18u) = 2;
    W(allocation + 0x1cu) = 2;
    W(allocation + 7u) = 0;
    W(allocation + 9u) = 0;
    W(allocation + 0xbu) = 0;
    W(allocation + 0xdu) = 1;
    B(allocation + 0x17u) = 6;
    W(allocation + 0x1au) = 1;
    W(allocation + 0xfu) = 0;
    W(allocation + 0x1eu) = 0;
    B(allocation + 0x20u) = 0;
    B(allocation + 0x21u) = 1;
    B(allocation + 0x22u) = 0;
    D(allocation + 0x5cu) = 0;
    D(allocation + 0x60u) = 0;
    D(allocation + 0x64u) = 0;
    return allocation;
}
