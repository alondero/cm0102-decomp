#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_007b14a0(u32 self, u32 flag)
{
    u32 p = *(volatile u32 *)(self + 0x1ee);
    s32 n;
    if (p) {
        cm_fn_009452ca(p);
        *(volatile u32 *)(self + 0x1ee) = 0;
    }
    n = *(volatile s32 *)(self + 0x190);
    *(volatile u32 *)(self + 0x1f2) = 0;
    *(volatile u32 *)(self + 0x1f6) = 0;
    *(volatile u32 *)(self + 0x2a4) = 0;
    if (n > -1 && flag) {
        p = *(volatile u32 *)(self + 0x170);
        if (p) {
            *(volatile u32 *)(self + 0x190) = 0xffffffff;
            cm_fn_0068d1f0((u8 *)0x00b74438, (const u8 *)p);
            cm_fn_00944c94((void *)p);
            *(volatile u32 *)(self + 0x170) = 0;
        }
    }
    return 1;
}
