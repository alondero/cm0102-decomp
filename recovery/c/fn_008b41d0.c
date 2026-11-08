#include "cmre.h"
#pragma GCC target("no-sse3")

extern u32 CM_THISCALL cm_fn_008b4cd0(u32 self, u32 person);

/* Preferred-image VA 0x008b41d0, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Incoming ECX is forwarded unchanged to the original 0x008b4cd0. */
u32 CM_THISCALL cm_fn_008b41d0(u32 self, u32 club, volatile u32 column)
{
    union { u32 bits; float value; } weights[6];
    u32 person, current, index, rows, total = 0u, count = 0u;
    s32 selected;
    long double value;
    u32 result;

    weights[0].bits = 0x3f800000u;
    weights[1].bits = 0x3fb9999au;
    weights[2].bits = 0x3fd9999au;
    weights[3].bits = 0x3ff33333u;
    weights[4].bits = 0x40033333u;
    weights[5].bits = 0x400ccccdu;
    if (club == 0u)
        return 0u;
    person = *(volatile u32 *)(club + 0xcfu);
    if (person == 0u)
        return (person & 0xffff0000u) | 100u;
    current = CM_GLOBAL(volatile u32, 0x00ae2364u);
    index = *(volatile u32 *)person;
    if ((s32)index < (s32)(current - 16u))
        return (person & 0xffff0000u) | 100u;

    rows = cm_fn_008b4cd0(self, person);
    /* The original reloads the BYTE argument after the original helper. */
    selected = (s32)*(volatile s8 *)&column;
    for (u32 offset = 0u; offset != 54u; offset += 9u) {
        if (*(volatile u32 *)(rows + offset) != 0xffffffffu) {
            u32 address_index = (u32)selected + offset;
            u8 score = *(volatile u8 *)(rows + address_index + 4u);
            if (score != 0u) {
                ++count;
                /* MOVSX CX,CL leaves ECX's high word intact. Only BP is
                   consumed after this loop, so retain the exact low-word sum. */
                total += (u32)(s32)(s8)score;
            }
        }
    }
    if (count != 0u) {
        column = (u32)(s32)(s16)total; /* original argument-slot store 0x008b4278 */
        value = (long double)(s32)column;
        value /= (long double)weights[count - 1u].value;
        /* Source cast lowered by original __ftol at 0x008b4287. */
        total = (u32)(long long)value;
    }
    column = (u32)(s32)(s16)total; /* original argument-slot store 0x008b4291 */
    value = (long double)(s32)column;
    value *= (long double)CM_GLOBAL(volatile double, 0x00970400u);
    /* Source cast lowered by original __ftol at 0x008b429f. */
    result = (u32)(long long)value;
    if ((s16)result < 50)
        return 50u;
    return result;
}
