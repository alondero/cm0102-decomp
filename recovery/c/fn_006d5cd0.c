#include "cmre.h"

u32 CM_STDCALL cm_fn_006d5cd0(u32 arg)
{
    u32 raw = (arg & 0x7fffu) - 0x1f40u;
    s32 index = (s16)raw;
    if (index < 0 || index >= 0x2a5)
        return 0;
    return (u32)(s32)*(volatile s8 *)(0x00b68788u + (u32)index * 18u);
}
