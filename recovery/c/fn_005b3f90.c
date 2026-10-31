/* Preferred-image VA 005b3f90, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_005b3c70(u32, s8, s8, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_005b3f90(u32 self, u32 alternate, u32 grid, u32 output)
{
    u8 scratch[0x204];
    u32 line;
    u8 *name = scratch + 4;
    u8 *extension = scratch + 0x104;
    if ((s8)alternate < 0 || (s8)alternate > 1) {
        line = 0x3b7;
        goto error;
    }
    if ((s16)grid < 0 || (s16)grid > 2 ||
        (s16)(grid >> 16) < 0 || (s16)(grid >> 16) > 3) {
        line = 0x3c0;
        goto error;
    }
    if (!output) {
        line = 0x3c8;
        name = scratch + 0x104;
        extension = scratch + 4;
        goto error;
    }
    for (u32 i = 0; i < 11; ++i, output += 4)
        cm_fn_005b3c70(self, (s8)i, (s8)alternate, grid, output);
    return 1;
error:
    cm_fn_00944cff(0x009cc3dc, 0, 0, name, extension);
    u32 text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
