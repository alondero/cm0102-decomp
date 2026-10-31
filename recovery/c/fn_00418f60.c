#include "cmre.h"

extern int CM_THISCALL cm_fn_004cf5a0(u32 *self, u32 *index);
extern u32 CM_THISCALL cm_fn_004196a0(u32 *self, u32 *index);

u8 CM_THISCALL cm_fn_00418f60(u8 *self, u32 *index_pointer, u32 second_arg)
{
    u32 index;
    u32 table;
    u32 context;
    u32 nested;
    u32 calculation;
    s32 maximum;
    u8 value;

    if (index_pointer == 0)
        return 0;
    index = *(volatile u32 *)index_pointer;
    maximum = CM_GLOBAL(volatile s32, 0x00ae2364);
    if ((s32)index >= maximum)
        return 0;

    if (second_arg != 0) {
        table = *(volatile u32 *)(self + 0x0c);
        value = *(volatile u8 *)(table + index);
        return (u8)(value & 1);
    }

    table = *(volatile u32 *)(self + 0x0c);
    value = *(volatile u8 *)(table + index);
    if ((value & 1) != 0) {
        context = CM_GLOBAL(volatile u32, 0x00ae1050);
        nested = (u32)cm_fn_004cf5a0((u32 *)context, index_pointer);
        CM_GLOBAL(volatile u32, 0x00ae1080) = nested;
        if (nested != 0) {
            u32 row = *(volatile u32 *)(nested + 4u);
            u32 base = CM_GLOBAL(volatile u32, 0x00ae23b4);
            calculation = base + row;
            calculation += (row * 145u) * 4u;
            if (calculation == 0)
                cm_fn_004196a0((u32 *)self, index_pointer);
        } else {
            cm_fn_004196a0((u32 *)self, index_pointer);
        }
    }

    table = *(volatile u32 *)(self + 0x0c);
    index = *(volatile u32 *)index_pointer;
    value = *(volatile u8 *)(table + index);
    return (u8)(value & 1);
}
