#include "cmre.h"

extern u32 CM_CDECL cm_fn_0090c510(void);
extern u32 CM_THISCALL cm_fn_008029f0(u32 self, u32 mixed_status);
extern void CM_THISCALL cm_fn_00802a50(u32 self, u32 mixed_status);

u8 CM_THISCALL cm_fn_006d8c70(u8 *self, u32 *array)
{
    u32 time = cm_fn_0090c510();
    for (s32 index = 0; index < 16; ++index) {
        u32 slot = *(volatile u32 *)((u32)array + (u32)index * 4u);
        if (slot == 0)
            continue;

        u32 start = *(volatile u32 *)((u32)self + 0x2d6cu + (u32)index * 4u);
        u32 length = *(volatile u32 *)((u32)self + 0x2dacu);
        u32 end = start + length;
        u8 outside;
        if (time > end) {
            outside = 1;
        } else if (time < start) {
            outside = 1;
        } else {
            outside = 0;
        }

        u8 status = *(volatile u8 *)((u32)self + 0x10u + (u32)index);
        if (status == 0xffu)
            goto test_outside;
        if (status == 0xfeu)
            return 0;

        u32 first_arg = (start & 0xffffff00u) | status;
        u32 helper_result = cm_fn_008029f0(0x00b74450u, first_arg);
        if (helper_result != 0)
            return 0;
        u32 second_status = *(volatile u8 *)((u32)self + 0x10u + (u32)index);
        u32 second_arg = (helper_result & 0xffffff00u) | second_status;
        cm_fn_00802a50(0x00b74450u, second_arg);
        *(volatile u8 *)((u32)self + 0x10u + (u32)index) = 0xffu;

test_outside:
        if (outside == 0)
            return 0;
    }
    return 1;
}
