/* Preferred-image VA 008c38b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EDX from the first two original helpers supplies the high bits of a later
   argument at VA008c399e. A fastcall view exposes the incoming EDX while
   preserving ECX and the native stack arguments/cleanup; retain EDX:EAX. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_FASTCALL cm_fn_008c7560(u32 self, u32 incoming_edx, u32 person, u32 club, u32 flag);
extern unsigned long long CM_FASTCALL cm_fn_008c7950(u32 self, u32 incoming_edx, u32 person, u32 nation, u32 record, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_THISCALL cm_fn_004cf720(u32 self, u32 record, u32 flag);
extern u32 CM_THISCALL cm_fn_008e5680(void *object);
extern void CM_THISCALL cm_fn_008e5710(void *object);
extern void CM_THISCALL cm_fn_008de790(u32 self, void *object, u32 club, u32 mode);
extern void CM_THISCALL cm_fn_008e4290(u32 self, u32 person, u32 record);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 club);
extern void CM_THISCALL cm_fn_005998e0(u32 self, u32 value);

u32 CM_THISCALL cm_fn_008c38b0(u32 self, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x44];
    unsigned long long pair;
    u32 previous, index, base, person, club, nation, receiver, pointer;
    u32 club_id, person_id, flag, value, result;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00963c28u;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    index = D(record);
    base = D(0x00ae23bcu);
    person = base + index * 110u;
    index = D(record + 4u);
    base = D(0x00ae23b4u);
    club = base + index * 0x245u;
    pair = cm_fn_008c7560(self, base + index, person, club, 1);
    if ((u32)pair != 0) {
        nation = D(club + 0x53u);
        pair = cm_fn_008c7950(self, (u32)(pair >> 32), person, nation, record, 0, 0);
        if ((u32)pair == 0) {
            receiver = D(0x00ae1050u);
            pointer = cm_fn_004cf5a0(receiver, person);
            D(0x00ae1080u) = pointer;
            if (pointer != 0 && (B(pointer + 0x35u) & 0x3fu) == 5u) {
                cm_fn_008e5680(work);
                club_id = D(club);
                person_id = D(person);
                frame.state = 0;
                D((u32)work + 8u) = club_id;
                D((u32)work + 12u) = person_id;
                B((u32)work + 0x1fu) = 0;
                cm_fn_008de790(self, work, club, 0x13u);
                frame.state = 0xffffffffu;
                cm_fn_008e5710(work);
            } else {
                cm_fn_008e4290(self, person, record);
            }
            result = 0;
            goto done;
        }
    }
    flag = B(record + 0x61u);
    receiver = D(0x00ae1050u);
    flag |= (u32)(pair >> 32) & 0xffffff00u;
    result = cm_fn_004cf720(receiver, record, flag);
    if (result != 0) {
        value = D(record + 8u);
        receiver = D(0x00ae2a38u);
        pointer = cm_fn_005a0590(receiver, club);
        cm_fn_005998e0(pointer, value);
        result = 1;
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
