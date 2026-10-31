#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00456770(u8 *self, u32 arg1, u32 arg2);
extern u32 CM_THISCALL cm_fn_00456740(u8 *self, u32 arg1, u32 arg2);
extern s32 CM_CDECL cm_fn_00540270(u32 *first, u32 *second);

s32 CM_CDECL cm_fn_008e55f0(u8 *first, u8 *second)
{
    s32 second_kind = *(volatile s8 *)(second + 0x19);
    s32 first_kind = *(volatile s8 *)(first + 0x19);
    s32 difference = first_kind - second_kind;
    if (difference != 0)
        return difference;

    u32 second_arg2 = *(volatile u32 *)(second + 4);
    u32 second_arg1 = *(volatile u32 *)second;
    if (cm_fn_00456770(first, second_arg1, second_arg2) != 0)
        return -1;

    second_arg2 = *(volatile u32 *)(second + 4);
    second_arg1 = *(volatile u32 *)second;
    if (cm_fn_00456740(first, second_arg1, second_arg2) != 0)
        return 1;

    u32 first_index = *(volatile u32 *)(first + 8);
    u32 scaled_first = first_index * 0x6eu;
    u32 base = CM_GLOBAL(volatile u32, 0x00ae23bcu);
    u32 first_record = base + scaled_first;

    u32 second_index = *(volatile u32 *)(second + 8);
    u32 second_record = base + second_index * 0x6eu;
    return cm_fn_00540270(&first_record, &second_record);
}
