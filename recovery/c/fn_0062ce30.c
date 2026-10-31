/* Preferred-image VA 0062ce30, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *date);
extern u8 CM_THISCALL cm_fn_0076d780(u32 incoming_ecx, u32 club, u32 flag);
extern u32 CM_THISCALL cm_fn_0076e040(u32 self, u32 nation, u32 kind);
extern u32 CM_CDECL cm_fn_00669bd0(u32 object, void *first, void *second);
extern u16 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);

u32 CM_STDCALL cm_fn_0062ce30(u32 person)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x18];
    u32 saved_person, club, flag, receiver, pointer, low, high, team, result;
    u32 later = 0;
    u16 year;
    u8 kind;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095b120u;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work + 0x10);
    frame.state = 0;
    cm_fn_0054a110(work);
    saved_person = person;
    B((u32)&frame.state) = 1;
    if (saved_person == 0) {
        result = 1;
        goto done;
    }
    club = D(saved_person + 0x24u);
    if (club != 0) {
        flag = B(saved_person + 0x5eu) & 8u;
        receiver = D(0x00b74318u);
        kind = cm_fn_0076d780(receiver, club, flag);
        person = (person & 0xffffff00u) | kind;
        club = D(saved_person + 0x24u);
        pointer = D(club + 0x53u);
        receiver = D(0x00b74318u);
        pointer = cm_fn_0076e040(receiver, pointer, person);
        low = D(pointer);
        year = W(0x00ae2c92u);
        D((u32)work + 8u) = low;
        high = D(pointer + 4u);
        D((u32)work + 12u) = high;
        if ((u16)(low >> 16) > year)
            later = 1;
        else if ((u16)(low >> 16) == year)
            later = (s16)low > (s16)W(0x00ae2c90u);
    }
    team = D(saved_person + 0x39u);
    result = 0;
    if (team != 0 && later == 0 && D(team + 0x1dbu) == 0xffffffffu) {
        if (cm_fn_00669bd0(team, work + 0x10, work) == 0) {
            high = D((u32)work + 4u);
            low = D((u32)work);
            if (cm_fn_0054cdb0(0x00ae2c90u, low, high) < 40u)
                result = 1;
        }
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
