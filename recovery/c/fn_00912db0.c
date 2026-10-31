#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e8290(const char *format, const char *message, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_00912db0(u8 *self, u8 param)
{
    s32 selector = *(volatile s8 *)(self + 0xee);
    volatile u32 *targets = (volatile u32 *)0x00912e94;

    if ((u32)selector > 3u) {
        u8 scratch[0x200];
        u32 error_result;
        u32 text_result;
        u32 format_result;

        cm_fn_00944cff(0x00ad4680u, 0, 0, scratch + 0x100, scratch);
        text_result = cm_fn_0090d130();
        format_result = cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                        text_result, scratch + 0x100, scratch, 0x37fu);
        (void)format_result;
        error_result = cm_fn_005e8290((const char *)0x009870e0,
                                      (const char *)0x00ae24d0, 0);
        *(volatile u32 *)0x00b67a34 = 0;
        return (error_result & 0xffff0000u) | 4u;
    }

    /* Preserve the original guarded DWORD jump-table access. */
    (void)*(volatile u32 *)&targets[selector];
    switch (selector) {
    case 0:
        return 4;
    case 1:
        return (param != 1u) + 3u;
    case 2:
        if (param == 3u || param == 6u)
            return 3;
        return 4;
    default:
        if (param == 1u || param == 3u || param == 5u)
            return 3;
        return 4;
    }
}
