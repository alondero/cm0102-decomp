/* Preferred-image VA 0x00951ce0, CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Input policy v2 supplies the clear direction flag used by the first scan. */
#include "cmre.h"

u8 * CM_CDECL cm_fn_00951ce0(u8 *string, s32 character)
{
    u32 cursor = (u32)string;
    u32 remaining = 0xffffffffu;
    u8 value;
    u8 target = (u8)character;

    /* 00951cec: REPNE SCASB, including its decrement on the matching NUL. */
    while (remaining != 0) {
        value = *(volatile u8 *)cursor;
        cursor += 1u;
        remaining -= 1u;
        if (value == 0)
            break;
    }

    remaining = 0u - (remaining + 1u);
    cursor -= 1u;

    /* 00951cf6 scans backward from the terminating NUL, including that byte. */
    while (remaining != 0) {
        value = *(volatile u8 *)cursor;
        cursor -= 1u;
        remaining -= 1u;
        if (value == target)
            break;
    }

    cursor += 1u;
    /* The separate original CMP rereads the last byte even after no match. */
    if (*(volatile u8 *)cursor != target)
        return (u8 *)0;
    return (u8 *)cursor;
}
