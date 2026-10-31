#include "cmre.h"

/* Preferred-image VA0090ec10, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/fullEAX; SEH965634/scratch208 allocationL0/yearWORDL4,
   diagnosticsL8/L108. L6/L7 remain unwritten. The last constructor's
   original 766e5e/766e67 and 766e9d/766ee3..f2 consume only the low WORD
   of both year arguments; their upper halves are not forwarded to 412e40.
   Preserve conditional numbering, all constructor arguments and final state5. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008af520(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00766e10(u32, u32, u32, u32, u32, u32,
                                     u32, u32, u32, u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_0090ec10(u32 self, volatile u32 argument_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x208];
    u32 l = (u32)scratch, context, pointer, eax, argument, index = 0;
    u32 year, name, constant, array, number;
    frame.state = 0xffffffffu;
    frame.handler = 0x00965634;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    year = W(0x00ae2c92);
    context = D(self + 0x13);
    W(l + 4) = (u16)year;
    W(self + 8) = 6;
    eax = (u32)cm_fn_00944e46(0x18);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00ad43cc, 0, 0, l + 0x108, l + 8);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax,
                       l + 0x108, l + 8, 0x71);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    pointer = (u32)cm_fn_00944cf1(0x25d);
    D(l) = pointer;
    argument = argument_arg;
    frame.state = 0;
    if (pointer != 0) {
        constant = D(0x009cf94c);
        name = D(0x009cff40);
        index = 1;
        eax = cm_fn_008af520(pointer, context, 0, name, 1000, 500,
                             constant, 1, 0, argument, 1);
    } else eax = 0;
    array = D(self + 4);
    frame.state = 0xffffffffu;
    D(array) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l) = pointer;
    frame.state = 1;
    if (pointer != 0) {
        constant = D(0x009cf94c);
        name = D(0x009cff44);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 1500, 500,
                             constant, 1, 0, 0, argument, 1);
    } else eax = 0;
    array = D(self + 4);
    frame.state = 0xffffffffu;
    D(array + 4) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l) = pointer;
    frame.state = 2;
    if (pointer != 0) {
        constant = D(0x009cf94c);
        name = D(0x009cff48);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 500, 250,
                             constant, 1, 0, 0, argument, 1);
    } else eax = 0;
    array = D(self + 4);
    frame.state = 0xffffffffu;
    D(array + 8) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l) = pointer;
    frame.state = 3;
    if (pointer != 0) {
        constant = D(0x009cf94c);
        name = D(0x009cff4c);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 500, 250,
                             constant, 1, 0, 1, argument, 1);
    } else eax = 0;
    array = D(self + 4);
    frame.state = 0xffffffffu;
    D(array + 0xc) = eax;
    pointer = (u32)cm_fn_00944cf1(0x2b);
    D(l) = pointer;
    frame.state = 4;
    if (pointer != 0) {
        constant = D(0x009cf94c);
        name = D(0x009cff50);
        number = index++;
        eax = cm_fn_0092fe80(pointer, context, number, name, 2500, 1200,
                             constant, 1, 0, 0, argument, 0);
    } else eax = 0;
    array = D(self + 4);
    frame.state = 0xffffffffu;
    D(array + 0x10) = eax;
    pointer = (u32)cm_fn_00944cf1(0xa2);
    D(l) = pointer;
    frame.state = 5;
    if (pointer != 0) {
        /* Only the observed WORD is supplied; no initialization of L6/L7. */
        year = W(l + 4);
        constant = D(0x009cf94c);
        name = D(0x009cff54);
        eax = cm_fn_00766e10(pointer, context, index, name, 2000, 1000,
                             constant, 7, year, 3, year + 1u, argument, 0);
    } else eax = 0;
    array = D(self + 4);
    D(array + 0x14) = eax;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
