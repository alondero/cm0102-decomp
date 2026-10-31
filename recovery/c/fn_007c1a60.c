#include "cmre.h"
/* Preferred-image VA 0x007c1a60, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);

long double CM_THISCALL cm_fn_007c1a60(u32 self, u32 player)
{
    u8 scratch[0x204];
    u32 index, base, nibble;
    long double value;
    char *version;
    if (player == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x104, scratch + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x104, scratch + 4, 0x4bd);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        value = (long double)CM_GLOBAL(volatile float, 0x00968970);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return value;
    }
    index = *(volatile u32 *)player;
    base = *(volatile u32 *)(self + 0x30u);
    nibble = *(volatile u8 *)(base + index * 39u + 0x26u);
    nibble &= 15u;
    if (nibble != 0) return (long double)nibble;
    return (long double)CM_GLOBAL(volatile float, 0x0096e2b8);
}
