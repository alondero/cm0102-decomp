#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0048c650(void *);
extern u32 CM_CDECL cm_fn_00944c94(void *);
extern void CM_STDCALL cm_fn_009462bb(u32, u32, u32, void *);

u32 CM_THISCALL cm_fn_00682300(void *self)
{
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    u32 p, count, result;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095c954u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;
    *(volatile u32 *)self = 0x0096d020u;
    p = *(volatile u32 *)((u32)self + 0xeau);
    frame.state = 2;
    if (p != 0) {
        count = *(volatile u32 *)(p - 4);
        cm_fn_009462bb(p, 0x10u, count, (void *)0x005a53c0u);
        cm_fn_00944c94((void *)(p - 4));
    }
    frame.state = 0xffffffffu;
    result = cm_fn_0048c650(self);
    CM_FS(void *, 0) = frame.prev;
    return result;
}
