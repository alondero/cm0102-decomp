#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0058e330(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_008687c0(u32 self)
{
    u32 p = *(volatile u32 *)self;
    u32 eax;
    if (p) {
        cm_fn_0058e330(p);
        eax = (u32)cm_fn_00944c94((void *)p);
    }
    p = *(volatile u32 *)(self + 4);
    if (p) {
        cm_fn_0058e330(p);
        eax = (u32)cm_fn_00944c94((void *)p);
    }
    eax = *(volatile u32 *)(self + 8);
    if (eax)
        eax = (u32)cm_fn_009452ca(eax);
    eax = *(volatile u32 *)(self + 12);
    if (eax)
        eax = (u32)cm_fn_009452ca(eax);
    p = *(volatile u32 *)(self + 16);
    if (p)
        eax = (u32)cm_fn_009452ca(p);
    return eax;
}
