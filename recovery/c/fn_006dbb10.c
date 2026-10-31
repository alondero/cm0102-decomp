#include "cmre.h"
extern u32 CM_THISCALL cm_fn_005fd480(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_008984d0(u32 self, u32 arg1, u32 *arg2, u32 arg3);

u32 CM_THISCALL cm_fn_006dbb10(u32 self)
{
    u32 club = *(volatile u32 *)(self + 0x2f);
    u32 p, service, result, kind;
    if (!club)
        return 0;
    service = CM_GLOBAL(volatile u32, 0x00de1d08);
    if (!service)
        return club;
    if (*(volatile u8 *)(self + 0x19)) {
        result = cm_fn_00600a50(club, 1, 1, 0, 0);
        if (result) {
            club = *(volatile u32 *)(self + 0x2f);
            p = *(volatile u32 *)(club + 0xcf);
            service = CM_GLOBAL(volatile u32, 0x00b63c98);
            result = cm_fn_005fd480(service, p);
            if (!result)
                return 0;
        }
    }
    kind = (u32)(s32)*(volatile s8 *)(self + 0x19);
    club = *(volatile u32 *)(self + 0x2f);
    service = CM_GLOBAL(volatile u32, 0x00de1d08);
    return cm_fn_008984d0(service, club, (u32 *)(self + 0x3f), kind);
}
