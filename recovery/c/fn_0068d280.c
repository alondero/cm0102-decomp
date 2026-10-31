#include "cmre.h"
extern void CM_THISCALL cm_fn_0068d010(void *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_0068d280(u32 self, u32 node)
{
    u32 first = *(volatile u32 *)(self + 2);
    u32 a, b;
    if (node == first) {
        a = *(volatile u32 *)(node + 6);
        *(volatile u32 *)(self + 2) = a;
    } else {
        b = *(volatile u32 *)(node + 10);
        a = *(volatile u32 *)(node + 6);
        *(volatile u32 *)(b + 6) = a;
    }
    if (node == *(volatile u32 *)(self + 6)) {
        b = *(volatile u32 *)(node + 10);
        *(volatile u32 *)(self + 6) = b;
    } else {
        a = *(volatile u32 *)(node + 6);
        b = *(volatile u32 *)(node + 10);
        *(volatile u32 *)(a + 10) = b;
    }
    a = *(volatile u32 *)(self + 10);
    *(volatile u16 *)self = (u16)(*(volatile u16 *)self - 1);
    if (node == a) {
        a = *(volatile u32 *)(a + 6);
        *(volatile u32 *)(self + 10) = a;
    }
    if (node) {
        cm_fn_0068d010((void *)node);
        cm_fn_00944c94((void *)node);
    }
    return 1;
}
