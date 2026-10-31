#include "cmre.h"

/* Preferred-image VA008d7cd0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/RET4/fullEAX; SEH9643d6, scratch1c0 messages L0/Le0.
   4cf5a0 and7875e0 pop one stack word; pass the actual ECX values set by
   this caller even where their draft declarations omit the receiver.
   Preserve early-exit EAX, including the first sender's full low DWORD. */
#define D(a) (*(volatile u32 *)(a))
#define B(a) (*(volatile u8 *)(a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_007875e0(u32, u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32, u32, u32);

u32 CM_STDCALL cm_fn_008d7cd0(u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x1c0];
    u32 l = (u32)scratch, person, id, owner, base, eax, other_club, club;
    u32 pointer, value;
    frame.state = 0xffffffffu; frame.handler = 0x009643d6;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    id = D(input + 0xc);
    owner = D(0x00ae1050);
    base = D(0x00ae23bc);
    person = base + id * 0x6eu;
    eax = cm_fn_004cf5a0(owner, person);
    base = D(0x00ae23b4);
    D(0x00ae1080) = eax;
    if (eax != 0 && (B(eax + 0x35) & 0x3f) != 5) {
        id = D(eax + 4);
        other_club = base + id * 0x245u;
    } else other_club = 0;
    id = D(input + 8);
    club = base + id * 0x245u;
    owner = D(0x00b7431c);
    eax = cm_fn_007875e0(owner, club);
    if (eax != 0) {
        cm_fn_0077bbe0(l, 0xfac, 0);
        value = D(person);
        frame.state = 0;
        cm_fn_00785790(l, 0, value);
        pointer = D(person + 4); value = D(pointer + 0x33);
        cm_fn_00785790(l, 1, value);
        pointer = D(person + 8); value = D(pointer + 0x33);
        cm_fn_00785790(l, 2, value);
        pointer = D(person + 0xc); value = D(pointer + 0x33);
        cm_fn_00785790(l, 3, value);
        if (other_club != 0) value = D(other_club);
        else value = 0xffffffffu;
        cm_fn_00785790(l, 4, value);
        value = D(input + 8);
        cm_fn_00785790(l, 5, value);
        value = D(input + 0x20);
        cm_fn_00785790(l, 7, value);
        value = D(input + 0x24);
        cm_fn_00785790(l, 8, value);
        cm_fn_00785790(l, 9, 0);
        cm_fn_00785790(l, 10, 0);
        owner = D(0x00b7431c);
        eax = (u32)cm_fn_007861e0(owner, l, club);
        frame.state = 0xffffffffu;
    }
    if (other_club == 0) goto done;
    owner = D(0x00b7431c);
    eax = cm_fn_007875e0(owner, other_club);
    if (eax == 0) goto done;
    eax = D(input + 4);
    if (eax == 0) goto done;
    value = D(eax + 2);
    D(eax + 0xa) = value;
    pointer = D(input + 4);
    eax = cm_fn_0068d1b0(pointer);
    if (eax == 0) goto done;
    do {
        id = D(eax);
        base = D(0x00ae23bc);
        person = base + id * 0x6eu;
        cm_fn_0077bbe0(l + 0xe0, 0xfac, 0);
        value = D(person);
        frame.state = 1;
        cm_fn_00785790(l + 0xe0, 0, value);
        pointer = D(person + 4); value = D(pointer + 0x33);
        cm_fn_00785790(l + 0xe0, 1, value);
        pointer = D(person + 8); value = D(pointer + 0x33);
        cm_fn_00785790(l + 0xe0, 2, value);
        pointer = D(person + 0xc); value = D(pointer + 0x33);
        cm_fn_00785790(l + 0xe0, 3, value);
        value = D(input + 8);
        cm_fn_00785790(l + 0xe0, 4, value);
        value = D(other_club);
        cm_fn_00785790(l + 0xe0, 5, value);
        value = D(input + 0x20);
        cm_fn_00785790(l + 0xe0, 7, value);
        value = D(input + 0x24);
        cm_fn_00785790(l + 0xe0, 8, value);
        cm_fn_00785790(l + 0xe0, 9, 0);
        cm_fn_00785790(l + 0xe0, 10, 1);
        owner = D(0x00b7431c);
        cm_fn_007861e0(owner, l + 0xe0, other_club);
        pointer = D(input + 4);
        frame.state = 0xffffffffu;
        eax = cm_fn_0068d1b0(pointer);
    } while (eax != 0);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
