#include "cmre.h"

u32 CM_THISCALL cm_fn_007ffd80(u32 self, u32 output, u32 count_ptr, u32 index_arg)
{
    u32 index = (u32)(s32)(s16)index_arg;
    s16 count = *(volatile s16 *)(self + index * 2 + 0x380a);
    u32 i = 0;
    *(volatile u16 *)count_ptr = (u16)count;
    if (count > 0) {
        do {
            u32 pos = (u32)(s32)(s16)i;
            u32 value = *(volatile u32 *)(self + (index * 16 + pos) * 48 + 0x7d6);
            ++i;
            *(volatile u32 *)(output + pos * 4) = value;
        } while ((s16)i < *(volatile s16 *)count_ptr);
    }
    return i;
}
