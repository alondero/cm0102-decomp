#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);

typedef struct {
    u32 prev;
    u32 handler;
    u32 state;
} cm_fn_006aa7d0_seh;

void CM_STDCALL cm_fn_006aa7d0(volatile u8 *record, u32 argument, s8 byte_argument)
{
    volatile cm_fn_006aa7d0_seh frame;
    volatile u8 local_area[0xe8];
    u32 entry_prev = CM_FS(u32, 0);

    frame.state = 0xffffffffu;
    frame.handler = 0x0095cfdb;
    frame.prev = entry_prev;
    CM_FS(u32, 0) = (u32)&frame;

    if (record != 0) {
        u32 high_dword = CM_GLOBAL(volatile u32, 0x00dd6ec8);
        u32 low_dword = CM_GLOBAL(volatile u32, 0x00dd6ecc);
        u16 upper_limit = CM_GLOBAL(volatile u16, 0x00ae2c92);

        *(volatile u32 *)(local_area + 0) = high_dword;
        *(volatile u32 *)(local_area + 4) = low_dword;
        {
            u16 current_upper = *(volatile u16 *)(local_area + 2);
            if (current_upper <= upper_limit &&
                (current_upper < upper_limit ||
                 (s16)high_dword <= (s16)CM_GLOBAL(volatile u16, 0x00ae2c90))) {
                (void)cm_fn_0077bbe0((void *)(local_area + 8), 0x29, 0);

                {
                    u32 nested = *(volatile u32 *)(record + 0x1c);
                    frame.state = 0;
                    u32 value = *(volatile u32 *)nested;
                    (void)cm_fn_00785790((void *)(local_area + 8), 0, value);
                }
                {
                    u32 nested = *(volatile u32 *)(record + 0x20);
                    u32 value = *(volatile u32 *)nested;
                    (void)cm_fn_00785790((void *)(local_area + 8), 1, value);
                }
                {
                    u32 nested = *(volatile u32 *)(record + 0x14);
                    u32 value = *(volatile u32 *)nested;
                    (void)cm_fn_00785790((void *)(local_area + 8), 2, value);
                }
                (void)cm_fn_00785790((void *)(local_area + 8), 3, argument);
                {
                    u32 nested = *(volatile u32 *)(record + 0x14);
                    u32 child = *(volatile u32 *)(nested + 0x5d);
                    u32 value = *(volatile u32 *)child;
                    (void)cm_fn_00785790((void *)(local_area + 8), 4, value);
                }
                (void)cm_fn_00785790((void *)(local_area + 8), 5,
                                     (u32)(s32)byte_argument);

                {
                    u32 nested = *(volatile u32 *)(record + 0x1c);
                    u32 global_object = CM_GLOBAL(volatile u32, 0x00b7431c);
                    (void)cm_fn_007861e0((void *)(u32)global_object,
                                         (u32)(local_area + 8), nested);
                }
                {
                    u32 nested = *(volatile u32 *)(record + 0x20);
                    u32 global_object = CM_GLOBAL(volatile u32, 0x00b7431c);
                    (void)cm_fn_007861e0((void *)(u32)global_object,
                                         (u32)(local_area + 8), nested);
                }
            }
        }
    }

    {
        u32 restore_prev = frame.prev;
        CM_FS(u32, 0) = restore_prev;
    }
}
