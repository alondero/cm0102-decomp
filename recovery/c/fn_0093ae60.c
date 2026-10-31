#include "cmre.h"

void CM_CDECL cm_fn_0093ae60(volatile u32 *state)
{
    volatile u8 *s = (volatile u8 *)state;
    volatile u8 *object;
    volatile u8 *records;
    volatile u8 *record;
    volatile u8 *cursor;
    volatile u8 saved_previous;
    u32 original_cursor;
    u32 data_base;
    u16 count;
    s16 remaining;
    u8 previous_byte;
    u8 layer;
    u32 token;
    u32 dispatch;
    u16 length;

    cursor = (volatile u8 *)(u32)*(volatile u32 *)(s + 0x20);
    object = (volatile u8 *)(u32)*(volatile u32 *)(s + 0x28);
    cursor -= 4;
    *(volatile u32 *)(s + 0x20) = (u32)cursor;
    records = (volatile u8 *)(u32)*(volatile u32 *)(object + 0x94);
    previous_byte = *cursor;
    count = *(volatile u16 *)(object + 0xca);
    saved_previous = previous_byte;
    remaining = (s16)(count - 1u);
    record = records;
    if (remaining >= 0) {
        do {
            if (*(volatile u8 *)(record + 7) == previous_byte)
                break;
            record += 8;
            --remaining;
        } while (remaining >= 0);
        if (remaining < 0)
            record = (volatile u8 *)0;
    } else {
        record = (volatile u8 *)0;
    }

    original_cursor = *(volatile u32 *)s;
    layer = *(volatile u8 *)(object + 0xc9);
    data_base = *(volatile u32 *)(object + 0x9c + (u32)layer * 4u);
    if (record == (volatile u8 *)0) {
        count = *(volatile u16 *)(object + 0xca);
        records = (volatile u8 *)(u32)*(volatile u32 *)(object + 0x94);
        record = records + (u32)(s32)(s16)count * 8u;
        *(volatile u16 *)(object + 0xca) = (u16)(count + 1u);
        previous_byte = saved_previous;
    }
    *(volatile u8 *)(record + 6) = layer;
    *(volatile u8 *)(record + 7) = previous_byte;
    *(volatile u32 *)record = *(volatile u32 *)s - data_base;

    cursor = (volatile u8 *)(u32)*(volatile u32 *)s;
    token = *cursor;
    ++cursor;
    *(volatile u32 *)(s + 4) = token;
    *(volatile u32 *)s = (u32)cursor;
    while (token != 0x2d) {
        token = *(volatile u32 *)(s + 4);
        cursor = (volatile u8 *)(u32)*(volatile u32 *)s;
        if (token - 0x40u <= 0x7fu) {
            dispatch = CM_GLOBAL(volatile u8, 0x0093af6cu + token - 0x40u);
            switch (dispatch) {
            case 0:
                length = *cursor;
                cursor += (u32)length + 1u;
                break;
            case 1:
                length = *cursor;
                cursor += (u32)length * 2u + 1u;
                break;
            case 2:
                cursor += token - 0xafu;
                break;
            case 3:
                cursor += token * 2u - 0x16eu;
                break;
            default:
                break;
            }
        }
        *(volatile u32 *)s = (u32)cursor;
        token = *cursor;
        ++cursor;
        *(volatile u32 *)(s + 4) = token;
        *(volatile u32 *)s = (u32)cursor;
    }
    length = *(volatile u16 *)s - (u16)original_cursor;
    length = (u16)(length - 1u);
    *(volatile u16 *)(record + 4) = length;
}
