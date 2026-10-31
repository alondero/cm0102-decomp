#include "cmre.h"

extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                   void *buffer1, void *buffer2);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                   s32 mode);

u32 CM_CDECL cm_fn_00669ae0(const volatile u32 *value)
{
    volatile u8 scratch[0x200];
    u32 candidate;

    if (value == 0) {
        (void)cm_fn_00944cff(0x009eaff4u, 0u, 0u,
                             (void *)(scratch + 0x100u), (void *)scratch);
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             (u32)(unsigned long)cm_fn_0090d130(),
                             (void *)(scratch + 0x100u), (void *)scratch,
                             0x3efu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0u;
    }

    candidate = *value;
    if (candidate == CM_GLOBAL(volatile u32, 0x009cf7a0) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf7a4) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf784) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf7ac) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf7a8) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf794) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf798) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf8a0) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf964) ||
        candidate == CM_GLOBAL(volatile u32, 0x009cf790))
        return 1u;
    return 0u;
}
