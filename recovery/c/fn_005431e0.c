#include "cmre.h"

extern u8 CM_CDECL cm_fn_00543540(u32 object, u32 field, u8 index);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern void CM_CDECL cm_fn_005e8290(const char *format, const char *destination,
                                     int flags);

u8 CM_CDECL cm_fn_005431e0(u32 object, u32 argument)
{
    u32 raw_argument;
    s8 index;

    if (object != 0) {
        raw_argument = argument;
        index = (s8)(u8)raw_argument;
        if (index >= 1 && index <= 0x1f) {
            u32 field = *(volatile u32 *)(object + 0x61u);
            return cm_fn_00543540(object, field, (u8)raw_argument);
        }
    }

    {
        u8 scratch[0x200];
        (void)cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x100, scratch);
        {
            u32 result = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 result, scratch + 0x100, scratch, 0x4816u);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0xffu;
    }
}
