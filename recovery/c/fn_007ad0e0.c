/* Preferred-image VA 007ad0e0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053d980(u32 person, u32 nation, u32 mode);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_0053c590(u32 club, u8 mode, u32 filter);
extern u32 CM_THISCALL cm_fn_008ce990(u32 self, u32 person, u32 club, u16 *output, u32 flag);
/* Native explicitly reloads ECX from its saved receiver before this ret12
   boundary, although the accepted callee does not consume that receiver. */
extern u32 CM_THISCALL cm_fn_007ad220(u32 incoming_ecx, u32 person, u32 club, u32 rating);

u32 CM_THISCALL cm_fn_007ad0e0(u32 self, u32 person)
{
    u8 work[0x14];
    u32 saved_person = person;
    u32 excluded, total, club, pointer, id, expected, result, receiver, index;
    u32 best = 0xffff9e58u;
    u8 rating;
    D((u32)work + 0x10u) = self;
    excluded = D(0x00ae2350u);
    club = D(0x00ae23b4u);
    total = D(0x00ae235cu) - excluded * 2u;
    D((u32)work + 4u) = 0;
    D((u32)work) = 0;
    if ((s32)total <= 0)
        return 0;
    do {
        pointer = D(club + 0x53u);
        if (cm_fn_0053d980(saved_person, pointer, 1) != 0)
            goto next;
        result = cm_fn_00540a50(club, (u32)work + 8u, 1);
        if (result != 0 && D((u32)work + 8u) == 0)
            goto next;
        pointer = D(saved_person + 0x1eu);
        if (pointer != 0) {
            id = D(pointer);
            expected = D(0x009cf424u);
            if (id == expected) {
                id = D(club);
                if (id != D(0x009d0120u) && id != D(0x009d0134u) &&
                    id != D(0x009d0128u) && id != D(0x009d01a0u) &&
                    id != D(0x009d0124u))
                    goto next;
            }
        }
        result = cm_fn_0053c590(club, 0, 0);
        receiver = D(0x00de1f54u);
        rating = (u8)result;
        B((u32)work + 12u) = rating;
        result = cm_fn_008ce990(receiver, saved_person, club, (u16 *)&person, 0);
        if (result == 0 || (s8)rating >= 50)
            goto next;
        result = D((u32)work + 12u);
        receiver = D((u32)work + 0x10u);
        result = cm_fn_007ad220(receiver, saved_person, club, result);
        if ((s16)result > (s16)best) {
            D((u32)work + 4u) = club;
            best = result;
        }
next:
        excluded = D(0x00ae2350u);
        index = D((u32)work) + 1u;
        club += 0x245u;
        total = D(0x00ae235cu) - excluded * 2u;
        D((u32)work) = index;
    } while ((s32)index < (s32)total);
    return D((u32)work + 4u);
}
