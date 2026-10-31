/* Preferred-image VA 0x00728e90/base 0x00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL3/full EAX: the original keeps the third argument in EAX throughout.
   EBP is reloaded from the actual first formal on every back edge. The
   transient MOVZX BP value is compared as a signed WORD. Type 8 falls
   through a second comparison with 7 and is ignored, as in the original. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u32 CM_CDECL cm_fn_00728e90(volatile u32 match_arg, u32 index, u32 output)
{
    u32 match, table, events, cursor, i = 0;
    u8 side, kind, value, time;
    if (output == 0) return output;
    match = match_arg;
    if (match == 0) return output;
    B(output + 0x43u) = 0;
    B(output + 0x44u) = 0;
    B(output + 0x4bu) = 0;
    B(output + 0x4cu) = 0;
    B(output + 0x49u) = 0xff;
    B(output + 0x4au) = 0xff;
    B(output + 0x46u) = 0;
    B(output + 0x45u) = 0;
    table = D(0x00b67a50);
    table = D(table + 0x19u);
    events = D(table + index * 105u + 0x5fu);
    if (events == 0) return output;
    cursor = events + 0x59u;
    while ((s32)i < 50) {
        side = B(cursor - 2u);
        if (side == 0xff) return output;
        if (B(match + 0x67u) == 0xff) goto next;
        table = D(match + 0x5fu);
        if (table == 0) goto next;
        time = B(cursor + 1u);
        if ((s16)time > (s16)W(table + 0x4du)) goto next;
        kind = B(cursor);
        if (kind == 1 || kind == 3) {
            if (side != 0) goto other;
            value = B(output + 0x43u);
            match = match_arg;
            ++value;
            ++i;
            B(output + 0x43u) = value;
            cursor += 4u;
            continue;
        }
        if (kind == 2) {
            if (side == 0) goto other;
            value = B(output + 0x43u);
            match = match_arg;
            ++value;
            ++i;
            B(output + 0x43u) = value;
            cursor += 4u;
            continue;
        }
        if (kind == 7) {
            u32 address = output + (side == 0 ? 0x49u : 0x4au);
            value = B(address);
            if (value == 0xff) {
                match = match_arg;
                ++i;
                B(address) = 1;
            } else {
                match = match_arg;
                ++value;
                ++i;
                B(address) = value;
            }
            cursor += 4u;
            continue;
        }
        goto next;
other:
        value = B(output + 0x44u);
        B(output + 0x44u) = (u8)(value + 1u);
next:
        match = match_arg;
        ++i;
        cursor += 4u;
    }
    return output;
}
