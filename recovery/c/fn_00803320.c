#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004032e0(u32 self);

u32 CM_THISCALL cm_fn_00803320(u32 self, u32 index_arg)
{
    u32 group = (u32)(s32)*(volatile s16 *)(self + 0x382a);
    u32 entry = self + group * 768;
    s32 index;
    if (*(volatile u16 *)(entry + 0x7d4)) {
        if ((u16)index_arg == *(volatile u16 *)(entry + 0x7f6))
            return *(volatile u32 *)(entry + 0x7f8);
        return 0;
    }
    entry = *(volatile u32 *)(entry + 0x7fc);
    if ((u16)index_arg == 0xffff)
        return 0;
    index = (s16)index_arg;
    return cm_fn_004032e0(entry + (u32)index * 3057 + 4);
}
