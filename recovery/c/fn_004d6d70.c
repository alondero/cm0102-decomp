/* Preferred-image VA 004d6d70, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 option);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 self, void *message, u32 club);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_004d6d70(u32 record, volatile u32 optional)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x2e0];
    u32 previous, club_id, table, person_id, club, person, value, pointer, receiver, result, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00956eebu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    if (record == 0) {
        cm_fn_00944cff(0x009a8190u, 0, 0, work + 0x1e0, work + 0xe0);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x1e0, work + 0xe0, 0x2611u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    cm_fn_0077bbe0(work, 0xbbeu, 0);
    club_id = D(record + 4u);
    table = D(0x00ae23b4u);
    frame.state = 0;
    person_id = D(record);
    club = table + club_id * 581u;
    value = D(club);
    table = D(0x00ae23bcu);
    person = table + person_id * 110u;
    cm_fn_00785790(work, 0, value);
    value = D(person);
    cm_fn_00785790(work, 2, value);
    pointer = D(person + 4u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 3, value);
    pointer = D(person + 8u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 4, value);
    pointer = D(person + 12u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work, 5, value);
    pointer = optional;
    if (pointer != 0)
        value = D(pointer);
    else
        value = 0xffffffffu;
    cm_fn_00785790(work, 6, value);
    receiver = D(0x00b7431cu);
    result = (u32)cm_fn_007861e0(receiver, work, club);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
