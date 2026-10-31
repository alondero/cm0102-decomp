#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_THISCALL cm_fn_008c4950(u32, u32);
extern u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_0069b790(
    u32 incoming_eax, u32 unused_edx, u32 receiver, u32 club, u32 mode, u32 fourth, u32 fifth);
extern u32 CM_CDECL __attribute__((regparm(1))) cm_fn_00544820(u32 incoming_eax, u32 club);
extern u32 CM_THISCALL cm_fn_0069ba50(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00600a50(u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008b2880(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007f7270(u32, u32);

static u32 clear_slots_7aaeb0(u32 person, u32 field, u32 offset, u32 count)
{
    u32 index = 0, club, slot, value;
    do {
        club = D(person + field);
        slot = club + index + offset;
        value = D(slot);
        if (value == person) D(slot) = 0;
        index += 4;
        --count;
    } while (count != 0);
    return slot;
}

static u32 clear_link_7aaeb0(u32 person, u32 field, u32 status,
                             u32 table, u32 eax, int secondary)
{
    u32 club = D(person + field), target, pointer, bound, value, receiver;
    int notify;
    if (club == 0) return eax;
    eax = (u32)(s32)(s8)B(person + status) - 1u;
    if (eax > 14) goto clear;
    target = D(table + eax * 4u);
    switch (target) {
    case 0x007aaed9: case 0x007ab0a9:
        D(club + 0xbf) = 0;
        break;
    case 0x007aaee4: case 0x007ab0be:
        eax = clear_slots_7aaeb0(person, field, 0xc3, 3);
        break;
    case 0x007aaf0d: case 0x007ab0f1:
        eax = D(club + 0xcf);
        notify = eax == person;
        if (notify) {
            bound = D(0x00ae2364);
            bound -= 16u;
            value = D(eax);
            if ((s32)value >= (s32)bound) {
                receiver = D(0x00de1f54);
                eax = cm_fn_008c4950(receiver, club);
            }
        }
        goto notify_link;
    case 0x007aaf34: case 0x007ab11c:
        D(club + 0xd3) = 0;
        break;
    case 0x007aaf3f: case 0x007ab131:
        value = D(club + 0xcf);
        notify = value == person;
notify_link:
        if (notify) {
            receiver = D(0x00b67a44);
            eax = cm_fn_0069b790(eax, 0, receiver, club, 3, 0, 0);
            eax = cm_fn_00544820(eax, club);
            if ((u8)eax != 0) {
                receiver = D(0x00b67a44);
                eax = cm_fn_0069ba50(receiver, club, person);
            }
        }
        if (!secondary || notify) D(person + field) = 0;
        break;
    case 0x007aaf79: case 0x007aafc5:
    case 0x007ab179: case 0x007ab1cf:
        eax = cm_fn_00600a50(club, 1, 1, 0, 0);
        if (eax != 0) {
            pointer = D(person + field);
            receiver = D(0x00de1f44);
            cm_fn_008b2880(receiver, pointer, 0);
        }
        eax = clear_slots_7aaeb0(person, field, 0x19f, 5);
        break;
    case 0x007ab00e: case 0x007ab225:
        eax = clear_slots_7aaeb0(person, field, 0x1b3, 7);
        pointer = D(person + field);
        eax = cm_fn_00600a50(pointer, 1, 1, 0, 0);
        if (eax != 0) {
            pointer = D(person + field);
            D(person + field) = 0;
            receiver = D(0x00b74448);
            value = D(pointer + 0xcf);
            eax = cm_fn_007f7270(receiver, value);
        }
        break;
    case 0x007ab062: case 0x007ab286:
        eax = clear_slots_7aaeb0(person, field, 0x1cf, 3);
        break;
    case 0x007ab086: case 0x007ab2aa:
        break;
    default: __builtin_unreachable();
    }
clear:
    D(person + field) = 0;
    return eax;
}

/* Preferred-image VA007aaeb0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   STD1/RET4/full incidental EAX. regparm(1) transports incoming EAX:
   the both-null path returns it untouched. Each dispatch retains its
   DWORD target-table read and each slot iteration reloads the link.
   Secondary unmatched CF paths have one clear; matched paths have two.
   544820 may only clear AL, so its incoming EAX is forwarded too.
   69b790's early returns retain EAX: regparm(3) carries EAX and ECX.
   Its entry EDX is unused: early exits never read it, and the first
   call600a50(club,3,1,0,0) overwrites EDX in its predicate helpers. */
u32 CM_STDCALL __attribute__((regparm(1))) cm_fn_007aaeb0(u32 incoming_eax, u32 person)
{
    u32 eax = clear_link_7aaeb0(person, 0x39, 0x3d, 0x007ab2bc, incoming_eax, 0);
    eax = clear_link_7aaeb0(person, 0x24, 0x28, 0x007ab2f8, eax, 1);
    B(person + 0x3d) = 0;
    B(person + 0x28) = 0;
    return eax;
}
