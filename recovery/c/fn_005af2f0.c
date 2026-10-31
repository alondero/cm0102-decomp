/* Preferred-image VA 0x005AF2F0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: stdcall (two stack arguments, ret 8). Argument 1 at entry [esp+4] is
   a base pointer; the dword at base+0x5d is tested as a record pointer. Argument
   2 at [esp+8] is a 16-bit value (compared as a word).
   The record's first dword is compared, in order, with dwords at 0x009CF338,
   0x009CF2DC, 0x009CF244 and 0x009CF300. A match returns
   ((EAX_at_match & ~0xff) | (arg2 != 6)) + 1 as a full 32-bit value, where
   EAX_at_match is the record pointer, or the 0x009CF300 value for the last test.
   Otherwise the result is built from the last EAX value: its high word is kept,
   the low word is arg2, and the low byte is 1 for arg2 6 or 5, else 2.
   Three callers: 0x005aa814, 0x005aa85b, 0x005ab999. Global meanings unknown. */
#include "cmre.h"

u32 CM_STDCALL cm_fn_005af2f0(u8 *base, u16 param_2)
{
    u32 first;
    u32 *record;
    u32 last_eax = 0;
    u32 result;

    if (base == 0) {
        goto fall_through;
    }
    first = *(u32 *)(base + 0x5d);
    if (first == 0) {
        goto fall_through;
    }
    last_eax = first;
    record = (u32 *)first;

    if (*record == CM_GLOBAL(u32, 0x009cf338)) {
        return ((first & 0xffffff00u) | (u32)(param_2 != 6)) + 1u;
    }
    if (*record == CM_GLOBAL(u32, 0x009cf2dc)) {
        return ((first & 0xffffff00u) | (u32)(param_2 != 6)) + 1u;
    }
    if (*record == CM_GLOBAL(u32, 0x009cf244)) {
        return ((first & 0xffffff00u) | (u32)(param_2 != 6)) + 1u;
    }
    last_eax = CM_GLOBAL(u32, 0x009cf300);
    if (*record == last_eax) {
        return ((last_eax & 0xffffff00u) | (u32)(param_2 != 6)) + 1u;
    }

fall_through:
    /* mov ax, word [esp+8]: the high word of EAX is kept from last_eax. */
    result = (last_eax & 0xffff0000u) | (u32)param_2;
    /* mov al, 1 (ax == 6 or ax == 5) or mov al, 2 (otherwise). */
    if (param_2 == 6 || param_2 == 5) {
        result = (result & 0xffffff00u) | 1u;
    } else {
        result = (result & 0xffffff00u) | 2u;
    }
    return result;
}
