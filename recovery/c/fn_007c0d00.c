#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0058e330(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_007c0d00(u32 self)
{
    u32 p = *(volatile u32 *)self;
    u32 eax;
    if (p) {
        cm_fn_0058e330(p);
        cm_fn_00944c94((void *)p);
        *(volatile u32 *)self = 0;
    }
    eax = *(volatile u32 *)(self + 4);
    if (eax) {
        cm_fn_009452ca(eax);
        *(volatile u32 *)(self + 4) = 0;
        *(volatile u32 *)(self + 8) = 0;
        *(volatile u32 *)(self + 0xc) = 0;
    }
    eax = *(volatile u32 *)(self + 0x30);
    if (eax) {
        cm_fn_009452ca(eax);
        *(volatile u32 *)(self + 0x30) = 0;
        *(volatile u32 *)(self + 0x34) = 0;
    }
    eax = *(volatile u32 *)(self + 0x3c);
    if (eax) {
        eax = (u32)cm_fn_009452ca(eax);
        *(volatile u32 *)(self + 0x3c) = 0;
        *(volatile u32 *)(self + 0x40) = 0;
    }
    return eax;
}
