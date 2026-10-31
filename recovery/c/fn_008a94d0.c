#include "cmre.h"

/* Preferred-image VA 0x008a94d0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The callee reads only the low bytes of its first two stack arguments. */
extern u32 CM_THISCALL cm_fn_005b3c70(u32, s8, s8, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);

u32 CM_THISCALL cm_fn_008a94d0(u32 self, s8 index, u32 output)
{
    u8 scratch[512];
    u8 mode, alternate;
    u32 grid;
    char *version;

    if (index >= 0 && index <= 10) {
        mode = *(volatile u8 *)(self + 0x2728u);
        grid = *(volatile u32 *)(self + 0x272du);
        if (mode == 1) {
            alternate = *(volatile u8 *)(self + 0x2727u);
            cm_fn_005b3c70(self + 0xe44u, index, alternate == 2, grid, output);
        } else {
            cm_fn_005b3c70(self + 0xe44u, index, mode == 2, grid, output);
        }
        return 1;
    }
    cm_fn_00944cff(0x00a8b94cu, 0, 0, scratch + 256, scratch);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version,
                  scratch + 256, scratch, 0x1784);
    cm_fn_005e8290((const char *)0x009870e0u,
                  (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return 0;
}
