#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005b4a60(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_005b4b70(u32 self, s8 first, s8 second, u32 third);
extern void cm_fn_005e8290(const char *title, const char *message, int value);
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3, void *out1, void *out2);

u32 CM_THISCALL cm_fn_008a9fa0(u32 self, u32 value)
{
    u8 *const obj = (u8 *)self;
    volatile u32 scratch[128];

    if ((s8)(u8)value < 0 || (s8)(u8)value > 10) {
        u32 *const first = (u32 *)(void *)&scratch[0];
        u32 *const second = (u32 *)(void *)&scratch[64];
        (void)cm_fn_00944cff(0x00a8b94cu, 0, 0, second, first);
        char *const converted = cm_fn_0090d130();
        /* Three stack arguments prepared for this formatter remain live. */
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)converted,
                             (u32)second, (u32)first, 0x1a0du);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
        return 0;
    }

    u8 mode = *(volatile u8 *)(obj + 0x2727);
    if (mode == 1) {
        return cm_fn_005b4a60(self + 0xe44u, value);
    }

    u32 third = *(volatile u32 *)(obj + 0x2729);
    s8 second = (mode == 2) ? 1 : 0;
    return cm_fn_005b4b70(self + 0xe44u, (s8)(u8)value, second, third);
}
