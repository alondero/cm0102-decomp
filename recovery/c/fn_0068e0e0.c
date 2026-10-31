#include "cmre.h"

extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Thiscall (ECX = self), no stack arguments, `ret 0`, no return value.
   Six u32 slots are visited in this order: +0x00, +0x08, +0x04, +0x0c, +0x10,
   +0x14. Slot +0x00 is released through 0x9452ca (cdecl, one argument) and
   zeroed. The other five are released through 0x68d040 (fastcall, the slot
   value in ECX), then through 0x944c94 (cdecl, one argument), and zeroed.
   A zero slot is skipped. */
void CM_THISCALL cm_fn_0068e0e0(void *self)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 value;

    value = *(volatile u32 *)(base + 0x00);
    if (value != 0) {
        cm_fn_009452ca(value);
        *(volatile u32 *)(base + 0x00) = 0;
    }

    value = *(volatile u32 *)(base + 0x08);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)(base + 0x08) = 0;
    }

    value = *(volatile u32 *)(base + 0x04);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)(base + 0x04) = 0;
    }

    value = *(volatile u32 *)(base + 0x0c);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)(base + 0x0c) = 0;
    }

    value = *(volatile u32 *)(base + 0x10);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)(base + 0x10) = 0;
    }

    value = *(volatile u32 *)(base + 0x14);
    if (value != 0) {
        cm_fn_0068d040((int)value);
        cm_fn_00944c94((void *)value);
        *(volatile u32 *)(base + 0x14) = 0;
    }
}
