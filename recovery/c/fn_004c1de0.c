#include "cmre.h"

extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                    u32 flags);
extern void CM_THISCALL cm_fn_00684230(void *self);
extern void CM_THISCALL cm_fn_00687430(void *self, u32 local_value,
                                      u32 entry_value, u32 arg3, u32 arg4);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 arg2, u32 arg3,
                                   void *buffer1, void *buffer2);
extern void *CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_004c1de0(void *self)
{
    u32 self_address = (u32)self;
    volatile u8 local_area[0x204];
    u32 allocation;
    u8 index = 0u;

    *(volatile s16 *)(self_address + 0x3eu) = 4;
    allocation = (u32)cm_fn_00944e46(0xecu);
    *(volatile u32 *)(self_address + 0xb1u) = allocation;
    if (allocation == 0u) {
        u32 formatted;
        (void)cm_fn_00944cff(0x009a7d6cu, 0u, 0u,
                             (void *)&local_area[0x104],
                             (void *)&local_area[4]);
        formatted = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, formatted,
                             (void *)&local_area[0x104],
                             (void *)&local_area[4], 0x145u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0u;
    }

    {
        s16 limit = *(volatile s16 *)(self_address + 0x3eu);
        local_area[0] = index;
        while ((s32)(s8)index < (s32)limit) {
        u32 table = *(volatile u32 *)(self_address + 0xeeu);
        u32 entry = *(volatile u32 *)(table + (u32)(s32)(s8)index * 4u);
        u32 local_value = *(volatile u32 *)&local_area[0];

        cm_fn_00687430(self, local_value, entry, 0u, 0u);
        index = (u8)(index + 1u);
        limit = *(volatile s16 *)(self_address + 0x3eu);
        local_area[0] = index;
        }
    }
    cm_fn_00684230(self);
    return 1u;
}
