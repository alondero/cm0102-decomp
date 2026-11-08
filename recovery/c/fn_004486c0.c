/* CM0102 3.9.68 reconstruction. Preferred-image VA
 * 004486c0; base00400000; target SHA256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
 * Native008d3409 consumes AL then008d3415 kills full EAX before reuse.
 * Incoming ECX remains live through root into original00452710. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00452710(u32 self, u32 record, u32 create);

__attribute__((target("no-sse3")))
u8 CM_THISCALL cm_fn_004486c0(u32 context, volatile u32 person, volatile u32 flag)
{
    u32 original_person = person;
    u32 index, base, record, result;
    s32 word, value, linear;
    long double t;
    if (!original_person) return 0;
    index = CM_GLOBAL(volatile u32, original_person + 8u);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4);
    record = base + index * 0x245u;
    result = cm_fn_00452710(context, record, 1);
    if (!result || !record) return 0;
    word = CM_GLOBAL(volatile s16, record + 0x80u);
    value = CM_GLOBAL(volatile s32, original_person + 0x1bu);
    person = (u32)word; /* Original overwrites actual first caller argument slot. */
    linear = word * 50 + 500000;
    if (value < linear) return 0;
    if (flag) {
        t = (long double)word;
        t = t * t;
        t = t * CM_GLOBAL(volatile double, 0x00967880);
        /* Registered __ftol at0044872b, low32 compared signed at00448730.
         * Original retains a second x87 copy of word, popped at00448732. */
        if (value < (s32)(u32)(long long)t) return 0;
    }
    if (!value) return 0;
    return (u8)(value > CM_GLOBAL(volatile s32, result + 0x313u));
}
