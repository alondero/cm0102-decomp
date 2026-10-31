#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e8290(const char *arg1, const char *arg2, s32 arg3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_0057d9d0(void *self, u8 code)
{
    volatile u8 scratch[0x200];
    u32 base = (u32)&scratch[0];
    s8 selector = *(volatile s8 *)((u32)self + 0xf6u);

    if (selector == 1) {
        if (code == 0u || code == 2u || code == 5u)
            return 6u;
        return 5u;
    }
    if (selector == 2) {
        if (code == 0u || code == 2u || code == 5u || code == 7u)
            return 6u;
        return 5u;
    }

    (void)cm_fn_00944cff(0x009c4674u, 0u, 0u,
                         (void *)(base + 0x100u), (void *)base);
    {
        char *text = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                             base + 0x100u, base, 0x245u);
    }
    {
        u32 boundary_eax = cm_fn_005e8290((const char *)0x009870e0u,
                                           (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return (boundary_eax & 0xffff0000u) | 5u;
    }
}
