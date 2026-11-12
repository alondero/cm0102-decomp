#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);

u32 CM_CDECL cm_fn_005444a0(u32 person, u32 input_club)
{
    u32 result, index, base;
    if (!person)
        return 0;
    /* Original derived club stays in ESI; preserve the incoming argument home. */
    volatile u32 club = input_club;
    if (!club) {
        result = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), person);
        CM_GLOBAL(volatile u32, 0x00ae1080) = result;
        if (!result)
            return 0;
        index = *(volatile u32 *)(result + 4);
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        club = base + index * 581;
        if (!club)
            return 0;
    }
    result = cm_fn_004cf5f0(CM_GLOBAL(volatile u32, 0x00ae1050), person, club);
    if (!result || !(*(volatile u8 *)(result + 0x4e) & 1))
        return 0;
    if (!cm_fn_00600a50(club, 1, 1, 0, 0) && *(volatile u8 *)(person + 0x3d) == 15)
        return 0;
    return 1;
}
