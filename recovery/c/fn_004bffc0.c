/* Preferred-image VA 004bffc0, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_004b4850(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_004ba8f0(u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_004bffc0(u32 first, u32 second, u32 output, u32 size, u32 mode)
{
    u8 scratch[0x2c8];
    if (!output || (s32)size < 0xc8) {
        cm_fn_00944cff(0x0099ec20, 0, 0, scratch + 0x1c8, scratch + 0xc8);
        u32 text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x1c8, scratch + 0xc8, 0x26a1);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    B(output) = 0;
    if (first) {
        if (second) {
            cm_fn_004b4850((u32)scratch, 100, first, mode);
            u8 a = (u8)cm_fn_004ba8f0(first, mode);
            cm_fn_004b4850((u32)scratch + 0x64, 100, second, mode);
            u8 b = (u8)cm_fn_004ba8f0(second, mode);
            u32 x = (u32)(s32)(s8)a;
            u32 y = (u32)(s32)(s8)b;
            cm_fn_0066f4e0(output, 0x009a7cfc, x, x, y, y, scratch, scratch + 0x64);
        } else {
            cm_fn_004b4850(output, size, first, mode);
        }
        return 1;
    }
    if (second) {
        cm_fn_004b4850(output, size, second, mode);
        return 1;
    }
    return 0;
}
