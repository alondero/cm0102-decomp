#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *text, const char *caption,
                                    s32 flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);

void CM_CDECL cm_fn_0090ba90(void *arg1, void *arg2, u32 count)
{
    u8 scratch[0x200];

    if (arg1 == 0) {
        (void)cm_fn_00944cff(0x00ad4134u, 0u, 0u,
                             &scratch[0x100], &scratch[0]);
        {
            u32 message = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                                 message, (u32)&scratch[0x100],
                                 (u32)&scratch[0], 0x16bu);
        }
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return;
    }

    if (arg2 != 0) {
        volatile u8 *first = (volatile u8 *)arg1;
        volatile u8 *second = (volatile u8 *)arg2;

        if (count != 0u) {
            do {
                u8 from_second = *second;
                u8 from_first = *first;
                *first = from_second;
                ++first;
                *second = from_first;
                ++second;
                --count;
            } while (count != 0u);
        }
        return;
    }

    (void)cm_fn_00944cff(0x00ad4134u, 0u, 0u,
                         &scratch[0], &scratch[0x100]);
    {
        u32 message = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             message, (u32)&scratch[0],
                             (u32)&scratch[0x100], 0x172u);
    }
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
}
