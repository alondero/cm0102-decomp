#include "cmre.h"

u32 CM_THISCALL cm_fn_008c5430(u32 self, u32 entry)
{
    u32 index = *(volatile u32 *)entry;
    u32 base = *(volatile u32 *)(self + 0x86eu);
    u32 address = base + index * 8u;
    u8 value = *(volatile u8 *)address;
    *(volatile u8 *)address = value & 0xbfu;
    return address;
}
