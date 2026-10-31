#include "cmre.h"

typedef void (CM_CDECL *cm_fn_00937410_callback)(u32 context);

void CM_CDECL cm_fn_00937410(volatile u32 *state, u32 begin, u32 end)
{
    u32 saved_cursor;
    u32 frame;
    u32 callback_value;
    volatile u32 * volatile callback_table;
    u32 current;
    u32 byte_index;
    cm_fn_00937410_callback callback;

    saved_cursor = *state;
    frame = *(volatile u32 *)((volatile u8 *)state + 0x28);
    callback_value = *(volatile u32 *)((volatile u8 *)state + 0x2c);
    *state = begin;
    callback_table = *(volatile u32 * volatile *)((volatile u8 *)frame + 0x8c);

    if (callback_value != 0) {
        current = begin;
        if (current < end) {
            do {
                u32 kind = *(volatile u32 *)((volatile u8 *)frame + 0x70);
                if (kind == 0x00936f10)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 1;
                else if (kind == 0x00936f40)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 0;
                else if (kind == 0x00936e80)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 2;
                else if (kind == 0x00936eb0)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 3;
                else if (kind == 0x00936ee0)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 4;
                else if (kind == 0x00936f80)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 5;
                else if (kind == 0x00936f90)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 6;
                else if (kind == 0x00936ff0)
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 7;
                else
                    *(volatile u16 *)((volatile u8 *)state + 0x3c) = 0xffff;

                callback_value = *(volatile u32 *)((volatile u8 *)state + 0x2c);
                callback = (cm_fn_00937410_callback)callback_value;
                callback((u32)state);
                callback_value = *(volatile u32 *)((volatile u8 *)state + 0x2c);
                if (callback_value == 0)
                    break;

                current = *state;
                byte_index = *(volatile u8 *)current;
                *(volatile u32 *)((volatile u8 *)state + 4) = byte_index;
                callback_value = *(volatile u32 *)(callback_table + byte_index);
                *state = current + 1;
                callback = (cm_fn_00937410_callback)callback_value;
                callback((u32)state);
                current = *state;
            } while (current < end);
        }
        *state = saved_cursor;
    }
}
