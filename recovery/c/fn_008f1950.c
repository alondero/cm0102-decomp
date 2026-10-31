/* Preferred-image VA 008f1950, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No direct callers: preserve the original final EAX at each exit. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 person, u32 type, u32 destination);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 value);
extern u32 CM_CDECL cm_fn_004dd0f0(u32 club, u32 pointer, u32 one, u32 zero);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 slot, u32 value, u32 zero);

u32 CM_CDECL cm_fn_008f1950(u32 person, u32 type)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0xe0];
    u32 predecessor, receiver, first, second, index, clubs, club, pointer, record, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x00964cdbu;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(work, 0, 0);
    frame.state = 0;
    receiver = D(0x00b7431cu);
    cm_fn_00786b70(receiver, person, type, (u32)work);
    first = (u32)cm_fn_00785830(work, 7);
    second = (u32)cm_fn_00785830(work, 8);
    index = (u32)cm_fn_00785830(work, 0);
    receiver = D(0x00de1f54u);
    clubs = D(0x00ae23bcu);
    club = clubs + index * 110u;
    pointer = cm_fn_008c61e0(receiver, first);
    record = D(pointer);
    /* These unchecked reads are present in the native callback. */
    if (D(record + 0x24u) == second && B(record + 0x2cu) != 3) {
        result = cm_fn_004dd0f0(club, pointer, 1, 0);
    } else {
        result = cm_fn_007fe600(0x00b74450u, 0x008f24c0u, 0x008f2800u, 1, 0, 0);
        if (result != 0) {
            cm_fn_007ff270(0x00b74450u, 0, record, 0);
            result = cm_fn_007ff270(0x00b74450u, 1, second, 0);
        }
    }
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
    return result;
}
