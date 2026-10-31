#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00685d30(u8 *self, u8 *child, u32 value);
extern void CM_THISCALL cm_fn_004204d0(u8 *self, u8 value);

u32 CM_THISCALL cm_fn_00420110(u8 *self, u8 *child, u32 value)
{
    u32 helper_result = cm_fn_00685d30(self, child, value);
    s16 child_type;
    u8 type_code;
    u8 child_code;
    u32 count;
    u32 saved_self_field;
    u32 table;
    u32 child_object;
    u32 vtable;
    u32 slot;
    void (CM_THISCALL *method)(u8 *self, u8 *child, u32 value);

    child_code = *(volatile u8 *)(child + 0x42);
    if (child_code != 0xffu)
        return 1;

    child_type = *(volatile s16 *)(child + 0x34);
    if (child_type < 10) {
        type_code = 0;
    } else {
        type_code = (u8)(1u + (child_type >= 0x16));
    }
    *(volatile u8 *)(child + 0x42) = type_code;

    for (;;) {
        s8 current_code = *(volatile s8 *)(child + 0x42);
        s32 current_count = *(volatile s32 *)(self + 0x30);
        u8 next;
        if (current_count >= current_code)
            break;
        next = (u8)(*(volatile u8 *)(self + 0x30) + 1u);
        cm_fn_004204d0(self, next);
    }

    table = *(volatile u32 *)(self + 0x0c);
    saved_self_field = *(volatile u32 *)(self + 8);
    *(volatile u32 *)(self + 8) = 0;
    count = (u32)(s32)*(volatile s8 *)(child + 0x42);
    child_object = *(volatile u32 *)(table + count * 4u);
    vtable = *(volatile u32 *)child_object;
    slot = *(volatile u32 *)(vtable + 4u);
    method = (void (CM_THISCALL *)(u8 *, u8 *, u32))slot;
    method((u8 *)child_object, child, value);
    *(volatile u32 *)(self + 8) = saved_self_field;
    *(volatile u8 *)(child + 0x42) = 0xffu;
    (void)helper_result;
    return 1;
}
