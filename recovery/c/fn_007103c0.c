#include "cmre.h"

u32 CM_THISCALL cm_fn_007103c0(u32 self)
{
    u32 pointer = *(volatile u32 *)(self + 0x19eu);
    u32 other = *(volatile u32 *)(pointer + 0xf582u);
    return (pointer & 0xffffff00u) | (self == other);
}
