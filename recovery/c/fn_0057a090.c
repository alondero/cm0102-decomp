/* Preferred-image VA 0x0057a090, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004a4a30(volatile u8 *self, volatile u32 *input);
extern u32 CM_THISCALL cm_fn_008e8a60(u32 self, u32 person, u32 match);
u32 CM_THISCALL cm_fn_0057a090(u32 self, u32 person, u32 match)
{
    u32 type = *(volatile u32 *)(match + 0x14u);
    u32 id, input, table, entry, result, other, lhs, rhs;
    if (!type) goto fallback;
    id = *(volatile u32 *)type;
    if (id != CM_GLOBAL(volatile u32, 0x009cf74cu) &&
        id != CM_GLOBAL(volatile u32, 0x009cf750u)) goto fallback;
    input = *(volatile u32 *)(person + 0x61u);
    if (!input) goto fallback;
    table = CM_GLOBAL(volatile u32, 0x00adadfcu);
    entry = *(volatile u32 *)(table + id * 4u);
    result = cm_fn_004a4a30((volatile u8 *)*(volatile u32 *)(entry + 8u), (volatile u32 *)input);
    if (!result) goto fallback;
    other = *(volatile u32 *)(person + 0x39u);
    if (!other) goto fallback;
    lhs = *(volatile u32 *)(result + 8u);
    rhs = *(volatile u32 *)other;
    if (lhs != rhs) return (lhs & 0xffffff00u) | 1u;
fallback:
    return cm_fn_008e8a60(self, person, match);
}
