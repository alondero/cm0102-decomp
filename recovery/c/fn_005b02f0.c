#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 context, u32 argument);
extern u32 CM_CDECL cm_fn_00540a50(u32 first, u32 *argument_slot, u32 one);

u8 CM_THISCALL cm_fn_005b02f0(u32 self, u32 argument)
{
    u32 mutable_argument = argument;
    u32 self_plus4;
    u32 field39;
    u32 self_field39;
    u32 result;
    u32 index;
    u32 offset;
    u32 table;

    if (argument == 0u)
        return 0u;

    self_plus4 = *(volatile u32 *)(self + 4u);
    if (self_plus4 == 0u)
        return 0u;

    {
        s32 argument_value = *(volatile s32 *)argument;
        s32 global_value = CM_GLOBAL(volatile s32, 0x00ae2364);
        if (argument_value >= global_value)
            return 3u;
    }

    if (CM_GLOBAL(volatile u8, 0x009cc270) == 0u)
        return 3u;

    field39 = *(volatile u32 *)(self_plus4 + 0x39u);
    if (field39 != 0u) {
        if (*(volatile u32 *)(argument + 0x39u) == field39)
            return 3u;

        result = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), argument);
        CM_GLOBAL(volatile u32, 0x00ae1080) = result;
        if (result != 0u) {
            index = *(volatile u32 *)(result + 4u);
            table = CM_GLOBAL(volatile u32, 0x00ae23b4);
            offset = index * 581u;
            result = table + offset;
        } else {
            result = 0u;
        }

        self_plus4 = *(volatile u32 *)(self + 4u);
        self_field39 = *(volatile u32 *)(self_plus4 + 0x39u);
        if (result == self_field39)
            return 3u;
    } else {
        self_field39 = 0u;
    }

    {
        u32 argument_field24 = *(volatile u32 *)(argument + 0x24u);
        if (argument_field24 != 0u) {
            u32 self_field24 = *(volatile u32 *)(self_plus4 + 0x24u);
            if (argument_field24 == self_field24)
                return 3u;
        }
    }

    field39 = *(volatile u32 *)(argument + 0x39u);
    if (field39 != 0u && self_field39 != 0u) {
        result = cm_fn_00540a50(field39, &mutable_argument, 1u);
        self_plus4 = *(volatile u32 *)(self + 4u);
        if (result == *(volatile u32 *)(self_plus4 + 0x39u))
            return 3u;
    }

    index = *(volatile u32 *)argument;
    table = *(volatile u32 *)self;
    offset = index * 9u;
    return *(volatile u8 *)(table + offset);
}
