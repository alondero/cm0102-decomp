#include "cmre.h"
/* Preferred-image VA009304f0/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX; SEH9660db, scratch158:
   serializer L0..Ldf, initialized-prefix pool Le0..L157. The original
   8d1da0 limit30 prefix proof is recorded in continuation87 audit. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00785790(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007862d0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_007863f0(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00786640(u32,u32,u32);
extern unsigned long long CM_THISCALL cm_fn_00786780(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u16 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_CDECL cm_fn_00546ab0(u32,u32);
extern u8 CM_THISCALL cm_fn_008d1da0(u32,u32,u32,u32,u32);

static __attribute__((always_inline)) inline u32 send(u32 record, u32 recipient)
{
    u32 receiver = D(0x00b7431c);
    return (u32)cm_fn_007861e0(receiver, record, recipient);
}

u32 CM_THISCALL cm_fn_009304f0(u32 self, volatile u32 input_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x158];
    u32 l = (u32)scratch, input, value, id, base, pointer, club, person;
    u32 receiver, eax, cursor, remaining, saved, other;
    u8 count = 0;
    frame.state = 0xffffffffu;
    frame.handler = 0x009660db;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(l, 0x1b, 0);
    value = D(self + 0xd);
    frame.state = 0;
    cm_fn_00785790(l, 0, value);
    value = D(self + 0x11); cm_fn_00785790(l, 1, value);
    input = input_arg;
    value = W(input); cm_fn_00785790(l, 2, value);
    value = D(input + 2); cm_fn_00785790(l, 3, value);
    value = D(input + 6); cm_fn_00785790(l, 4, value);
    value = D(input + 0xa); cm_fn_00785790(l, 5, value);
    value = D(input + 0xe); cm_fn_00785790(l, 6, value);
    value = D(input + 0x16); cm_fn_00785790(l, 7, value);
    value = D(input + 0x1a); cm_fn_00785790(l, 8, value);
    value = D(input + 0x1e); cm_fn_00785790(l, 9, value);
    value = D(input + 0x22); cm_fn_00785790(l, 10, value);
    value = D(input + 0x26); cm_fn_00785790(l, 11, value);
    value = D(input + 0x2e); cm_fn_00785790(l, 12, value);
    cm_fn_00785790(l, 13, 0x420e9);
    value = D(input + 0x32); cm_fn_00785790(l, 14, value);
    value = D(input + 0x36); cm_fn_00785790(l, 15, value);
    value = D(input + 0x3a); cm_fn_00785790(l, 16, value);
    value = D(input + 0x3e); cm_fn_00785790(l, 17, value);
    value = D(input + 0x46); cm_fn_00785790(l, 18, value);
    value = cm_fn_0090cfc0(3); cm_fn_00785790(l, 19, value);
    id = D(input + 0xe);
    base = D(0x00ae23bc);
    value = D(base + id * 110u + 0x69);
    cm_fn_00785790(l, 20, value);
    id = D(input + 0xe);
    base = D(0x00ae23bc);
    pointer = D(base + id * 110u + 0x61);
    if (pointer) {
        value = (u32)(s32)(s16)cm_fn_005b7d20(pointer);
        cm_fn_00785790(l, 21, value);
        id = D(input + 0x16);
        base = D(0x00ae23b4);
        club = base + id * 581u;
        id = D(input + 0xe);
        base = D(0x00ae23bc);
        person = base + id * 110u;
        value = cm_fn_00546ab0(person, club);
        cm_fn_00785790(l, 22, value);
    }
    receiver = D(0x00de1f54);
    if (receiver) {
        id = D(input + 0xe);
        base = D(0x00ae23bc);
        person = base + id * 110u;
        pointer = D(person + 0x61);
        if (pointer) count = cm_fn_008d1da0(receiver, person, 0xda, l + 0xe0, 30);
    }
    eax = D(self + 9);
    if (eax) {
        receiver = D(0x00b7431c);
        eax = (u32)cm_fn_007862d0(receiver, l, eax);
        if ((s8)count <= 0) goto done;
        cursor = l + 0xe0;
        remaining = (u32)(s32)(s8)count;
        do {
            eax = D(cursor);
            other = D(self + 9);
            value = D(eax + 0x57);
            if (value != other) eax = send(l, eax);
            cursor += 4;
            remaining--;
        } while (remaining);
        goto done;
    }
    eax = D(self + 5);
    if (!eax) goto everyone;
    pointer = D(eax + 0x57);
    if (pointer) {
        receiver = D(0x00b7431c);
        eax = (u32)cm_fn_007863f0(receiver, l, pointer);
        if ((s8)count <= 0) goto done;
        cursor = l + 0xe0;
        remaining = (u32)(s32)(s8)count;
        do {
            eax = D(self + 5);
            if (eax) {
                saved = D(cursor);
                value = D(saved + 0x53);
                other = D(eax + 0x57);
                if (value == other) goto next_region;
            }
            eax = D(cursor);
            eax = send(l, eax);
next_region:
            cursor += 4;
            remaining--;
        } while (remaining);
        goto done;
    }
    eax = D(eax + 0x53);
    if (!eax) goto everyone;
    receiver = D(0x00b7431c);
    eax = (u32)cm_fn_00786640(receiver, l, eax);
    if ((s8)count <= 0) goto done;
    cursor = l + 0xe0;
    remaining = (u32)(s32)(s8)count;
    do {
        saved = D(self + 5);
        if (saved) {
            eax = D(cursor);
            eax = D(eax + 0x53);
            if (eax) {
                value = D(eax + 0x71);
                eax = D(saved + 0x53);
                if (value == eax) goto next_nation;
            }
        }
        eax = D(cursor);
        eax = send(l, eax);
next_nation:
        cursor += 4;
        remaining--;
    } while (remaining);
    goto done;
everyone:
    receiver = D(0x00b7431c);
    eax = (u32)cm_fn_00786780(receiver, l);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
