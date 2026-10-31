/* Preferred-image VA 0x00951bdd, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00947b09(u32 code);
extern u32 CM_CDECL cm_fn_00952f5b(u32 string);

u32 CM_CDECL cm_fn_00951bdd(u32 source)
{
    u32 count = 0, cursor, value, output, destination;
    if (!source) return 0;
    value = D(source);
    cursor = source + 4u;
    if (value) {
        do {
            value = D(cursor);
            ++count;
            cursor += 4u;
        } while (value);
    }
    output = (u32)cm_fn_00944e46(count * 4u + 4u);
    destination = output;
    if (!output) cm_fn_00947b09(9);
    value = D(source);
    cursor = source;
    while (value) {
        cursor += 4u;
        D(destination) = cm_fn_00952f5b(value);
        value = D(cursor);
        destination += 4u;
    }
    D(destination) &= 0;
    return output;
}
