#include "cmre.h"

/* Preferred-image VA 0x007570b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The sole indexed caller's next callee overwrites EAX at 00556957.
   00785790 consumes only the signed low byte of its index slot. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_007e4750(u32, u32, u32);
extern u32 CM_CDECL cm_fn_007e4de0(u32, u32, u32);
extern u8 CM_THISCALL cm_fn_008d1da0(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

void CM_STDCALL cm_fn_007570b0(volatile u32 first, volatile u32 second,
                              volatile u32 third)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x17c];
    u32 local = (u32)scratch;
    u32 person, club, match, value, other, news = local + 0x24;
    u32 date = local + 4, counter = local + 3, recipients = local + 0x104;
    u8 index, count, a, b;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095d906;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(date);
    person = first;
    frame.state = 0;
    if (person == 0) goto done;
    value = D(person + 0x61);
    if (value == 0) goto done;
    club = second;
    if (club == 0) goto done;
    if ((s16)CM_GLOBAL(volatile u16, value + 0xb) < 0xdac) goto done;
    cm_fn_0077bbe0(news, 0xbcb, 0);
    value = D(person);
    *(volatile u8 *)&frame.state = 1;
    cm_fn_00785790(news, 0, value);
    value = D(person + 4);
    value = D(value + 0x33);
    cm_fn_00785790(news, 1, value);
    value = D(person + 8);
    value = D(value + 0x33);
    cm_fn_00785790(news, 2, value);
    value = D(person + 0xc);
    if (value != 0) value = D(value + 0x33);
    else value = 0xffffffffu;
    cm_fn_00785790(news, 3, value);
    value = D(club);
    cm_fn_00785790(news, 4, value);
    value = cm_fn_0090cfc0(2);
    cm_fn_00785790(news, 5, value);
    match = third;
    cm_fn_007e4750(match, club, date);
    B(counter) = 6;
    cm_fn_007e4de0(date, news, counter);
    a = B(match + 0x49);
    if (a == 0xff) {
        index = B(counter); B(counter) = (u8)(index + 1);
        cm_fn_00785790(news, index, 0);
    } else {
        b = B(match + 0x4a);
        if ((s8)a > (s8)b) {
            other = D(match + 0x1c);
            index = B(counter);
            B(counter) = (u8)(index + 1);
            cm_fn_00785790(news, index, other == club ? 1 : 2);
        } else if ((s8)a < (s8)b) {
            other = D(match + 0x20);
            index = B(counter);
            B(counter) = (u8)(index + 1);
            cm_fn_00785790(news, index, other == club ? 1 : 2);
        }
    }
    value = D(0x00de1f54);
    if (value != 0) count = cm_fn_008d1da0(value, person, 0xda, recipients, 0x1e);
    else count = 0;
    B(counter) = 0;
    if ((s8)count > 0) {
        index = 0;
        do {
            other = D(recipients + (u32)(s32)(s8)index * 4u);
            value = D(0x00b7431c);
            cm_fn_007861e0(value, news, other);
            index = B(counter);
            index = (u8)(index + 1);
            B(counter) = index;
        } while ((s8)index < (s8)count);
    }
done:
    value = frame.prev;
    CM_FS(u32, 0) = value;
}
