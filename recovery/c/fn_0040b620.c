#include "cmre.h"

extern void *CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_005e8290(const char *arg1, const char *arg2, s32 arg3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                    void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_0040b620(void *self)
{
    volatile u8 scratch[0x200];
    u32 base = (u32)&scratch[0];
    u32 self_address = (u32)self;
    u32 allocation;

    *(volatile u16 *)(self_address + 0x3eu) = 0x10u;
    allocation = (u32)cm_fn_00944e46(0x60u);
    *(volatile u32 *)(self_address + 0xa7u) = allocation;
    if (allocation == 0u) {
        (void)cm_fn_00944cff(0x00987a40u, 0u, 0u,
                             (void *)(base + 0x100u), (void *)base);
        {
            char *text = cm_fn_0090d130();
            (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                                 base + 0x100u, base, 0x1cdu);
        }
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0xffffffffu;
    }

    {
        s16 count = *(volatile s16 *)(self_address + 0x3eu);
        if (count > 0) {
            s32 index = 0;
            do {
                u32 source = *(volatile u32 *)(self_address + 0xb2u);
                u32 output = *(volatile u32 *)(self_address + 0xa7u);
                u32 source_index = (u32)(s32)(s16)index;
                u32 row = *(volatile u32 *)(source + source_index * 4u);
                u32 output_slot = output + (u32)index * 6u;
                *(volatile u32 *)output_slot = row;
                output = *(volatile u32 *)(self_address + 0xa7u);
                *(volatile u8 *)(output + (u32)index * 6u + 4u) = 0u;
                output = *(volatile u32 *)(self_address + 0xa7u);
                *(volatile u8 *)(output + (u32)index * 6u + 5u) = 0u;
                ++index;
                count = *(volatile s16 *)(self_address + 0x3eu);
            } while (index < (s32)count);
        }
    }
    return 1u;
}
