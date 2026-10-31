#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *allocation);
extern void CM_THISCALL cm_fn_0068d040(u32 self);

u32 CM_THISCALL cm_fn_004c74f0(u32 self)
{
    u32 value = *(volatile u32 *)self;
    u32 object;
    if (value != 0)
        cm_fn_009452ca(value);
    value = *(volatile u32 *)(self + 0xc);
    if (value != 0)
        cm_fn_009452ca(value);
    value = *(volatile u32 *)(self + 0x28);
    if (value != 0)
        value = (u32)cm_fn_009452ca(value);
    object = *(volatile u32 *)(self + 0x34);
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
    }
    object = *(volatile u32 *)(self + 0x38);
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
    }
    return value;
}
