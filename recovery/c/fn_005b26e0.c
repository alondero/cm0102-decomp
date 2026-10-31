#include "cmre.h"

extern int CM_CDECL cm_fn_00946a22(u32 pointer, u32 offset, u32 zero);
extern u32 CM_CDECL cm_fn_00945bfe(u8 *destination, u32 count, u32 flag, u32 *source);

u32 CM_CDECL cm_fn_005b26e0(u32 *source_slot, const u8 *record, s16 expected,
                            u32 base, u8 *destination, u32 limit)
{
    u16 record_type;
    u16 record_length;
    s32 limit_minus_one;
    s32 count;
    u32 offset;
    u32 source;
    u32 copied;

    record_type = *(volatile u16 *)(record + 6);
    if ((u32)record_type != (u32)(s32)expected)
        return 0;

    limit_minus_one = (s32)(limit - 1u);
    record_length = *(volatile u16 *)(record + 8);
    count = (s32)record_length;
    if (count >= limit_minus_one)
        count = limit_minus_one;
    if (count <= 0)
        return 0;

    offset = (u32)*(volatile u16 *)(record + 0x0a) + base;
    source = *(volatile u32 *)source_slot;
    if (source == 0)
        return 0;
    if (cm_fn_00946a22(source, offset, 0) != 0)
        return 0;

    source = *(volatile u32 *)source_slot;
    copied = cm_fn_00945bfe(destination, (u32)count, 1, (u32 *)source);
    if (copied != 1)
        return 0;
    *(volatile u8 *)(destination + (u32)count) = 0;
    if (*(volatile u8 *)destination == 0)
        return 0;
    return 1;
}
