#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern void CM_THISCALL cm_fn_00684230(void *self);
extern void CM_THISCALL cm_fn_00687430(void *self, u32 local_value,
                                      u32 entry_value, u32 arg3, u32 arg4);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern void *CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_005e11e0(volatile u8 *self)
{
    volatile u8 local_area[0x204];
    void *allocation;

    *(volatile u16 *)(self + 0x3e) = 4;
    allocation = cm_fn_00944e46(0xecu);
    *(volatile u32 *)(self + 0xb1) = (u32)allocation;
    if (allocation == 0) {
        u32 error_text;
        cm_fn_00944cff(0x009cd640u, 0u, 0u,
                       (void *)&local_area[0x104], (void *)&local_area[4]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x104], (void *)&local_area[4],
                       0x1d4u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        s8 index = 0;
        s16 count = *(volatile s16 *)(self + 0x3e);
        local_area[0] = 0;
        if (count > 0) {
            for (;;) {
                u32 table = *(volatile u32 *)(self + 0xee);
                u32 entry = *(volatile u32 *)(table + (u32)(s32)index * 12u);
                u32 local_value = *(volatile u32 *)&local_area[0];
                cm_fn_00687430((void *)self, local_value, entry, 0u, 0u);
                index = (s8)((u8)index + 1u);
                count = *(volatile s16 *)(self + 0x3e);
                local_area[0] = (u8)index;
                if ((s16)index >= count) {
                    break;
                }
            }
        }
    }
    cm_fn_00684230((void *)self);
    return 1;
}
