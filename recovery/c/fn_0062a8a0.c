#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0058e330(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *allocation);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

u32 CM_THISCALL cm_fn_0062a8a0(u32 self)
{
    u32 value = *(volatile u32 *)self;
    u32 object;
    if (value != 0)
        cm_fn_009452ca(value);
    value = *(volatile u32 *)(self + 4);
    if (value != 0)
        cm_fn_009452ca(value);
    object = *(volatile u32 *)(self + 0xc);
    if (object != 0) {
        cm_fn_0058e330(object);
        cm_fn_00944c94((void *)object);
    }
    value = *(volatile u32 *)(self + 0x10);
    if (value != 0)
        value = (u32)cm_fn_009452ca(value);
    object = *(volatile u32 *)(self + 0x28);
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
    }
    object = *(volatile u32 *)(self + 0x1c);
    if (object != 0)
        value = (u32)cm_fn_009452ca(object);
    return value;
}
