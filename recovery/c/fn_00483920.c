/* Preferred-image VA 00483920, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Keep both dispatch-table accesses and native AL-only return updates. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00483920(s8 selector, u32 output)
{
    u8 work[0x200];
    u32 index, map, target, result, version, value;
    index = (u32)(s32)selector - 1u;
    if (index > 31u) {
        cm_fn_00944cff(0x0098e598u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x4eabu);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return (result & 0xffffff00u) | 4u;
    }
    map = B(0x00483a68u + index);
    target = D(0x00483a50u + map * 4u);
    switch (target) {
    case 0x00483944u: value = 7; result = 4; break;
    case 0x00483962u: value = 10; result = 5; break;
    case 0x0048397cu: value = 13; result = 6; break;
    case 0x00483996u: value = 16; result = 7; break;
    case 0x004839b0u: value = 19; result = 8; break;
    case 0x004839cau: value = 22; result = 9; break;
    default: __builtin_unreachable();
    }
    if (output != 0)
        D(output) = value;
    return (output & 0xffffff00u) | result;
}
