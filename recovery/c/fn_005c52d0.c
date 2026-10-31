#include "cmre.h"

u32 CM_STDCALL cm_fn_005c52d0(u32 object)
{
    s16 value = *(volatile s16 *)(object + 0x80);
    u32 upper = (object & 0xffff0000u) | ((u16)value & 0xff00u);
    if (value >= 8000)
        return upper | 5u;
    if (value >= 6000)
        return upper | 4u;
    if (value >= 4000)
        return upper | 3u;
    return upper | (value >= 2000 ? 2u : 1u);
}
