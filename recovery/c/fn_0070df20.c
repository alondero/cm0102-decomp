#include "cmre.h"

u8 CM_THISCALL cm_fn_0070df20(u32 self)
{
    s8 value = *(volatile s8 *)(self + 0x102u);
    return value < 2 || value > 6;
}
