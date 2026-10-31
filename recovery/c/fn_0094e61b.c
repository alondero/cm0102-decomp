/* Preferred-image VA 0x0094e61b, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_CDECL cm_fn_00948835(void);
extern u32 CM_CDECL cm_fn_00951d4a(u32 byte_value);

u32 CM_CDECL cm_fn_0094e61b(void)
{
    u32 cursor;
    u8 value;
    if (!D(0x00de4630u)) cm_fn_00948835();
    cursor = D(0x00de4628u);
    value = B(cursor);
    if (value == 0x22u) {
        for (;;) {
            value = B(cursor + 1u);
            ++cursor;
            if (value == 0x22u || !value) break;
            if (cm_fn_00951d4a(value)) ++cursor;
        }
        if (B(cursor) == 0x22u) ++cursor;
    } else if (value > 0x20u) {
        do { ++cursor; } while (B(cursor) > 0x20u);
    }
    for (;;) {
        value = B(cursor);
        if (!value || value > 0x20u) return cursor;
        ++cursor;
    }
}
