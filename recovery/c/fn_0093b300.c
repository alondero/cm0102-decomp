#include "cmre.h"

extern u32 CM_CDECL cm_fn_009415c0(u32 self);

u32 CM_CDECL cm_fn_0093b300(u32 output, u32 input)
{
    u32 cursor = output;
    u32 remaining = 10;
    u32 value = cm_fn_009415c0(input);
    do {
        value = cm_fn_009415c0(input);
        *(volatile u16 *)cursor = (u16)value;
        value = cm_fn_009415c0(input);
        *(volatile u16 *)(cursor + 4) = (u16)value;
        *(volatile u16 *)(cursor + 2) = (u16)(*(volatile u16 *)cursor + (u16)value);
        cursor += 6;
        --remaining;
    } while (remaining != 0);
    cursor = output + 0x54;
    remaining = 12;
    do {
        value = cm_fn_009415c0(input);
        *(volatile u16 *)(cursor - 0x18) = (u16)value;
        value = cm_fn_009415c0(input);
        *(volatile u16 *)cursor = (u16)value;
        cursor += 2;
        --remaining;
    } while (remaining != 0);
    return value;
}
