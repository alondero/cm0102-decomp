#include "cmre.h"

u32 CM_CDECL cm_fn_009417f0(u32 object)
{
    u32 base = *(volatile u32 *)object;
    u32 result = 0;
    u32 shift = 0;
    u8 byte;
    do {
        u32 offset = *(volatile u32 *)(object + 4);
        u32 current_shift = shift;
        shift += 7;
        byte = *(volatile u8 *)(base + offset);
        *(volatile u32 *)(object + 4) = offset + 1;
        result |= ((u32)byte & 0x7fu) << (current_shift & 31u);
    } while (byte & 0x80u);
    return result;
}
