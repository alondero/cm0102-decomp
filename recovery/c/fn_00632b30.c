#include "cmre.h"

/* Preferred-image VA00632b30, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/fullEAX; SEH95b2c4/scratch218 selfL0/allocationL4/14B recordL8,
   diagnosticsL18/L118. Six independent allocations; numbering advances only
   on nonnull allocation. All eleven constructor stack words/RET44 retained.
   Keep final state5 and the initial-allocation failure's actual EAX. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00632b30(u32 self, volatile u32 argument_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x218];
    u32 l = (u32)scratch, context, pointer, eax, argument, index = 0;
    u32 name, object, array, number;
    frame.state = 0xffffffffu; frame.handler = 0x0095b2c4;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    D(l) = self;
    context = D(self + 0x13);
    W(self + 8) = 6;
    eax = (u32)cm_fn_00944e46(0x18);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x009e9dd4, 0, 0, l + 0x118, l + 0x18);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                       l + 0x118, l + 0x18, 0x63);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    B(l + 8) = 5;
    D(l + 9) = 0xffffffffu;
    D(l + 0xd) = 0xffffffffu;
    D(l + 0x11) = 0xffffffffu;
    B(l + 0x15) = 10;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    argument = argument_arg;
    frame.state = 0;
    if (pointer != 0) {
        index = 1;
        name = D(0x009cfa20);
        eax = cm_fn_0092fe80(pointer, context, 0, name, 9000, 8000,
                             0xffffffffu, 1, l + 8, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    frame.state = 0xffffffffu;
    array = D(object + 4);
    D(array) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    frame.state = 1;
    if (pointer != 0) {
        name = D(0x009cfa24);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 9000, 8000,
                             0xffffffffu, 1, 0, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    frame.state = 0xffffffffu;
    array = D(object + 4);
    D(array + 4) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    frame.state = 2;
    if (pointer != 0) {
        name = D(0x009cfa30);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 7000, 0x1482,
                             0xffffffffu, 1, 0, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    frame.state = 0xffffffffu;
    array = D(object + 4);
    D(array + 8) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    frame.state = 3;
    if (pointer != 0) {
        name = D(0x009cfa28);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 9000, 8000,
                             0xffffffffu, 1, 0, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    frame.state = 0xffffffffu;
    array = D(object + 4);
    D(array + 0xc) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    frame.state = 4;
    if (pointer != 0) {
        name = D(0x009cfa2c);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 7000, 6000,
                             0xffffffffu, 1, 0, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    frame.state = 0xffffffffu;
    array = D(object + 4);
    D(array + 0x10) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l + 4) = pointer;
    frame.state = 5;
    if (pointer != 0) {
        name = D(0x009cff78);
        eax = cm_fn_0092fe80(pointer, context, index, name, 5000, 3000,
                             0xffffffffu, 1, 0, 0, argument, 1);
    } else eax = 0;
    object = D(l);
    array = D(object + 4);
    D(array + 0x14) = eax;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
