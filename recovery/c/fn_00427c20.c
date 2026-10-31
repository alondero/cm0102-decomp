#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                    u32 flags);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 arg2, u32 arg3,
                                   void *buffer1, void *buffer2);
extern void *CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_00427c20(void *self)
{
    u32 self_address = (u32)self;
    volatile u8 scratch[0x200];
    u32 allocation;
    s32 index = 0;

    *(volatile s16 *)(self_address + 0x3eu) = 0x47;
    allocation = (u32)cm_fn_00944e46(0x1aau);
    *(volatile u32 *)(self_address + 0xa7u) = allocation;
    if (allocation == 0u) {
        void *buffer0 = (void *)&scratch[0];
        void *buffer100 = (void *)&scratch[0x100];
        u32 formatted;

        (void)cm_fn_00944cff(0x0098a3bcu, 0u, 0u, buffer100, buffer0);
        formatted = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, formatted,
                             buffer100, buffer0, 0x143u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0xffffffffu;
    }

    while (index < (s32)*(volatile s16 *)(self_address + 0x3eu)) {
        u32 source_base = *(volatile u32 *)(self_address + 0x14u);
        u32 destination = *(volatile u32 *)(self_address + 0xa7u);
        u32 source = *(volatile u32 *)(source_base + (u32)index * 6u);

        *(volatile u32 *)(destination + (u32)index * 6u) = source;
        destination = *(volatile u32 *)(self_address + 0xa7u);
        *(volatile u8 *)(destination + (u32)index * 6u + 5u) = 0xffu;
        destination = *(volatile u32 *)(self_address + 0xa7u);
        *(volatile u8 *)(destination + (u32)index * 6u + 4u) = 0u;
        ++index;
    }
    return 1u;
}
