#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

void CM_THISCALL cm_fn_005ecf20(volatile u8 *self)
{
    volatile u8 local_area[0x200];
    volatile u8 *buffer0 = &local_area[0];
    volatile u8 *buffer100 = &local_area[0x100];
    u32 allocation;

    *(volatile u8 *)(self + 8) = 2;
    allocation = cm_fn_00944e46(0x18u);
    *(volatile u32 *)(self + 4) = allocation;
    if (allocation == 0) {
        u32 error_text;
        cm_fn_00944cff(0x009cdbc8u, 0u, 0u,
                       (void *)buffer100, (void *)buffer0);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)buffer100, (void *)buffer0, 0x51u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return;
    }

    *(volatile u8 *)(allocation + 0u) = 0x0b;
    *(volatile u8 *)(allocation + 1u) = 0;
    *(volatile u8 *)(allocation + 2u) = 0;
    *(volatile u8 *)(allocation + 3u) = 0x16;
    *(volatile u8 *)(allocation + 4u) = 4;
    *(volatile u8 *)(allocation + 5u) = 1;
    {
        u32 row = *(volatile u32 *)(self + 4);
        row += 6u;
        *(volatile u8 *)(row + 0u) = 0x0b;
        *(volatile u8 *)(row + 1u) = 0;
        *(volatile u8 *)(row + 2u) = 6;
        *(volatile u8 *)(row + 3u) = 0x1d;
        *(volatile u8 *)(row + 4u) = 7;
        *(volatile u8 *)(row + 5u) = 0;
    }
    {
        u32 row = *(volatile u32 *)(self + 4);
        row += 0x0cu;
        *(volatile u8 *)(row + 0u) = 0x0b;
        *(volatile u8 *)(row + 1u) = 1;
        *(volatile u8 *)(row + 2u) = 0xff;
        *(volatile u8 *)(row + 3u) = 1;
        *(volatile u8 *)(row + 4u) = 0;
        *(volatile u8 *)(row + 5u) = 1;
    }
    {
        u32 row = *(volatile u32 *)(self + 4);
        row += 0x12u;
        *(volatile u8 *)(row + 4u) = 0;
        *(volatile u8 *)(row + 5u) = 0;
        *(volatile u8 *)(row + 0u) = 0x0b;
        *(volatile u8 *)(row + 1u) = 1;
        *(volatile u8 *)(row + 2u) = 0xff;
        *(volatile u8 *)(row + 3u) = 0x14;
    }
}
