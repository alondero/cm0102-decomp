#include "cmre.h"

u8 CM_THISCALL cm_fn_0085fa20(u32 self)
{
    return (*(volatile u8 *)(self + 0x35u) & 0x3fu) == 1u;
}
