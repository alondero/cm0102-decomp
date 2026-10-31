/* Faithful C recovery at preferred-image VA 0043f210. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004a4a30(void *self, u32 arg);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_008e8a60(void *self, u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_0043f210(void *self, u32 first, u32 second)
{
    u32 right = second;
    u32 left = first;
    u32 data = *(volatile u32 *)(right + 0x14u);
    if (data == 0)
        return 0;

    u32 left61 = *(volatile u32 *)(left + 0x61u);
    if (left61 == 0)
        return data & 0xffffff00u;

    u32 item = *(volatile u32 *)data;
    u32 sentinel = CM_GLOBAL(volatile u32, 0x009cf684);
    if (item == sentinel)
        return data & 0xffffff00u;

    u32 owner = CM_GLOBAL(volatile u32, 0x00ae2c74);
    u32 checked = (u32)cm_fn_005c6710((void *)owner, data);
    if (checked != 0)
        return checked & 0xffffff00u;

    data = *(volatile u32 *)(right + 0x14u);
    u32 table = CM_GLOBAL(volatile u32, 0x00adadfc);
    u32 index = *(volatile u32 *)data;
    left61 = *(volatile u32 *)(left + 0x61u);
    u32 slot = *(volatile u32 *)(table + index * 4u);
    u32 receiver = *(volatile u32 *)(slot + 8u);
    u32 found = cm_fn_004a4a30((void *)receiver, left61);
    if (found != 0) {
        u32 left39 = *(volatile u32 *)(left + 0x39u);
        if (left39 != 0) {
            u32 found_value = *(volatile u32 *)(found + 8u);
            u32 left_value = *(volatile u32 *)left39;
            if (found_value != left_value)
                return (found_value & 0xffffff00u) | 4u;
        }
    }
    return cm_fn_008e8a60(self, left, right);
}
