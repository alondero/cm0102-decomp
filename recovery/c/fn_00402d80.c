#include "cmre.h"

extern void CM_CDECL cm_fn_005e3fd0(u32 self, u32 arg1, u32 arg2);
extern void CM_CDECL cm_fn_005e42b0(u32 pointer);
extern void CM_CDECL cm_fn_005e2eb0(u32 self, u32 arg1, u32 arg2, u32 arg3);

void CM_THISCALL cm_fn_00402d80(u32 self)
{
    u16 index;
    u32 table_base;
    u32 saved_200;

    if (*(volatile u32 *)(self + 0xbe9u) == 0u)
        return;

    index = *(volatile u16 *)(self + 0xb76u);
    if ((s16)index == -1 || (s16)index >= *(volatile s16 *)(self + 0xb74u))
        goto after_rows;

    do {
        u32 row_slot = self + 0x212u + (u32)(s32)(s16)index * 2u;
        s16 row = *(volatile s16 *)row_slot;
        u32 scaled = (u32)(s32)row * 396u;
        table_base = *(volatile u32 *)(self + 0x208u);
        *(volatile u32 *)(table_base + scaled + 0x188u) = 0u;

        table_base = *(volatile u32 *)(self + 0x208u);
        row = *(volatile s16 *)row_slot;
        {
            u32 nested_offset = (u32)(s32)row * 396u;
            s16 recursive_index = *(volatile s16 *)(table_base + nested_offset + 0x7cu);
            if (recursive_index != -1) {
                u32 child_offset = (u32)(s32)recursive_index * 3057u;
                u32 child_base = *(volatile u32 *)(self + 0x204u);
                cm_fn_00402d80(child_base + child_offset);
            }
        }

        index = (u16)(index + 1u);
    } while ((s16)index < *(volatile s16 *)(self + 0xb74u));

after_rows:
    saved_200 = *(volatile u32 *)(self + 0x200u);
    *(volatile u32 *)(self + 0xbe9u) = 0u;
    if (saved_200 == 0u)
        return;

    {
        u32 first = *(volatile u32 *)self;
        u32 second = *(volatile u32 *)(self + 4u);
        cm_fn_005e3fd0(first, second, saved_200);
    }
    saved_200 = *(volatile u32 *)(self + 0x200u);
    cm_fn_005e42b0(saved_200);

    {
        u32 arg3 = *(volatile u32 *)(self + 0xcu);
        u32 arg2 = *(volatile u32 *)(self + 8u);
        u32 arg1 = *(volatile u32 *)(self + 4u);
        u32 receiver = *(volatile u32 *)self;
        *(volatile u32 *)(self + 0x200u) = 0u;
        cm_fn_005e2eb0(receiver, arg1, arg2, arg3);
    }
}
