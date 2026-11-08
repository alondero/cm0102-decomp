/* Preferred-image VA 004d4660, base 00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4; every original exit writes full EAX as zero or one.
   The three native callees remain explicit original-code boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define C(a) CM_GLOBAL(volatile s8, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_CDECL cm_fn_00544520(u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_00866f60(volatile u32 *self);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32 self, u32 output, u32 amount);

u32 CM_THISCALL cm_fn_004d4660(u32 self, u32 argument)
{
    u8 scratch[16];
    u32 index, rows, person, record, club, base, flags, result, date;
    u32 date_low;
    u16 year, other_year;
    s8 age;
    index = D(argument);
    rows = D(0x00ae23bcu);
    person = rows + index * 110u;
    if (person == 0u) return 1u;
    index = D(person);
    base = D(0x00ae2364u);
    if ((s32)index >= (s32)base) return 1u;
    base = D(0x00ae2d0cu);
    index = D(base + index * 79u);
    if ((s32)index < 0) return 1u;
    base = D(self);
    record = base + index * 80u;
    if (record == 0u) return 1u;
    if ((B(record + 0x45u) & 2u) != 0u) return 1u;
    index = D(record + 4u);
    result = D(argument + 4u);
    if (result != index) return 1u;
    base = D(0x00ae23b4u);
    club = base + index * 581u;
    index = D(record);
    person = rows + index * 110u;
    result = D(person + 0x61u);
    if (result == 0u) {
        result = D(person + 0x69u);
        if (result != 0u) return 1u;
    }
    if ((B(argument + 0x35u) & 0x3fu) == 0u) return 0u;
    if (B(record + 0x49u) != 0u) return 0u;
    result = cm_fn_00544520(person, 0u);
    if (result != 0u) return 0u;
    if ((B(record + 0x4fu) & 0xf0u) == 0x70u) return 0u;
    age = C(person + 0x18u);
    if (age < 25) {
        index = D(person);
        base = D(0x00ae2d0cu);
        if ((B(base + index * 79u + 0xbu) & 0x0fu) == 0u) {
            flags = D(record + 0x45u);
            if ((flags & 0xfdu) == 0u && (flags & 0x100u) == 0u)
                return 1u;
        }
    }
    result = cm_fn_00866f60((volatile u32 *)record);
    if (result != 0u) return 0u;
    if ((B(record + 0x4eu) & 8u) != 0u) return 0u;
    if (age <= 23) return 1u;
    if (C(record + 0x3fu) >= 3) return 1u;
    if (C(record + 0x3du) >= -50) return 1u;
    result = D(club + 0xcfu);
    if (result == 0u) return 1u;
    result = cm_fn_0054c8e0(0x00ae2c90u, (u32)scratch + 8u, 150u);
    date_low = D(result);
    result = D(result + 4u);
    D((u32)scratch + 4u) = result;
    date = D(club + 0xcfu) + 0x3eu;
    D((u32)scratch) = date_low;
    year = W((u32)scratch + 2u);
    other_year = W(date + 2u);
    if (year > other_year) return 0u;
    if (year < other_year) return 1u;
    return (s16)(u16)date_low > (s16)W(date) ? 0u : 1u;
}
