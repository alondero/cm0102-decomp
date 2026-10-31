#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

void CM_THISCALL cm_fn_0040a620(volatile u8 *self)
{
    volatile u8 local_area[0x200];
    u32 allocation;

    *(volatile u8 *)(self + 8u) = 2;
    allocation = cm_fn_00944e46(0x18u);
    *(volatile u32 *)(self + 4u) = allocation;
    if (allocation == 0) {
        u32 error_text;
        cm_fn_00944cff(0x00987960u, 0u, 0u,
                       (void *)&local_area[0x100], (void *)&local_area[0]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x100], (void *)&local_area[0],
                       0x50u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return;
    }

    *(volatile u8 *)(allocation + 1u) = 0;
    *(volatile u8 *)allocation = 1;
    *(volatile u8 *)(allocation + 2u) = 0;
    *(volatile u8 *)(allocation + 3u) = 0x18u;
    *(volatile u8 *)(allocation + 4u) = 6;
    *(volatile u8 *)(allocation + 5u) = 1;

    allocation = *(volatile u32 *)(self + 4u);
    *(volatile u8 *)(allocation + 6u) = 1;
    *(volatile u8 *)(allocation + 7u) = 0;
    *(volatile u8 *)(allocation + 8u) = 3;
    *(volatile u8 *)(allocation + 9u) = 6;
    *(volatile u8 *)(allocation + 10u) = 7;
    *(volatile u8 *)(allocation + 11u) = 0;

    allocation = *(volatile u32 *)(self + 4u);
    *(volatile u8 *)(allocation + 12u) = 1;
    *(volatile u8 *)(allocation + 13u) = 1;
    *(volatile u8 *)(allocation + 14u) = 0;
    *(volatile u8 *)(allocation + 15u) = 0x0eu;
    *(volatile u8 *)(allocation + 16u) = 0x0bu;
    *(volatile u8 *)(allocation + 17u) = 1;

    allocation = *(volatile u32 *)(self + 4u);
    allocation += 0x12u;
    *(volatile u8 *)(allocation + 5u) = 0;
    *(volatile u8 *)allocation = 1;
    *(volatile u8 *)(allocation + 1u) = 1;
    *(volatile u8 *)(allocation + 2u) = 3;
    *(volatile u8 *)(allocation + 3u) = 4;
    *(volatile u8 *)(allocation + 4u) = 2;
}
