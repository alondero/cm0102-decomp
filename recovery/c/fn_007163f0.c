/* Preferred-image VA 0x007163f0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_THISCALL cm_fn_005b5270(u32 self, u32 index);
u32 CM_THISCALL cm_fn_007163f0(u32 self)
{
    u32 group = (u32)(s32)*(volatile s8 *)(self + 0x27u);
    u32 base = *(volatile u32 *)(self + 0x19eu);
    u32 record = base + group * 0x18e3u;
    u32 argument, result;
    if (*(volatile u32 *)(record + 0x9766u) & 0x1000u)
        return (group & 0xffffff00u) | *(volatile u8 *)(base + group + 6u);
    argument = (group & 0xffffff00u) | *(volatile u8 *)(self + 0x19u);
    result = (u32)cm_fn_005b5270(record + 0x91e2u, argument) & 0x800000u;
    return result ? result | 1u : 0u;
}
