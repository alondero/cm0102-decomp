#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *format, const char *destination, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);

u32 CM_CDECL cm_fn_0048dcd0(u8 sought)
{
    u32 index = 0;
    u8 scratch[0x200];

    while ((s32)index < 0x23) {
        u8 value = CM_GLOBAL(volatile u8, 0x00b63d74u + (index * 0x48u));
        if (value == sought)
            return index;
        index++;
    }

    (void)cm_fn_00944cff(0x0099b80cu, 0, 0, scratch + 0x100, scratch);
    {
        u32 result = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             result, scratch + 0x100, scratch, 0x543u);
    }
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
    return 3;
}
