#include "cmre.h"

extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);
extern void cm_fn_005e8290(const char *title, const char *message, int flags);

void CM_CDECL cm_fn_0060d780(u32 path_index)
{
    volatile u8 scratch[0x200];
    (void)cm_fn_00944c9f(0x00de1f64u, 0x009d93f4u, path_index);
    (void)cm_fn_00944cff(0x009d93ccu, 0, 0,
                         (void *)&scratch[0x100], (void *)&scratch[0]);
    char *converted = cm_fn_0090d130();
    (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, converted,
                         (u32)&scratch[0x100], (u32)&scratch[0], 0x16b1u);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
}
