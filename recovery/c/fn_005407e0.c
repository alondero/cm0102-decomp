/* WIP: return-width gate: six callers copy EAX before later AX-width uses.
   Both differential builds pass, but acceptance requires resolving that proof. */
#include "cmre.h"

/* Preferred-image VA 0x005407e0, base 0x00400000, CM0102 3.9.68,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Callers consume the signed AX result. The halves truncate toward zero
   separately, before addition: this is not the average of the two inputs. */
extern u8 CM_CDECL cm_fn_005408c0(u32 first, u32 second);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))

s16 CM_CDECL cm_fn_005407e0(u32 first, u32 second, u32 mode)
{
    u8 kind;
    u32 record, base;
    s32 upper, lower;

    if (mode != 0) {
        kind = cm_fn_005408c0(first, second);
        record = U32_AT(first, 0x69);
        base = 8;
    } else {
        kind = cm_fn_005408c0(first, second);
        record = U32_AT(first, 0x61);
        base = 9;
    }
    if (kind == 4)
        return S16_AT(record, base + 4);
    if (kind == 2)
        return S16_AT(record, base);
    if (kind == 0)
        return S16_AT(record, base + 2);

    upper = S16_AT(record, base + (kind == 3 ? 4 : 2));
    lower = S16_AT(record, base);
    return (s16)(upper / 2 + lower / 2);
}
