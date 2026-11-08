/* Preferred-image VA 0x006e1a40, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   regparm(3) names the actual entry EAX/EDX/ECX; there are no stack arguments.
   EAX high bits survive byte eligibility/selection writes and otherwise come
   from the actual last original helper result, independently of the best score. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_006e1cf0(u32 first, u32 second, u32 club);

u32 CM_CDECL __attribute__((regparm(3))) cm_fn_006e1a40(
    u32 incoming_eax, u32 unused_edx, u32 self)
{
    u32 carrier = incoming_eax;
    u16 best = 0;
    u8 selected = 0xffu;
    u8 index = 0;
    u32 record;
    (void)unused_edx;
    record = D(self + 0x23u);
    do {
        u8 eligible = B(record + 0x19u);
        carrier = (carrier & 0xffffff00u) | eligible;
        if ((s8)eligible >= 0) {
            u32 club = D(self + 0x2fu);
            carrier = cm_fn_006e1cf0(0, record, club);
            if (selected == 0xffu || (s16)(u16)carrier > (s16)best) {
                best = (u16)carrier;
                selected = index;
            }
        }
        ++index;
        record += 0x1beu;
    } while (index < 20u);
    return (carrier & 0xffffff00u) | selected;
}
