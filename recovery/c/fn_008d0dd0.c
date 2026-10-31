/* Preferred-image VA 008d0dd0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 person);
extern u32 CM_CDECL cm_fn_00946464(u32 key, u32 base, u32 count,
                                  u32 size, u32 comparator);

u32 CM_THISCALL cm_fn_008d0dd0(u32 self, u32 person)
{
    volatile u8 key[16];
    u32 id, flags, receiver, found, index, country, count, base, result;
    u8 value;
    id = D(person);
    flags = D(self + 0x86eu);
    if ((B(flags + id * 8u) & 8u) == 0) return 0;
    receiver = D(0x00ae1050u);
    found = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = found;
    if (found == 0) return 0;
    index = D(found + 4u);
    base = D(0x00ae23b4u);
    country = base + index * 581u;
    if (country == 0) return 0;
    id = D(person);
    receiver = D(0x00ae1050u);
    D((u32)key + 9u) = id;
    found = cm_fn_004cf5a0(receiver, person);
    D(0x00ae1080u) = found;
    if (found != 0) {
        index = D(found + 4u);
        base = D(0x00ae23b4u);
        country = base + index * 581u;
    } else {
        country = 0;
    }
    id = D(country);
    count = D(self + 0x888u);
    base = D(self + 0x880u);
    D((u32)key + 1u) = id;
    /* The native comparator at 008e54d0 reads only key+9 and key+1.
       Leave the remaining bytes unwritten, as in the original. */
    result = cm_fn_00946464((u32)key, base, count, 0xdu, 0x008e54d0u);
    if (result != 0) return result;
    id = D(person);
    flags = D(self + 0x86eu);
    value = B(flags + id * 8u);
    B(flags + id * 8u) = (u8)(value + 0xf8u);
    return 0;
}
