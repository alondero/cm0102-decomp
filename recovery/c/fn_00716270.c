#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_005b5270(u32 address, s8 index);
extern u16 CM_THISCALL cm_fn_006c38c0(u32 table, u32 object);

u8 CM_THISCALL cm_fn_00716270(u8 *self)
{
    s8 index = *(volatile s8 *)(self + 0x19u);
    s8 field = *(volatile s8 *)(self + 0x27);
    u32 x = (u32)(s32)field;
    u32 five_x = x + x * 4u;
    u32 scaled = five_x * 32u - x;
    u32 six_hundred_thirty_seven_x = x + scaled * 4u;
    u32 address_offset = x + (six_hundred_thirty_seven_x * 5u) * 2u;
    u32 table = *(volatile u32 *)(self + 0x19eu);
    u32 address = address_offset + table + 0x91e2u;
    unsigned long long first = cm_fn_005b5270(address, index);
    if (((u32)first & 0x00020000u) != 0)
        return 1;

    table = *(volatile u32 *)(self + 0x19eu);
    if (table != 0) {
        u16 value = cm_fn_006c38c0(table, (u32)self);
        if (value == 1)
            return 0;
    }

    field = *(volatile s8 *)(self + 0x27);
    index = *(volatile s8 *)(self + 0x19u);
    x = (u32)(s32)field;
    five_x = x + x * 4u;
    scaled = five_x * 32u - x;
    six_hundred_thirty_seven_x = x + scaled * 4u;
    address_offset = x + (six_hundred_thirty_seven_x * 5u) * 2u;
    table = *(volatile u32 *)(self + 0x19eu);
    address = address_offset + table + 0x91e2u;
    unsigned long long second = cm_fn_005b5270(address, index);
    return ((u32)second & 0x40000000u) != 0 ? 2u : 0u;
}
