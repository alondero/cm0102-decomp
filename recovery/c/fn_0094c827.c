#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_00951183(u32 index, s32 low,
                                                  s32 high, u32 mode);

unsigned long long CM_CDECL cm_fn_0094c827(volatile u8 *stream)
{
    u32 index = *(volatile u32 *)(stream + 0x10);
    u32 initial_low;
    s32 initial_high;
    u32 count;
    u32 flags;
    u32 map_entry_address;
    u32 map_index;
    u32 map_pointer;
    u32 map_flags;
    u32 start;
    u32 end;
    u32 output_position;
    unsigned long long initial_result;
    unsigned long long second_result;

    if (*(volatile s32 *)(stream + 4) < 0)
        *(volatile u32 *)(stream + 4) = 0;

    initial_result = cm_fn_00951183(index, 0, 0, 1);
    initial_low = (u32)initial_result;
    initial_high = (s32)(initial_result >> 32);
    if (initial_high < 0)
        return 0xffffffffffffffffull;

    if ((*(volatile u16 *)(stream + 0x0c) & 0x108u) == 0) {
        s32 current = *(volatile s32 *)(stream + 4);
        return initial_result - (unsigned long long)(long long)current;
    }

    end = *(volatile u32 *)stream;
    start = *(volatile u32 *)(stream + 8);
    count = end - start;
    flags = *(volatile u32 *)(stream + 0x0c);
    map_index = index;
    if ((flags & 3u) != 0) {
        map_pointer = *(volatile u32 *)(0x00de42e0u +
                                         (u32)((s32)index >> 5) * 4u);
        map_entry_address = map_pointer + 4u + (index & 0x1fu) * 8u;
        map_flags = *(volatile u8 *)map_entry_address;
        if ((map_flags & 0x80u) != 0) {
            u32 cursor = start;
            while (cursor < end) {
                if (*(volatile u8 *)cursor == 0x0a)
                    ++count;
                ++cursor;
            }
        }
    } else if ((flags & 0x80u) == 0) {
        CM_GLOBAL(volatile u32, 0x00de2f8c) = 0x16;
        return 0xffffffffffffffffull;
    }

    if (initial_result == 0)
        return count;

    if ((*(volatile u8 *)(stream + 0x0c) & 1u) != 0) {
        u32 current = *(volatile u32 *)(stream + 4);
        if (current == 0) {
            count &= current;
        } else {
            output_position = end - start + current;

            map_entry_address = 0x00de42e0u +
                                (u32)((s32)index >> 5) * 4u;
            map_pointer = *(volatile u32 *)map_entry_address;
            map_index = index & 0x1fu;
            map_flags = *(volatile u8 *)(map_pointer + map_index * 8u + 4u);
            if ((map_flags & 0x80u) != 0) {
                second_result = cm_fn_00951183(index, 0, 0, 2);
                if (second_result == initial_result) {
                    u32 base = *(volatile u32 *)(stream + 8);
                    u32 cursor = base;
                    u32 scan_end = base + output_position;
                    while (cursor < scan_end) {
                        if (*(volatile u8 *)cursor == 0x0a)
                            ++output_position;
                        ++cursor;
                    }
                    flags = *(volatile u8 *)(stream + 0x0d) & 0x20u;
                    if (flags != 0)
                        ++output_position;
                } else {
                    (void)cm_fn_00951183(index, initial_low,
                                         initial_high, 0);
                    if (output_position > 0x200u) {
                        output_position = *(volatile u32 *)(stream + 0x18);
                    } else {
                        u32 clamp_flags = *(volatile u32 *)(stream + 0x0c);
                        if ((clamp_flags & 8u) == 0 || (clamp_flags & 0x400u) != 0)
                            output_position = *(volatile u32 *)(stream + 0x18);
                        else
                            output_position = 0x200u;
                    }
                    map_pointer = *(volatile u32 *)map_entry_address;
                    map_flags = *(volatile u8 *)(map_pointer + map_index * 8u + 4u);
                    if ((map_flags & 4u) != 0)
                        ++output_position;
                }
            }
            initial_result -= output_position;
        }
    }

    return initial_result + count;
}
