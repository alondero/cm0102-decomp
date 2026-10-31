/* Faithful C recovery at preferred-image VA 004448a0. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0058e330(u32 *param);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_THISCALL cm_fn_004448a0(void *self)
{
    u32 base = (u32)self;
    u32 value = *(volatile u32 *)base;
    if (value != 0) {
        cm_fn_0058e330((u32 *)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)base = 0;
    }

    value = *(volatile u32 *)(base + 4u);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 4u) = 0;
        *(volatile u32 *)(base + 8u) = 0;
        *(volatile u32 *)(base + 0xcu) = 0;
    }
    value = *(volatile u32 *)(base + 0x10u);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x10u) = 0;
        *(volatile u32 *)(base + 0x14u) = 0;
        *(volatile u32 *)(base + 0x18u) = 0;
    }
    value = *(volatile u32 *)(base + 0x1cu);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x1cu) = 0;
        *(volatile u32 *)(base + 0x20u) = 0;
    }
    value = *(volatile u32 *)(base + 0x24u);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x24u) = 0;
        *(volatile u32 *)(base + 0x28u) = 0;
        *(volatile u32 *)(base + 0x2cu) = 0;
    }
}
