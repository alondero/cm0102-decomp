#include "cmre.h"

u32 CM_THISCALL cm_fn_00487810(u32 self)
{
    *(volatile u32 *)self = 0x41u;
    *(volatile u32 *)(self + 4u) = 8u;
    *(volatile u8 *)(self + 12u) = 6u;
    *(volatile u32 *)(self + 8u) = 0x154a21u;
    *(volatile u8 *)(self + 13u) = 0xffu;
    *(volatile u8 *)(self + 14u) = 0xffu;
    return self;
}
