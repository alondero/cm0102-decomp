#include "cmre.h"

/* fn_0049f450: fastcall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x0049f450, image base 0x00400000.
   Object fields: 0x00 pointer released through 0x009452ca and cleared; 0x0c pointer released,
   then 0x10 and 0x14 cleared and 0x18 set to 0xfe; 0x19 pointer released, then 0x1d and 0x21
   cleared and 0x25 set to 0xfe; eight 0x5a53c0 receivers at +0x38a, +0x372, +0x35a, +0x342,
   +0x121, +0x109, +0xf1 and +0xd9.
   SEH frame {prev, handler 0x00956699, state} is linked first and unlinked last; state is
   written as 1 (after the +0x0c read), then byte 0, then -1 in listing order. */

extern void CM_FASTCALL cm_fn_0058e330(int *param_1);
extern void CM_THISCALL cm_fn_005a53c0(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_FASTCALL cm_fn_0049f450(u8 *param_1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 eax;
    u32 iv;
    u32 p;

    seh.state = -1;
    seh.handler = 0x00956699u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    eax = *(volatile u32 *)(param_1 + 0x0c);
    seh.state = 1;
    if (eax != 0u) {
        cm_fn_009452ca(eax);
        *(volatile u32 *)(param_1 + 0x0c) = 0u;
        *(volatile u32 *)(param_1 + 0x10) = 0u;
        *(volatile u32 *)(param_1 + 0x14) = 0u;
        *(volatile u8 *)(param_1 + 0x18) = 0xfeu;
    }

    eax = *(volatile u32 *)(param_1 + 0x19);
    if (eax != 0u) {
        cm_fn_009452ca(eax);
        *(volatile u32 *)(param_1 + 0x19) = 0u;
        *(volatile u32 *)(param_1 + 0x1d) = 0u;
        *(volatile u32 *)(param_1 + 0x21) = 0u;
        *(volatile u8 *)(param_1 + 0x25) = 0xfeu;
    }

    if (CM_GLOBAL(u32, 0x00adae18) == 1u) {
        iv = CM_GLOBAL(u32, 0x00adae14);
        if (iv != 0u) {
            cm_fn_0058e330((int *)iv);
            cm_fn_00944c94(iv);
            CM_GLOBAL(u32, 0x00adae14) = 0u;
        }
    }

    p = *(volatile u32 *)param_1;
    if (p != 0u) {
        cm_fn_009452ca(p);
        *(volatile u32 *)param_1 = 0u;
    }

    eax = CM_GLOBAL(u32, 0x00adae18);
    *(volatile u8 *)&seh.state = 0u;
    CM_GLOBAL(u32, 0x00adae18) = eax - 1u;
    cm_fn_005a53c0((u32)(param_1 + 0x38a));
    cm_fn_005a53c0((u32)(param_1 + 0x372));
    cm_fn_005a53c0((u32)(param_1 + 0x35a));
    cm_fn_005a53c0((u32)(param_1 + 0x342));
    seh.state = -1;
    cm_fn_005a53c0((u32)(param_1 + 0x121));
    cm_fn_005a53c0((u32)(param_1 + 0x109));
    cm_fn_005a53c0((u32)(param_1 + 0xf1));
    cm_fn_005a53c0((u32)(param_1 + 0xd9));

    CM_FS(u32, 0) = seh.prev;
}
