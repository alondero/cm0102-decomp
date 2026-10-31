#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *allocation);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 allocation);

u32 CM_THISCALL cm_fn_004185f0(u32 self)
{
    u32 value = *(volatile u32 *)(self + 0xc);
    u32 object;
    if (value != 0) {
        value = (u32)cm_fn_009452ca(value);
        *(volatile u32 *)(self + 0xc) = 0;
    }
    object = *(volatile u32 *)self;
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
        *(volatile u32 *)self = 0;
    }
    object = *(volatile u32 *)(self + 4);
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
        *(volatile u32 *)(self + 4) = 0;
    }
    object = *(volatile u32 *)(self + 8);
    if (object != 0) {
        cm_fn_0068d040(object);
        value = (u32)cm_fn_00944c94((void *)object);
        *(volatile u32 *)(self + 8) = 0;
    }
    return value;
}
