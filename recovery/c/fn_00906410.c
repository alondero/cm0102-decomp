#include "cmre.h"

/* Preferred-image VA 0x00906410, CM0102 3.9.68, SHA-256
   0c78e7da...f86. Field offsets and read order follow the original listing.
   The table index arithmetic and comparator subtraction wrap at 32 bits. */
extern s32 CM_CDECL cm_fn_00540080(u32 *arg1, u32 *arg2);

#define U32_AT(base, offset) (*(volatile u32 *)((base) + (offset)))
#define S16_AT(base, offset) (*(volatile s16 *)((base) + (offset)))
#define S8_AT(base, offset) (*(volatile s8 *)((base) + (offset)))

s32 CM_CDECL cm_fn_00906410(u32 *arg1, u32 *arg2)
{
    u32 second = *(volatile u32 *)arg2;
    u32 first = *(volatile u32 *)arg1;
    u32 second_link = U32_AT(second, 0x53);
    u32 first_link = U32_AT(first, 0x53);
    u32 second_index, first_index, table;
    s8 first_byte, second_byte;
    s32 second_value, first_value;

    if (first_link != second_link) {
        u32 first_value = U32_AT(first_link, 0);
        u32 second_value = U32_AT(second_link, 0);
        return (s32)(first_value - second_value);
    }

    first_index = U32_AT(first, 0x1db);
    second_index = U32_AT(second, 0x1db);
    if (first_index != second_index) {
        table = CM_GLOBAL(volatile u32, 0x00ae23d0);
        second_value = S16_AT(table + second_index * 0x6bu, 0x69);
        first_value = S16_AT(table + first_index * 0x6bu, 0x69);
        return second_value - first_value;
    }

    first_byte = S8_AT(first, 0x1df);
    second_byte = S8_AT(second, 0x1df);
    if (first_byte != second_byte)
        return (s32)first_byte - (s32)second_byte;

    return cm_fn_00540080(arg1, arg2);
}
