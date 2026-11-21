#include "cmre.h"

extern u32 CM_CDECL cm_fn_007615d0(u32);
extern u32 CM_CDECL cm_fn_007643d0(u32);
extern u32 CM_THISCALL cm_fn_007ff010(u32, u32);
extern void CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00803320(u32, u32);

static u32 field(s16 index, u32 offset)
{
    u32 table = CM_GLOBAL(volatile u32, 0x00b74c4c + (u32)(s32)CM_GLOBAL(volatile s16, 0x00b77c7a) * 768u);
    return CM_GLOBAL(volatile u32, table + offset + (u32)(s32)index * 396u);
}

u32 CM_CDECL cm_fn_0049bef0(u32 arg)
{
    s16 index = (s16)arg;
    u32 value, other, slot;
    if (index != -1) {
        value = cm_fn_007ff010(0x00b74450, 4);
        value = cm_fn_00803320(0x00b74450, value);
        cm_fn_007ff270(0x00b74450, 3, value, 0);
        value = field(index, 0xba966);
    } else {
        value = 0;
    }
    if ((u16)value == 0)
        value = (value & 0xffff0000u) | (u16)CM_GLOBAL(volatile u16, 0x00dd6bde);
    if ((u16)value != 1) {
        value = cm_fn_007615d0(arg);
        if (!value)
            value = cm_fn_007643d0(arg);
        return value;
    }
    other = index == -1 ? 0 : field(index, 0xba9a6);
    value = cm_fn_007ff010(0x00b74450, 1);
    if (other == value) {
        slot = 1;
        other = 0;
    } else {
        other = index == -1 ? 0 : field(index, 0xba9a6);
        value = cm_fn_007ff010(0x00b74450, 2);
        if (other == value) {
            slot = 2;
            other = 0;
        } else {
            value = cm_fn_007ff010(0x00b74450, 1);
            slot = value == 0 ? 1 : 2;
            other = index == -1 ? 0 : field(index, 0xba9a6);
        }
    }
    cm_fn_007ff270(0x00b74450, slot, other, 0);
    return 0xfffffff5u;
}
