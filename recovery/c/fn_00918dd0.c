#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_005e8290(const char *text, const char *caption,
                                    s32 flags);

static u32 cm_fn_00918dd0_error(u8 scratch[0x200])
{
    u8 *first = &scratch[0];
    u8 *second = &scratch[0x100];
    (void)cm_fn_00944cff(0x00ad4a6cu, 0u, 0u, second, first);
    {
        u32 message = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                             (u32)second, (u32)first, 0x25fu);
    }
    u32 message_result = cm_fn_005e8290((const char *)0x009870e0u,
                                        (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
    return (message_result & 0xffff0000u) | 5u;
}

u32 CM_THISCALL cm_fn_00918dd0(void *self, u8 mode)
{
    u8 scratch[0x200];
    u32 selector = (u32)(s32)*(volatile s8 *)((u32)self + 0xf6u);
    u32 target;

    if (selector > 3u)
        return cm_fn_00918dd0_error(scratch);
    target = CM_GLOBAL(volatile u32, 0x00918eacu + selector * 4u);
    switch (target) {
    case 0x00918de9u:
        if (mode == 2u || mode == 4u || mode == 6u || mode == 7u)
            return 6u;
        return 5u;
    case 0x00918dfcu:
        if (mode == 2u || mode == 4u || mode == 7u)
            return 6u;
        return 5u;
    case 0x00918e20u:
        return mode == 2u ? 5u : 6u;
    case 0x00918e9eu:
        return 5u;
    default:
        return 5u;
    }
}
