#include "cmre.h"

extern void CM_CDECL cm_fn_00549c90(u32 input);
extern u32 CM_THISCALL cm_fn_00867240(u32 self, u32 option);

u32 CM_THISCALL cm_fn_00867520(volatile u8 *self, volatile u32 option)
{
    u32 index = *(volatile u32 *)self;
    u32 table;
    *(volatile u32 *)(self + 0x45) = 0;
    table = CM_GLOBAL(volatile u32, 0x00ae23bc);
    cm_fn_00549c90(table + index * 110u);
    if (option)
        return cm_fn_00867240((u32)self, 0);
    return 0;
}
