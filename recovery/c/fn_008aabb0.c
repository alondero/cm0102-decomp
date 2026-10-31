/* Preferred-image VA 008aabb0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u8 CM_THISCALL cm_fn_008aabb0(u32 self, s8 index, u32 mode)
{
    u8 scratch[0x200];
    u32 base, offset, row, text;
    if (index < 0 || index > 19) {
        cm_fn_00944cff(0x00a8b94cu, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, scratch + 0x100, scratch, 0x1c31u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    if (mode != 0 && D(self + 0xde5u) == 0)
        return 0;
    base = D(self + 4u);
    if (base == 0)
        return 0;
    if (mode != 0) {
        offset = (u32)(s32)(s8)B(self + 0xdeau) * 20u + (u32)(s32)index;
        row = base + offset * 0x1beu + 0x4796u;
    } else {
        row = self + (u32)(s32)index * 0x1beu + 0x2735u;
    }
    if (B(row + 0x19u) == 0xfbu)
        return 0;
    return B(row + 0x24u);
}
