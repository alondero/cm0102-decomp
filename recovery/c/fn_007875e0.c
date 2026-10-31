#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053ca90(volatile u8 *self);
extern unsigned long long CM_THISCALL cm_fn_005fd210(void *self, u32 arg1);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);

u32 CM_STDCALL cm_fn_007875e0(u32 club)
{
    u32 value, service;
    if (!club) return 0;
    value = cm_fn_00600a50(club, 1, 1, 0, 0);
    if (!value) return 0;
    value = cm_fn_0053ca90((volatile u8 *)club);
    service = CM_GLOBAL(volatile u32, 0x00b63c98);
    cm_fn_005fd210((void *)service, value);
    return 1;
}
