/* WIP: output memory mismatch, case 12, after five failed test iterations. */
#include "cmre.h"

void CM_CDECL cm_fn_00933fa0(void *arg1, void *arg2,
                             s32 arg3, s32 arg4, s32 arg5, s32 arg6)
{
    volatile u32 *array = *(volatile u32 * volatile *)((volatile u8 *)arg1 + 0x3c8);
    volatile u32 *table = *(volatile u32 * volatile *)((volatile u8 *)arg2 + 0x18);
    u32 masked;
    s32 difference_a;
    s32 difference_b;
    s32 table_delta;
    s32 table_b;
    s32 combined;
    s32 bound;
    s32 threshold;
    u32 value;
    u32 array4;
    u32 array3;
    u32 array6;
    u32 array5;
    u32 table3;
    u32 table4;
    u32 table5;

    masked = (table[arg6] + 0x20u) & 0xffffffc0u;
    array4 = array[arg4];
    table3 = table[arg3];
    array3 = array[arg3];
    difference_a = (s32)(array4 - array3);
    array6 = array[arg6];
    array5 = array[arg5];
    difference_b = (s32)(array6 - array5);
    if ((s32)(masked - table3) < 0xc0 ||
        difference_a <= -8 || difference_b <= -8)
        goto store_masked;

    table4 = table[arg4];
    table5 = table[arg5];
    table_delta = (s32)(table4 - table3);
    table_b = (s32)(masked - table5);
    combined = (s32)((u32)table_b + (u32)table_delta);
    threshold = (s32)((u32)difference_a + (u32)difference_b -
                      ((table_b >= 0x20) ? 0x20u : 7u));
    if (combined < threshold)
        goto store_masked;

    bound = (s32)((u32)difference_a + (u32)difference_b + 0x39u);
    if (combined > bound) {
        value = table[arg3] + 0x40u;
        table[arg3] = value;
        table[arg6] = masked;
        return;
    }

    threshold = table_b >= 0x20 ? 25 : 0;
    if (table_b > (s32)((u32)difference_b - (u32)threshold) ||
        table_delta <= (s32)((u32)difference_a + 0x19u))
        goto store_masked;

    table[arg3] = table[arg3] + 0x40u;
    masked += 0x40u;
store_masked:
    table[arg6] = masked;
}
