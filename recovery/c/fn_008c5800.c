#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);

u32 CM_STDCALL cm_fn_008c5800(u32 person, u32 type)
{
    u32 found, club, index, base, result, low;
    found = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), person);
    CM_GLOBAL(volatile u32, 0x00ae1080) = found;
    if (found) {
        index = *(volatile u32 *)(found + 4);
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        club = base + index * 581;
    } else {
        club = 0;
    }
    result = cm_fn_004cf5f0(CM_GLOBAL(volatile u32, 0x00ae1050), person, club);
    if (!result)
        return 0;
    low = *(volatile u8 *)(result + 0x4f) >> 4;
    if ((s16)type == 0x200)
        low = 1;
    else if ((s16)type == 0x400)
        low = 2;
    else if ((s16)type == 0x2000)
        low = 5;
    return (result & 0xffffff00u) | low;
}
