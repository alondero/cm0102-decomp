#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_00785d40(void *self, u32 arg1,
                                                       u32 arg2, u32 arg3);

struct cm_seh_00795e20 { void *prev; u32 handler; u32 state; };

void CM_STDCALL cm_fn_00795e20(s32 param1, volatile s32 *param2)
{
    volatile u8 local[0xe0];
    volatile struct cm_seh_00795e20 frame;
    s32 index;
    s32 limit;
    u32 table;
    u32 row_index;
    u32 target_row;
    u32 row;
    u32 pointer;
    u32 value;
    s8 parameter_byte;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095ea3bu;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    cm_fn_0077bbe0((void *)local, 0x1393, 0);
    frame.state = 0;
    if (param1 < 0 || param1 >= 0x10 || param2 == 0)
        goto cleanup;

    index = *param2;
    if (index < 0)
        goto cleanup;
    limit = CM_GLOBAL(volatile s32, 0x00ae2364);
    if ((s32)(u32)index > (s32)((u32)limit - 1u))
        goto cleanup;

    table = CM_GLOBAL(volatile u32, 0x00ae23bc);
    row_index = ((u32)limit - (u32)param1 - 1u) * 0x6eu;
    target_row = table + row_index;
    cm_fn_00785790((void *)local, 0, (u32)index);
    parameter_byte = *(volatile s8 *)((volatile u8 *)param2 + 0x113);
    cm_fn_00785790((void *)local, 1, (u32)(s32)parameter_byte);

    index = *param2;
    table = CM_GLOBAL(volatile u32, 0x00ae23bc);
    row = table + (u32)index * 0x6eu;
    pointer = *(volatile u32 *)(row + 4);
    value = *(volatile u32 *)(pointer + 0x33);
    cm_fn_00785790((void *)local, 2, value);

    pointer = *(volatile u32 *)(row + 8);
    value = *(volatile u32 *)(pointer + 0x33);
    cm_fn_00785790((void *)local, 3, value);

    pointer = *(volatile u32 *)(row + 0x0c);
    value = *(volatile u32 *)(pointer + 0x33);
    cm_fn_00785790((void *)local, 4, value);

    cm_fn_00785d40((void *)CM_GLOBAL(volatile u32, 0x00b7431c),
                   (u32)local, target_row, 0);

cleanup:
    CM_FS(void *, 0) = frame.prev;
}
