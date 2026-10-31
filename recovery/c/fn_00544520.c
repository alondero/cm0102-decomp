#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32, u32, u32);

u32 CM_CDECL cm_fn_00544520(u32 person, u32 club)
{
    u32 object, index, base;
    if (!person) return 0;
    if (!club) {
        object = cm_fn_004cf5a0(D(0x00ae1050), person);
        D(0x00ae1080) = object;
        if (!object) return 0;
        index = D(object + 4);
        base = D(0x00ae23b4);
        club = base + index * 581u;
        if (!club) return 0;
    }
    object = cm_fn_004cf5f0(D(0x00ae1050), person, club);
    if (!object) return 0;
    return (CM_GLOBAL(volatile u8, object + 0x4e) & 9) != 0;
}
