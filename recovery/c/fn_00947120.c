#include "cmre.h"
extern u32 CM_CDECL cm_fn_009465fc(u32 value);

s32 CM_CDECL cm_fn_00947120(const u8 *left, const u8 *right, s32 limit)
{
    if (limit == 0) return 0;
    if (CM_GLOBAL(volatile u32, 0x00de3048) == 0) {
        u32 remaining = (u32)limit;
        u8 a = *(volatile u8 *)left;
        u8 b = *(volatile u8 *)right;
        for (;;) {
            if (a == 0 || b == 0) break;
            ++left;
            ++right;
            if (a >= 0x41u && a <= 0x5au) a = (u8)(a + 0x20u);
            if (b >= 0x41u && b <= 0x5au) b = (u8)(b + 0x20u);
            if (a != b) return a < b ? -1 : 1;
            --remaining;
            if (remaining == 0) break;
            a = *(volatile u8 *)left;
            b = *(volatile u8 *)right;
        }
        return a == b ? 0 : (a < b ? -1 : 1);
    }

    u32 remaining = (u32)limit;
    u32 first_value = 0;
    u32 second_value = 0;
    for (;;) {
        first_value = (first_value & 0xffffff00u) | *(volatile u8 *)left;
        second_value = (second_value & 0xffffff00u) | *(volatile u8 *)right;
        if (first_value == 0 || second_value == 0) break;
        ++left;
        ++right;
        u32 second_result = cm_fn_009465fc(second_value);
        u32 first_result = cm_fn_009465fc(first_value);
        first_value = first_result;
        second_value = second_result;
        if (first_result != second_result)
            return first_result < second_result ? -1 : 1;
        --remaining;
        if (remaining == 0) break;
    }
    return first_value == second_value ? 0 :
        (first_value < second_value ? -1 : 1);
}
