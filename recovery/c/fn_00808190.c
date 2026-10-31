#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (read only on the
   table paths): 0x24 + index for a signed byte index in 1..31 (second-argument
   zero), and 0x64 + index for a signed byte index in 1..12 (second-argument
   non-zero). Globals: 0x00a7df38 is the format string passed to 0x00944cff;
   0x00ae24d0, 0x009870e8 and 0x009870e0 are the report-tail strings and
   buffers; 0x00b67a34 dword cleared on the report path. */

extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern char *cm_fn_0090d130(void);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, s32 param_3);

u8 CM_THISCALL cm_fn_00808190(u32 self, u32 arg1, u32 arg2)
{
    /* Two 0x100-byte buffers: the first at offset 0, the second at offset 0x100. */
    u8 scratch[0x200];
    s8 index;

    if (arg2 != 0) {
        index = (s8)(u8)arg1;
        if (index >= 1 && index <= 12) {
            return *(volatile u8 *)(self + (u32)(s32)index + 0x64u);
        }

        /* 0x008081bd: 0x944cff(0xa7df38, 0, 0, L+0x100, L), then the report tail
           with 0x3c6 as the last vararg and L+0x100 pushed last (first vararg). */
        cm_fn_00944cff(0x00a7df38u, 0u, 0u, scratch + 0x100, scratch);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                             (u32)(scratch + 0x100), (u32)scratch, 0x3c6u);
    } else {
        index = (s8)(u8)arg1;
        if (index >= 1 && index <= 31) {
            return *(volatile u8 *)(self + (u32)(s32)index + 0x24u);
        }

        /* 0x00808207: 0x944cff(0xa7df38, 0, 0, L, L+0x100), then the report tail
           with 0x3d2 as the last vararg. */
        cm_fn_00944cff(0x00a7df38u, 0u, 0u, scratch, scratch + 0x100);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)cm_fn_0090d130(),
                             (u32)scratch, (u32)(scratch + 0x100), 0x3d2u);
    }

    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
    return 1u;
}
