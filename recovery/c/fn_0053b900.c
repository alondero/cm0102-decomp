#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern void CM_CDECL cm_fn_005e8290(const char *format, const char *destination,
                                     int flags);

u32 CM_CDECL cm_fn_0053b900(u32 *input)
{
    u32 window;
    u32 value;
    u32 limit;

    if (input == 0) {
        u8 scratch[0x200];
        (void)cm_fn_00944cff(0x009b5ac8u, 0, 0, scratch + 0x100, scratch);
        {
            u32 result = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 result, scratch + 0x100, scratch, 0x349fu);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    window = CM_GLOBAL(volatile u32, 0x00ae2350u);
    value = *(volatile u32 *)input;
    limit = CM_GLOBAL(volatile u32, 0x00ae235cu);
    limit -= window + window;
    return (s32)value >= (s32)limit ? 1u : 0u;
}
