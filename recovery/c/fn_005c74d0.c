#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern void CM_CDECL cm_fn_005e8290(const char *format, const char *destination,
                                     int flags);

u8 CM_STDCALL cm_fn_005c74d0(s8 input)
{
    u32 index = (u32)(s32)input;
    u32 target;

    if (index <= 0x0bu) {
        target = CM_GLOBAL(volatile u32, 0x005c756cu + index * 4u);
        if (target == 0x005c74e7u)
            return 2;
        if (target == 0x005c74f2u)
            return 3;
        if (target == 0x005c7560u)
            return 1;
    }

    {
        u8 scratch[0x200];
        (void)cm_fn_00944cff(0x009cd0a0u, 0, 0, scratch + 0x100, scratch);
        {
            u32 result = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 result, scratch + 0x100, scratch, 0xf18u);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 1;
    }
}
