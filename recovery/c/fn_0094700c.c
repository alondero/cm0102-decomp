/* Preferred-image VA 0x0094700c, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0094b307(u32 stream);

u32 CM_CDECL cm_fn_0094700c(u32 destination, u32 length, u32 stream)
{
    u32 cursor = destination, remaining = length, value, count, next;
    if ((s32)remaining <= 0) return 0;
    --remaining;
    while (remaining) {
        count = D(stream + 4u) - 1u;
        D(stream + 4u) = count;
        if ((s32)count >= 0) {
            next = D(stream);
            value = B(next);
            D(stream) = next + 1u;
        } else {
            value = cm_fn_0094b307(stream);
        }
        if (value == 0xffffffffu) {
            if (cursor == destination) return 0;
            break;
        }
        B(cursor) = (u8)value;
        ++cursor;
        if ((u8)value == 10) break;
        --remaining;
    }
    B(cursor) &= 0;
    return destination;
}
