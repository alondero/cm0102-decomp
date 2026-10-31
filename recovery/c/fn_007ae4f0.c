#include "cmre.h"
extern u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

u32 CM_THISCALL cm_fn_007ae4f0(u32 self)
{
    volatile struct { u32 prev, handler; s32 state; } frame;
    u32 eax, p;
    frame.state = -1;
    frame.handler = 0x0095f362;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    eax = *(volatile u32 *)(self + 0x1ee);
    frame.state = 6;
    if (eax) {
        eax = (u32)cm_fn_009452ca(eax);
        *(volatile u32 *)(self + 0x1ee) = 0;
    }
    p = *(volatile u32 *)(self + 0x170);
    if (p) {
        cm_fn_0068d1f0((u8 *)0x00b74438, (const u8 *)p);
        eax = (u32)cm_fn_00944c94((void *)p);
        *(volatile u32 *)(self + 0x170) = 0;
    }
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
