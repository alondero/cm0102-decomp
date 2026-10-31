#include "cmre.h"

extern void CM_THISCALL cm_fn_0068d010(void *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);

void CM_THISCALL cm_fn_0068d2f0(u32 self, u32 source)
{
    u32 node = *(volatile u32 *)(self + 2u);
    if (node != 0) {
        u32 next;
        do {
            node = *(volatile u32 *)(self + 2u);
            next = *(volatile u32 *)(node + 6u);
            if (node != 0) {
                cm_fn_0068d010((void *)node);
                cm_fn_00944c94((void *)node);
            }
            *(volatile u32 *)(self + 2u) = next;
        } while (next != 0);
    }
    *(volatile u16 *)self = 0;
    *(volatile u32 *)(self + 2u) = 0;
    *(volatile u32 *)(self + 6u) = 0;
    *(volatile u32 *)(self + 0xau) = 0;
    node = *(volatile u32 *)(source + 2u);
    *(volatile u32 *)(source + 0xau) = node;
    for (;;) {
        u32 cursor = *(volatile u32 *)(source + 0xau);
        u32 data, next, size, edx;
        if (cursor == 0) break;
        data = *(volatile u32 *)cursor;
        next = *(volatile u32 *)(cursor + 6u);
        *(volatile u32 *)(source + 0xau) = next;
        if (data == 0) break;
        size = *(volatile u16 *)(node + 4u);
        edx = (data & 0xffff0000u) | size;
        data = *(volatile u32 *)node;
        cm_fn_0068d0d0((volatile u8 *)self, edx, (const u8 *)data, (s16)size);
        node = *(volatile u32 *)(source + 0xau);
    }
}
