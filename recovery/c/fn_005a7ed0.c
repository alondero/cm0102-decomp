#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern char *CM_CDECL cm_fn_0066f4e0(u32 destination, const char *format);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 argument);

u32 CM_STDCALL cm_fn_005a7ed0(u32 object, u32 flag, u32 receiver)
{
    u32 format;
    if (flag == 0)
        format = 0x009cba54;
    else if ((u32)cm_fn_00785830((void *)object, 0) != 0)
        format = 0x009cbb3c;
    else
        format = 0x009cba74;
    cm_fn_0066f4e0(0x00de1f64, (const char *)format);
    return (u32)cm_fn_004ae660((void *)receiver, 0x00de1f64);
}
