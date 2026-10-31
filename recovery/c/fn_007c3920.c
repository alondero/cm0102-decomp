#include "cmre.h"
/* Preferred-image VA 0x007c3920, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_007c3920(u32 self, u32 player, s8 kind)
{
    u8 scratch[0x200];
    u8 *first, *second;
    u32 index, slots, target, offset, line, base;
    char *version;
    if (player == 0) {
        first = scratch + 0x100; second = scratch; line = 0xa65;
        goto error;
    }
    index = D(player);
    base = D(self + 0x30u);
    slots = base + index * 39u;
    if ((u32)((s32)kind - 1) > 5u) goto invalid;
    target = D(0x007c3a28u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x007c3987: offset = 0; break;
    case 0x007c398b: offset = 4; break;
    case 0x007c3990: offset = 8; break;
    case 0x007c3995: offset = 12; break;
    case 0x007c399a: offset = 16; break;
    case 0x007c399f: offset = 20; break;
    default: goto invalid;
    }
    index = D(slots + offset);
    if ((s32)index <= -1) return 0;
    base = D(self + 4u);
    return (index << 5) + base;
invalid:
    first = scratch; second = scratch + 0x100; line = 0xaa7;
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
