#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_THISCALL cm_fn_007e7860(u32 self)
{
    u32 list = *(volatile u32 *)(self + 7u);
    u32 pointer;
    *(volatile u32 *)self = 0x0096f104u;
    if (list != 0) {
        cm_fn_0068d040(list);
        cm_fn_00944c94((void *)list);
    }
    pointer = *(volatile u32 *)(self + 0xbu);
    if (pointer != 0) cm_fn_009452ca(pointer);
}
