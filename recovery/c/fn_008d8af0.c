#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);

/* Preferred-image VA 008d8af0/base00400000, pinned 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX. Scratch 2e0: p9L0,p8L4,p7L8,p6Lc,
   p5L10,p3L14,p4L18,p10L1c,moneyL20; textL24/L1b4, moneytextL150. */
u32 CM_STDCALL cm_fn_008d8af0(volatile u32 input_arg, volatile u32 mode_arg,
                             volatile u32 output_arg)
{
    volatile struct { u32 prev, handler, state; } seh;
    u32 scratch[0x2e0 / 4];
    u32 l = (u32)scratch;
    u32 input, p6, p7, p8, eax, mode, club0, club1, person0, p11;
    u32 third, second, first, person, output, base, offset0, offset1;
    u32 offset, club_pointer0, club_pointer1;
    s32 initial0, initial1;
    unsigned long long money;

    seh.state = 0xffffffffU;
    seh.handler = 0x0096442bU;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;
    input = input_arg;
    p6 = (u32)cm_fn_00785830(input, 6);
    D(l + 0xc) = p6;
    p7 = (u32)cm_fn_00785830(input, 7);
    D(l + 8) = p7;
    p8 = (u32)cm_fn_00785830(input, 8);
    D(l + 4) = p8;
    eax = (u32)cm_fn_00785830(input, 9);
    mode = mode_arg;
    D(l) = eax;
    if (mode == 0) {
        cm_fn_0053e140(p7, p8, eax, l + 0x24, 300, 5, p6);
        cm_fn_0066f4e0(0x00de1f64, 0x00a94fc0, l + 0x24);
        output = output_arg;
        cm_fn_004ae660(output, 0x00de1f64);
        third = D(l);
        base = D(0x00ae23bc);
        offset = p6 * 110U;
        eax = cm_fn_0053e990(base + offset, p7, p8, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            output = output_arg;
            eax = cm_fn_004ae8a0(output, l + 0x24, 0x7d7, base + offset);
        }
        CM_FS(u32, 0) = seh.prev;
        return eax;
    }

    club0 = (u32)cm_fn_00785830(input, 0);
    club1 = (u32)cm_fn_00785830(input, 1);
    person0 = (u32)cm_fn_00785830(input, 2);
    eax = (u32)cm_fn_00785830(input, 3);
    D(l + 0x14) = eax;
    eax = (u32)cm_fn_00785830(input, 4);
    D(l + 0x18) = eax;
    eax = (u32)cm_fn_00785830(input, 5);
    D(l + 0x10) = eax;
    eax = (u32)cm_fn_00785830(input, 10);
    D(l + 0x1c) = eax;
    p11 = (u32)cm_fn_00785830(input, 11);
    third = D(l + 0x10);
    second = D(l + 0x18);
    first = D(l + 0x14);
    cm_fn_0053e140(first, second, third, l + 0x1b4, 300, 1, person0);
    person = D(l + 0xc);
    third = D(l);
    second = D(l + 4);
    first = D(l + 8);
    cm_fn_0053e140(first, second, third, l + 0x24, 300, 1, person);
    eax = D(l + 0x1c);
    if ((s32)eax > (s32)p11) {
        D(l + 0x20) = eax;
        seh.state = 0;
        money = cm_fn_0043f460(l + 0x20, 6);
        cm_fn_00441c10(l + 0x150, 100, (u32)money, (u32)(money >> 32), 6);
        base = D(0x00ae23b4);
        offset0 = club0 * 581U;
        offset1 = club1 * 581U;
        initial0 = (s8)B(base + offset0 + 0x52);
        initial1 = (s8)B(base + offset1 + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a950d4, initial1, initial1,
                       initial0, initial0, base + offset1 + 0x38, l + 0x24,
                       base + offset0 + 0x38, l + 0x1b4, l + 0x150);
        seh.state = 0xffffffffU;
    } else {
        base = D(0x00ae23b4);
        offset0 = club0 * 581U;
        offset1 = club1 * 581U;
        initial0 = (s8)B(base + offset0 + 0x52);
        initial1 = (s8)B(base + offset1 + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a95000, initial1, initial1,
                       initial0, initial0, base + offset1 + 0x38, l + 0x24,
                       base + offset0 + 0x38, l + 0x1b4);
    }
    output = output_arg;
    cm_fn_004ae660(output, 0x00de1f64);
    base = D(0x00ae23b4);
    club_pointer0 = offset0 + base;
    cm_fn_004ae8a0(output, club_pointer0 + 0x38, 0x7d5, club_pointer0);
    base = D(0x00ae23b4);
    club_pointer1 = offset1 + base;
    cm_fn_004ae8a0(output, club_pointer1 + 0x38, 0x7d5, club_pointer1);
    third = D(l + 0x10);
    second = D(l + 0x18);
    first = D(l + 0x14);
    base = D(0x00ae23bc);
    offset = person0 * 110U;
    eax = cm_fn_0053e990(base + offset, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(output, l + 0x1b4, 0x7d7, base + offset);
    }
    person = D(l + 0xc);
    second = D(l + 4);
    first = D(l + 8);
    third = D(l);
    base = D(0x00ae23bc);
    offset = person * 110U;
    eax = cm_fn_0053e990(base + offset, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        eax = cm_fn_004ae8a0(output, l + 0x24, 0x7d7, base + offset);
    }
    CM_FS(u32, 0) = seh.prev;
    return eax;
}
