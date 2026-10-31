#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0068d0d0(u8 *self, u32 destination, u32 size);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u8 *self);

u32 CM_THISCALL cm_fn_00775580(u32 *self, u8 *argument)
{
    u32 first = *(volatile u32 *)self;
    if (first == 0) {
        return 0;
    }

    if (argument == 0) {
        return first & 0xffffff00u;
    }

    u32 first_value = *(volatile u32 *)(u32)(first + 2u);
    *(volatile u32 *)(u32)(first + 0x0au) = first_value;
    u32 count = 0;
    u32 current = cm_fn_0068d1b0((u8 *)(u32)*(volatile u32 *)self);

    while (current != 0) {
        u32 arg_value = *(volatile u32 *)(u32)(argument + 2);
        *(volatile u32 *)(u32)(argument + 0x0a) = arg_value;
        u32 candidate = cm_fn_0068d1b0(argument);

        while (candidate != 0) {
            u32 key = *(volatile u32 *)(u32)candidate;
            u32 current_key = *(volatile u32 *)(u32)(current + 4u);
            if (key == current_key) {
                break;
            }
            candidate = cm_fn_0068d1b0(argument);
        }

        if (candidate == 0) {
            u32 destination = current + 4u;
            (void)cm_fn_0068d0d0(argument, destination, 4u);
            count = (count + 1u) & 0xffu;
        }

        current = cm_fn_0068d1b0((u8 *)(u32)*(volatile u32 *)self);
    }

    return (current & 0xffffff00u) | count;
}
