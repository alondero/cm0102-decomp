#include "cmre.h"

/* Preferred-image VA 0x00842c00, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Some callers forward EAX. Preserve the full incidental result, including
   the partial AL/AX writes on early-return paths. */
extern u32 CM_THISCALL cm_fn_008cca10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u16 CM_CDECL cm_fn_005b7d20(u32);
extern u32 CM_CDECL cm_fn_0053b900(u32 *);
extern u32 CM_THISCALL cm_fn_00842e10(u32, u32, u32, u16);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_00842c00(u32 self, u32 person, u32 force)
{
    u8 scratch[0x14];
    u32 eax, base, field, value, receiver, low, current_club;
    u32 flags, total, sorted, club, index = 0, date_year, person_year;
    u8 count;

    eax = D(person + 0x61u);
    if (eax == 0) return eax;
    if (force == 0 && (s16)W(eax + 11u) < 5500) return eax;
    eax = D(person);
    base = D(0x00ae2d0cu);
    value = B(base + eax * 79u + 11u) & 0x30u;
    eax = (eax & 0xffffff00u) | value;
    if (value == 0x10u) return eax;
    if (B(person + 0x3du) != 11) return eax;
    if (D(person + 0x24u) != 0 && B(person + 0x28u) == 5) return eax;
    eax = D(person + 0x1au);
    if (eax != 0) {
        field = D(eax);
        eax = D(0x009cf458u);
        if (field == eax) return eax;
    }
    receiver = D(0x00de1f54u);
    eax = cm_fn_008cca10(receiver, person, 0);
    if (eax != 0) return eax;
    if (D(person + 0x39u) != 0) {
        eax = cm_fn_0054c8e0(0x00ae2c90u, (u32)scratch + 12u, 200);
        low = D(eax);
        eax = D(eax + 4u);
        D((u32)scratch) = low;
        date_year = (low >> 16) & 0xffffu;
        D((u32)scratch + 4u) = eax;
        person_year = W(person + 0x40u);
        eax = (eax & 0xffff0000u) | person_year;
        if (date_year < person_year ||
            (date_year == person_year && (s16)low < (s16)W(person + 0x3eu))) {
            receiver = D(0x00ae1050u);
            eax = cm_fn_004cf5a0(receiver, person);
            if (eax != 0) {
                value = B(eax + 0x4fu) & 0xf0u;
                if (value != 0x70u && D(eax + 0x45u) == 0) return eax;
            }
        }
    }
    receiver = D(0x00ae1050u);
    eax = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = eax;
    if (eax != 0) {
        eax = D(eax + 4u);
        base = D(0x00ae23b4u);
        current_club = base + eax * 581u;
    } else {
        current_club = 0;
    }
    eax = D(person);
    base = D(0x00ae2d0cu);
    count = (u8)(B(base + eax * 79u + 11u) & 15u);
    eax = D(person + 0x61u);
    flags = cm_fn_005b7d20(eax);
    if (flags & 2u) flags |= 4u;
    if (flags & 8u) flags |= 0x10u;
    if (flags & 0x20u) flags |= 0x10u;
    if (flags & 0x80u) flags |= 0x100u;
    if (flags & 0x800u) flags |= 0x400u;
    eax = D(0x00ae235cu);
    sorted = D(0x00ae2d1cu);
    if ((s32)eax <= 0) return eax;
    do {
        eax = D(sorted);
        base = D(0x00ae23b4u);
        club = base + eax * 581u;
        eax = (eax & 0xffffff00u) | count;
        if ((s8)count >= 5) return eax;
        eax = cm_fn_0053b900((u32 *)club);
        if (eax == 0 && current_club != club && D(person + 0x39u) != club) {
            eax = cm_fn_00842e10(self, person, club, (u16)flags);
            if (eax != 0) count = (u8)(count + 1u);
        }
        eax = index;
        total = D(0x00ae235cu);
        ++eax;
        sorted += 4u;
        index = eax;
    } while ((s32)eax < (s32)total);
    return eax;
}
