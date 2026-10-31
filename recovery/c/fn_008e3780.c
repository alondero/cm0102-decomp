#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_CDECL cm_fn_0053e140(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0053e990(u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, unsigned long long, u32);
extern u32 CM_CDECL cm_fn_006181d0(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_THISCALL cm_fn_004ae660(u32, u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32, u32, u32, u32);

/* Preferred-image VA008e3780/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD3/RET12/full incidental EAX. Scratch340: currency100B L20,
   first-name300B L84, cash100B L1b0, second-name300B L214.
   43f460 returns the full EDX:EAX pair to the original money formatter. */
u32 CM_STDCALL cm_fn_008e3780(u32 event, volatile u32 mode,
                              volatile u32 output)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x340 / 4];
    u32 l = (u32)scratch, previous, person1, first1, second1, third1;
    u32 club1, club2, person2, value, amount2, first, second, third;
    u32 base, offset1, offset2, receiver, pointer, eax, person_offset;
    s32 gender1, gender2;
    unsigned long long money;

    frame.state = 0xffffffffu;
    frame.handler = 0x0096480b;
    previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    person1 = cm_fn_00785830(event, 6);
    D(l + 0x14) = person1;
    first1 = cm_fn_00785830(event, 7);
    D(l + 0x18) = first1;
    second1 = cm_fn_00785830(event, 8);
    D(l + 8) = second1;
    third1 = cm_fn_00785830(event, 9);
    value = mode;
    D(l + 4) = third1;
    if (value == 0) {
        cm_fn_0053e140(first1, second1, third1, l + 0x84, 300, 5, person1);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9d514, 100, l + 0x84);
        receiver = output;
        cm_fn_004ae660(receiver, 0x00de1f64);
        third = D(l + 4);
        base = D(0x00ae23bc);
        person_offset = person1 * 110u;
        eax = cm_fn_0053e990(base + person_offset, first1, second1, third, 0);
        if (eax != 0) {
            base = D(0x00ae23bc);
            receiver = output;
            eax = cm_fn_004ae8a0(receiver, l + 0x84, 0x7d7, base + person_offset);
        }
        goto done;
    }
    club1 = cm_fn_00785830(event, 0);
    club2 = cm_fn_00785830(event, 1);
    person2 = cm_fn_00785830(event, 2);
    value = cm_fn_00785830(event, 3);
    D(l + 0x10) = value;
    value = cm_fn_00785830(event, 4);
    D(l + 0xc) = value;
    value = cm_fn_00785830(event, 5);
    D(l + 0x1c) = value;
    value = cm_fn_00785830(event, 10);
    D(l) = value;
    amount2 = cm_fn_00785830(event, 11);
    third = D(l + 0x1c);
    second = D(l + 0xc);
    first = D(l + 0x10);
    cm_fn_0053e140(first, second, third, l + 0x214, 300, 1, person2);
    value = D(l + 0x14);
    third = D(l + 4);
    second = D(l + 8);
    first = D(l + 0x18);
    cm_fn_0053e140(first, second, third, l + 0x84, 300, 1, value);
    value = D(l);
    if ((s32)value > (s32)amount2) {
        D(l) = value;
        frame.state = 0;
        money = cm_fn_0043f460(l, 6);
        cm_fn_00441c10(l + 0x1b0, 100, money, 6);
        base = D(0x00ae23b4);
        offset1 = club1 * 581u;
        value = D(base + offset1 + 0x53);
        cm_fn_006181d0(value, l + 0x20, 100, 8);
        base = D(0x00ae23b4);
        gender1 = (s32)(s8)B(base + offset1 + 0x52);
        offset2 = club2 * 581u;
        gender2 = (s32)(s8)B(base + offset2 + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9d660,
                       gender2, gender2, gender1, gender1, 100, 100,
                       base + offset2 + 0x38, l + 0x84,
                       base + offset1 + 0x38, l + 0x214, l + 0x20, l + 0x1b0);
        frame.state = 0xffffffffu;
    } else {
        base = D(0x00ae23b4);
        offset1 = club1 * 581u;
        value = D(base + offset1 + 0x53);
        cm_fn_006181d0(value, l + 0x20, 100, 8);
        base = D(0x00ae23b4);
        gender1 = (s32)(s8)B(base + offset1 + 0x52);
        offset2 = club2 * 581u;
        gender2 = (s32)(s8)B(base + offset2 + 0x52);
        cm_fn_0066f4e0(0x00de1f64, 0x00a9d550,
                       gender2, gender2, gender1, gender1, 100, 100,
                       base + offset2 + 0x38, l + 0x84,
                       base + offset1 + 0x38, l + 0x214, l + 0x20);
    }
    receiver = output;
    cm_fn_004ae660(receiver, 0x00de1f64);
    base = D(0x00ae23b4);
    pointer = base + offset1;
    cm_fn_004ae8a0(receiver, pointer + 0x38, 0x7d5, pointer);
    base = D(0x00ae23b4);
    pointer = base + offset2;
    cm_fn_004ae8a0(receiver, pointer + 0x38, 0x7d5, pointer);
    third = D(l + 0x1c);
    second = D(l + 0xc);
    first = D(l + 0x10);
    base = D(0x00ae23bc);
    person_offset = person2 * 110u;
    eax = cm_fn_0053e990(base + person_offset, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        cm_fn_004ae8a0(receiver, l + 0x214, 0x7d7, base + person_offset);
    }
    person1 = D(l + 0x14);
    second = D(l + 8);
    first = D(l + 0x18);
    third = D(l + 4);
    base = D(0x00ae23bc);
    person_offset = person1 * 110u;
    eax = cm_fn_0053e990(base + person_offset, first, second, third, 0);
    if (eax != 0) {
        base = D(0x00ae23bc);
        eax = cm_fn_004ae8a0(receiver, l + 0x84, 0x7d7, base + person_offset);
    }
done:
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return eax;
}
