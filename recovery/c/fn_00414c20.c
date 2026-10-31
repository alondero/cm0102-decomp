#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00414a60(u32 self, u32 value);
extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                     u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2,
                                       u32 arg3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *local100, void *local0);

void CM_CDECL cm_fn_00414c20(u32 value)
{
    u8 scratch[0x200];
    u32 argument = value;
    if (argument == 0) {
        (void)cm_fn_00944cff(0x00987fa4u, 0, 0,
                             scratch + 0x100, scratch);
        u32 message = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, message,
                             scratch + 0x100, scratch, 0x5au);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, argument);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = argument;
        return;
    }

    u32 result = cm_fn_007fe600(0x00b74450u, 0x00414cf0u, 0x00416960u,
                                0, 0, 0);
    if (result != 0) {
        u32 lookup = CM_GLOBAL(volatile u32, 0x00ad9c60u);
        u32 selected = cm_fn_00414a60(lookup, argument);
        cm_fn_007ff270(0x00b74450u, 0, selected, 0);
        cm_fn_007ff270(0x00b74450u, 1, 0, 0);
    }
}
