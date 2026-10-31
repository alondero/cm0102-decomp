#include "cmre.h"
/* WIP: the comparator at 0x00757ee0 is an interior code target passed to the
   external search helper; the C semantic harness escapes when that helper
   invokes it, while the original proceeds to its data fault. */

extern void CM_FASTCALL cm_fn_0054a110(u16 *param);
extern u32 CM_CDECL cm_fn_00946464(u32 local, u32 base, u32 count,
                                    u32 stride, void *compare);

u32 CM_THISCALL cm_fn_00757da0(
    volatile u8 *self, volatile u32 *param2, volatile u32 *param3,
    s32 param4, s16 param5, volatile u32 *param6)
{
    volatile u8 local[0x20];
    s32 count;
    s32 index;
    u32 base;
    u32 entry;
    u32 found;
    u32 first;
    u32 second;
    u16 bound_high;
    u16 found_high;
    s16 bound_low;
    s16 found_low;

    cm_fn_0054a110((u16 *)(local + 8));
    if ((u32)param4 == 0x0bdfu) {
        count = *(volatile s32 *)(self + 8);
        if (count > 0) {
            base = *(volatile u32 *)self;
            bound_low = param5;
            entry = base + 0x0c;
            for (index = 0; index < count; ++index) {
                if (*(volatile u32 *)(entry + 7) == 0x0bdfu &&
                    *(volatile u32 *)entry == *param3 &&
                    *(volatile u16 *)(entry + 5) == (u16)bound_low)
                    return 1;
                entry += 0x17;
            }
        }
        return 0;
    }

    if (param2 != (volatile u32 *)0)
        *(volatile u32 *)(local + 0x10) = *param2;
    else
        *(volatile u32 *)(local + 0x10) = 0xffffffffu;
    *(volatile u32 *)(local + 0x1b) = (u32)param4;
    count = *(volatile s32 *)(self + 8);
    *(volatile u32 *)(local + 0x18) = *param3;
    *(volatile u16 *)(local + 0x1d) = (u16)param5;
    base = *(volatile u32 *)self;
    found = cm_fn_00946464((u32)(local + 8), base, (u32)count,
                            0x17, (void *)0x00757ee0);
    if (found == 0)
        return 0;

    if (param6 != (volatile u32 *)0) {
        first = param6[0];
        second = param6[1];
        *(volatile u32 *)(local + 0x00) = first;
        bound_high = *(volatile u16 *)(local + 0x02);
        *(volatile u32 *)(local + 0x04) = second;
        found_high = *(volatile u16 *)(found + 2);
        if (bound_high > found_high)
            return 0;
        if (bound_high < found_high)
            return 1;
        bound_low = (s16)(u16)first;
        found_low = *(volatile s16 *)found;
        if (bound_low > found_low)
            return 0;
    }
    return 1;
}
