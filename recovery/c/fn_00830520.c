#include "cmre.h"

extern void CM_THISCALL cm_fn_00417c10(u32 *self, u32 *output, u16 *source);
extern short *CM_THISCALL cm_fn_00549f70(short *self, short a, char b,
                                        u16 c, u32 d);

u32 CM_CDECL cm_fn_00830520(void)
{
    volatile u8 local[0x10];
    u32 target;
    u32 slot_address;
    u32 entry;
    u32 index;
    u32 object_field;
    u32 object_value;
    u32 result_ptr;
    u32 value0;
    u32 value1;
    u16 local_high;
    u16 global_high;
    s16 local_low;
    s16 global_low;

    target = CM_GLOBAL(volatile u32, 0x00ae2d08);
    slot_address = 0x00b63d60;
    index = 0;
    while (slot_address < 0x00b64738) {
        entry = *(volatile u32 *)slot_address;
        if (entry != 0) {
            if (entry == target) {
                cm_fn_00417c10((u32 *)0x00dd6ec8,
                               (u32 *)local,
                               (u16 *)(0x00b63d8a + index * 0x48));
                object_field = *(volatile u32 *)(entry + 0x71);
                object_value = *(volatile u32 *)object_field;
                if (object_value != CM_GLOBAL(volatile u32, 0x009cfa10))
                    return 1;
                global_high = CM_GLOBAL(volatile u16, 0x00dd6eca);
                if (global_high != 0x07d1u)
                    return 1;

                result_ptr = (u32)cm_fn_00549f70(
                    (short *)(local + 8), 1, 8, 0x07d1u, 0xffffffffu);
                value0 = *(volatile u32 *)result_ptr;
                value1 = *(volatile u32 *)(result_ptr + 4);
                *(volatile u32 *)(local + 0) = value0;
                local_high = *(volatile u16 *)(local + 2);
                *(volatile u32 *)(local + 4) = value1;
                global_high = CM_GLOBAL(volatile u16, 0x00dd6eca);
                if (local_high < global_high)
                    goto second_pair;
                if (local_high > global_high)
                    return 1;
                local_low = (s16)(u16)value0;
                global_low = CM_GLOBAL(volatile s16, 0x00dd6ec8);
                if (local_low < global_low)
                    goto second_pair;
                return 1;

second_pair:
                result_ptr = (u32)cm_fn_00549f70(
                    (short *)(local + 8), 1, 8, 0x07d1u, 0xffffffffu);
                cm_fn_00417c10((u32 *)0x00dd6ec8,
                               (u32 *)local,
                               (u16 *)result_ptr);
                return 1;
            }
            if (target == 0 &&
                (*(volatile u8 *)(entry + 0x11c) & 2u) != 0) {
                cm_fn_00417c10((u32 *)0x00dd6ec8,
                               (u32 *)(local + 8),
                               (u16 *)(0x00b63d8a + index * 0x48));
                return 1;
            }
        }
        slot_address += 0x48;
        ++index;
    }
    return 0;
}
