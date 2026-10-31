#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);

u32 CM_CDECL cm_fn_00527f60(s32 amount)
{
    u32 old_count;
    u32 new_count;
    u32 difference;
    u32 i;
    u32 table;
    u32 result;
    u32 address;
    u32 value;

    old_count = CM_GLOBAL(volatile u32, 0x00ae237c);
    new_count = old_count + (u32)amount;
    result = cm_fn_00945667(CM_GLOBAL(volatile u32, 0x00ae23d4),
                            (u32)new_count * 0x1au);
    if (result == 0)
        return 0;

    old_count = CM_GLOBAL(volatile u32, 0x00ae237c);
    new_count = old_count + (u32)amount;
    table = CM_GLOBAL(volatile u32, 0x00ae2384);
    CM_GLOBAL(volatile u32, 0x00ae237c) = new_count;
    difference = new_count - table;
    CM_GLOBAL(volatile u32, 0x00ae23d4) = result;
    value = result + (u32)difference * 0x1au;
    CM_GLOBAL(volatile u32, 0x00ae23dc) = value;

    if (amount >= 1) {
        i = 1;
        do {
            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            new_count -= i;
            *(volatile u32 *)(address + new_count * 0x1au) = new_count;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            *(volatile u32 *)(address + new_count * 0x1au + 4) = 0;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            *(volatile u16 *)(address + new_count * 0x1au + 8) = 0;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            *(volatile u32 *)(address + new_count * 0x1au + 0x0a) = 0;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            *(volatile u32 *)(address + new_count * 0x1au + 0x0e) = 0;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            address = CM_GLOBAL(volatile u32, 0x00ae23d4);
            *(volatile u32 *)(address + new_count * 0x1au + 0x12) = 0;

            new_count = CM_GLOBAL(volatile u32, 0x00ae237c);
            new_count -= i;
            {
                s32 repeat;
                ++i;
                repeat = (s32)i <= amount;
                address = CM_GLOBAL(volatile u32, 0x00ae23d4);
                *(volatile u32 *)(address + new_count * 0x1au + 0x16) = 0;
                if (!repeat)
                    break;
            }
        } while (1);
    }
    return 1;
}
