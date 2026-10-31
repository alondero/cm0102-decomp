#include "cmre.h"

extern void CM_FASTCALL cm_fn_00684230(u32 self);
extern void CM_THISCALL cm_fn_00687430(u32 self, u8 arg1, u32 arg2,
                                      u32 arg3, u32 arg4);
extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_THISCALL cm_fn_00426130(volatile u8 *self)
{
    volatile u8 local_area[0x204];
    u32 allocation;

    *(volatile u16 *)(self + 0x3eu) = 3;
    allocation = cm_fn_00944e46(0xb1u);
    *(volatile u32 *)(self + 0xb1u) = allocation;
    if (allocation == 0) {
        u32 error_text;
        cm_fn_00944cff(0x0098a1e8u, 0u, 0u,
                       (void *)&local_area[0x104], (void *)&local_area[4]);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)&local_area[0x104], (void *)&local_area[4],
                       0x195u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        s16 initial_count = *(volatile s16 *)(self + 0x3eu);
        local_area[0] = 0;
        if (initial_count > 0) {
        u8 index = 0;
        do {
            u32 array = *(volatile u32 *)(self + 0x14u);
            u8 value = local_area[0];
            u32 offset = (u32)(s32)(s8)index * 6u;
            u32 row_value = *(volatile u32 *)(array + offset);
            cm_fn_00687430((u32)self, (u8)value, row_value, 0u, 0u);
            array = *(volatile u32 *)(self + 0x14u);
            index = (u8)(index + 1u);
            *(volatile u32 *)(array + offset) = 0;
            {
                s16 live_count = *(volatile s16 *)(self + 0x3eu);
                u8 keep_going = (s16)(s8)index < live_count;
                local_area[0] = index;
                if (!keep_going) {
                    break;
                }
            }
        } while (1);
        }
    }
    cm_fn_00684230((u32)self);
    return 1;
}
