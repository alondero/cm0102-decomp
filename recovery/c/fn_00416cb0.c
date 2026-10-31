#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u8 *self, u32 first, u32 second);
extern unsigned long long CM_THISCALL cm_fn_00785830(u8 *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(u32 *self, u32 first, u32 second,
                                      u32 output);
extern void CM_CDECL cm_fn_00416d50(u32 value);

void CM_CDECL cm_fn_00416cb0(u32 first_parameter, u32 second_parameter)
{
    struct {
        u32 previous;
        u32 handler;
        u32 state;
    } volatile frame;
    u8 local_area[0xe0];
    u32 second_value;
    u32 first_value;
    u32 row_index;
    u32 column_index;
    u32 table;
    u32 row;
    u32 result;

    frame.state = 0xffffffffu;
    frame.handler = 0x00953dabu;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    (void)cm_fn_0077bbe0(local_area, 0, 0);
    second_value = second_parameter;
    first_value = first_parameter;
    frame.state = 0;
    (void)cm_fn_00786b70((u32 *)CM_GLOBAL(volatile u32, 0x00b7431cu),
                          first_value, second_value, (u32)local_area);
    row_index = (u32)cm_fn_00785830(local_area, 0);
    column_index = (u32)cm_fn_00785830(local_area, 1);

    table = CM_GLOBAL(volatile u32, 0x00ad9c60u);
    table = *(volatile u32 *)(table + 8u);
    row = *(volatile u32 *)(table + row_index * 4u);
    row = *(volatile u32 *)(row + 4u);
    result = *(volatile u32 *)(row + column_index * 4u);
    cm_fn_00416d50(result);

    CM_FS(u32, 0) = frame.previous;
}
