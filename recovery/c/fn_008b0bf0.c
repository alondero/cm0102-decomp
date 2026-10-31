#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 filename, u32 flags_a, u32 flags_b,
                                    void *output_a, void *output_b);
extern void CM_CDECL cm_fn_005e8290(u32 message, u32 caption, u32 flags);

u32 CM_THISCALL cm_fn_008b0bf0(u8 *self, u8 value)
{
    if (value <= 9) {
        *(volatile u8 *)((u32)self + 0x7d9u) = (u8)value;
        return 1u;
    }

    u8 scratch[0x200];
    (void)cm_fn_00944cff(0x00a8cc88u, 0, 0, &scratch[0x100], &scratch[0]);
    u32 detail = cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, detail,
                         (u32)&scratch[0x100], (u32)&scratch[0], 0x67u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return 0;
}
