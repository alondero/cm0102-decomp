#include "cmre.h"

extern void cm_fn_005e8290(const char *title, const char *message, int arg);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3,
                                   void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_005b3460(volatile u8 *self, s8 selected,
                              volatile u32 *output)
{
    volatile u8 local_area[0x200];

    if (selected < 0 || selected > 10) {
        volatile u8 *buffer0 = &local_area[0];
        volatile u8 *buffer100 = &local_area[0x100];
        u32 error_text;
        cm_fn_00944cff(0x009cc3dcu, 0u, 0u,
                       (void *)buffer100, (void *)buffer0);
        error_text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, error_text,
                       (void *)buffer100, (void *)buffer0, 0x27bu);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    *output = 0;
    {
        s32 index = 0;
        while (index < 11) {
            if (index != (s32)selected) {
                s16 current = *(volatile s16 *)(self + 0x12du +
                                                (u32)index * 2u);
                s16 chosen = *(volatile s16 *)(self + 0x12du +
                                                (u32)(u8)selected * 2u);
                if (current == chosen) {
                    s8 current_rank = *(volatile s8 *)(self + 0x143u +
                                                       (u32)index);
                    s8 chosen_rank = *(volatile s8 *)(self + 0x143u +
                                                       (u32)(u8)selected);
                    if (current_rank < chosen_rank) {
                        *output = 1;
                        return 1;
                    }
                    return 0;
                }
            }
            index++;
        }
    }
    return 1;
}
