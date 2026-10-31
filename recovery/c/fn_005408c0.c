#include "cmre.h"

/* Preferred-image VA 0x005408c0, image base 0x00400000, 3.9.68 executable
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Return width: all ten direct call sites consume AL. Eight use MOVSX EAX,AL;
   the two in 0x005407e0 compare AL before producing their own AX result. */
extern u32 CM_CDECL cm_fn_0053d980(u32 object, u32 owner, u32 mode);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))

u8 CM_CDECL cm_fn_005408c0(u32 first, u32 second)
{
    u32 first_link, second_link, first_owner, second_owner;
    u32 first_group, second_group, threshold, value, detail;
    s32 first_value, second_value;

    if (second == 0)
        return 4;
    first_link = U32_AT(first, 0x39);
    if (first_link != 0) {
        second_link = U32_AT(second, 0x39);
        if (first_link == second_link)
            return 0;
        second_link = U32_AT(second, 0x39);
        if (second_link != 0) {
            first_group = U32_AT(first_link, 0x53);
            second_group = U32_AT(second_link, 0x53);
            if (first_group == second_group)
                return 0;
        }
    }

    first_owner = U32_AT(first, 0x1a);
    second_owner = U32_AT(second, 0x1a);
    if (first_owner == second_owner)
        return 2;
    if (cm_fn_0053d980(second, first_owner, 1) == 0)
        return 2;
    first_link = U32_AT(first, 0x39);
    if (first_link != 0) {
        first_group = U32_AT(first_link, 0x53);
        if (cm_fn_0053d980(second, first_group, 1) == 0)
            return 3;
    }

    threshold = CM_GLOBAL(volatile u32, 0x00ae2364);
    value = U32_AT(second, 0);
    if ((s32)value >= (s32)(threshold - 0x10u))
        return 4;
    detail = U32_AT(first, 0x61);
    if (detail == 0)
        return 4;
    second_link = U32_AT(second, 0x39);
    if (second_link == 0)
        return 4;
    first_value = S16_AT(detail, 9);
    second_value = S16_AT(second_link, 0x80);
    return first_value + second_value >= 0x36b0 ? 3 : 4;
}
