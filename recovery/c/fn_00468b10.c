#include "cmre.h"
u32 CM_THISCALL cm_fn_00468b10(u32 self, u32 index_arg)
{
    u32 index;
    if ((u16)index_arg == 0xffffu) return 0;
    index = (u32)(s32)(s16)index_arg;
    return CM_GLOBAL(volatile u32, self + index * 396u + 0xba966u);
}
