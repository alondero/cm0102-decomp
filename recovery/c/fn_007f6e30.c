#include "cmre.h"

extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                   s32 mode);

u8 CM_THISCALL cm_fn_007f6e30(volatile u8 *self,
                              const volatile u32 *index_pointer)
{
    volatile u8 scratch[0x200];
    const volatile u32 *selected = index_pointer;
    u32 fallback_value;

    if (selected == 0) {
        s32 table_index = *(volatile s16 *)0x00b77c7au;
        fallback_value = *(volatile u32 *)(0x00b74c26u +
                                             (u32)table_index * 0x300u);
        if (fallback_value == 0u) {
            (void)cm_fn_00944cff(0x00a7b094u, 0u, 0u,
                                 (void *)(scratch + 0x100u), (void *)scratch);
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 (u32)(unsigned long)cm_fn_0090d130(),
                                 (void *)(scratch + 0x100u), (void *)scratch,
                                 0x12cu);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
            return 0xffu;
        }
        selected = (const volatile u32 *)(unsigned long)fallback_value;
    }

    {
        u32 value = *selected;
        u32 difference = value - CM_GLOBAL(volatile u32, 0x00ae2364) + 0x10u;
        s32 signed_difference = (s32)difference;

        if (signed_difference >= 0 && signed_difference <= 0x0f)
            return self[0x4cu + difference];
    }

    (void)cm_fn_00944cff(0x00a7b094u, 0u, 0u,
                         (void *)scratch, (void *)(scratch + 0x100u));
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                         (u32)(unsigned long)cm_fn_0090d130(),
                         (void *)scratch, (void *)(scratch + 0x100u),
                         0x136u);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
    return 0xffu;
}
