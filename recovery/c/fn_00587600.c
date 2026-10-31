#include "cmre.h"

/* Preferred-image VA 0x00587600, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole indexed caller overwrites EAX at 007552c3. */
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u8 CM_THISCALL cm_fn_008d1da0(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00785790(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007861e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786780(u32, u32);
extern u32 CM_CDECL cm_fn_00547810(u32, u32);
extern s32 CM_CDECL cm_fn_0090cfc0(u32);

void CM_CDECL cm_fn_00587600(volatile u32 first, volatile u32 second,
                            volatile u32 third, volatile u32 fourth)
{
    u32 previous = CM_FS(u32, 0);
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x158];
    u32 news = (u32)scratch, recipients = news + 0xe0;
    u32 person, club, candidate, value, manager, position;
    s32 remaining, limit, chance;
    u8 count;

    frame.state = 0xffffffffu;
    frame.handler = 0x00958b5b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    person = first;
    if (person == 0) goto done;
    value = D(person + 0x61);
    if (value == 0) goto done;
    club = second;
    if (club == 0) goto done;
    manager = D(0x00de1f54);
    if (manager != 0) count = cm_fn_008d1da0(manager, person, 0xd8, recipients, 0x1e);
    else count = 0;
    cm_fn_0077bbe0(news, 0xfbe, 0);
    value = D(person);
    frame.state = 0;
    cm_fn_00785790(news, 0, value);
    value = D(person + 4); value = D(value + 0x33);
    cm_fn_00785790(news, 1, value);
    value = D(person + 8); value = D(value + 0x33);
    cm_fn_00785790(news, 2, value);
    value = D(person + 0xc); value = D(value + 0x33);
    cm_fn_00785790(news, 3, value);
    value = fourth;
    if (value != 0) value = D(value);
    else value = 0xffffffffu;
    cm_fn_00785790(news, 4, value);
    value = D(club);
    cm_fn_00785790(news, 5, value);
    value = (u32)cm_fn_0090cfc0(3);
    cm_fn_00785790(news, 6, value);
    candidate = third;
    value = cm_fn_00547810(person, candidate);
    if (value != 0) goto include_candidate;
    if (candidate == 0) goto no_candidate;
    value = D(candidate + 0x69);
    if (value == 0) goto no_candidate;
    limit = CM_GLOBAL(volatile s8, value + 0x16);
    chance = cm_fn_0090cfc0((u32)limit);
    limit = cm_fn_0090cfc0(20);
    if (chance > limit) goto include_candidate;
no_candidate:
    cm_fn_00785790(news, 7, 0xffffffffu);
    goto distribute;
include_candidate:
    value = D(candidate);
    cm_fn_00785790(news, 7, value);
    value = D(candidate + 4); value = D(value + 0x33);
    cm_fn_00785790(news, 8, value);
    value = D(candidate + 8); value = D(value + 0x33);
    cm_fn_00785790(news, 9, value);
    value = D(candidate + 0xc); value = D(value + 0x33);
    cm_fn_00785790(news, 10, value);
distribute:
    value = D(person + 0x61);
    if (value != 0 && CM_GLOBAL(volatile s16, value + 0xd) > 0x1f40) {
        manager = D(0x00b7431c);
        cm_fn_00786780(manager, news);
    } else {
        remaining = (s8)count;
        if (remaining > 0) {
            position = recipients;
            do {
                value = D(position);
                manager = D(0x00b7431c);
                cm_fn_007861e0(manager, news, value);
                position += 4;
                --remaining;
            } while (remaining != 0);
        }
    }
done:
    previous = frame.prev;
    CM_FS(u32, 0) = previous;
}
