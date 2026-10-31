/* WIP: return mismatch remains in flagged offset/correction path; case 324
   returns 0xfcffff9d in the original and 0x1771cf3d in C. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_0094c552(u32 file, u32 origin, u32 mode);

s32 CM_CDECL cm_fn_00946aae(volatile u8 *file)
{
    u32 saved_file;
    s32 result;
    u32 length;
    u32 start;
    u32 end;
    u32 flags;
    u32 index;
    u32 table;
    u32 position;
    u32 count;
    u32 bit_offset;
    u32 requested;
    u8 byte;

    saved_file = *(volatile u32 *)(file + 0x10);
    if (*(volatile s32 *)(file + 4) < 0)
        *(volatile u32 *)(file + 4) = 0;
    result = cm_fn_0094c552(saved_file, 0, 1);
    if (result < 0)
        return -1;

    flags = *(volatile u32 *)(file + 0x0c);
    if ((flags & 0x108) == 0)
        return (s32)((u32)result - *(volatile u32 *)(file + 4));

    end = *(volatile u32 *)file;
    start = *(volatile u32 *)(file + 8);
    length = end - start;
    if ((flags & 3) != 0) {
        index = saved_file >> 5;
        bit_offset = (saved_file & 0x1f) * 8;
        table = CM_GLOBAL(volatile u32, 0x00de42e0 + index * 4);
        byte = *(volatile u8 *)(table + bit_offset + 4);
        if ((byte & 0x80) != 0) {
            position = start;
            while (position < end) {
                if (*(volatile u8 *)position == 0x0a)
                    ++length;
                ++position;
            }
        }
    } else if ((flags & 0x80) == 0) {
        CM_GLOBAL(u32, 0x00de2f8c) = 0x16;
        return -1;
    }

    if (result == 0)
        return (s32)length;
    if ((*(volatile u8 *)(file + 0x0c) & 1) == 0)
        return (s32)(length + (u32)result);

    requested = *(volatile u32 *)(file + 4);
    if (requested == 0) {
        return result;
    }
    requested = end - start + requested;
    index = saved_file >> 5;
    bit_offset = (saved_file & 0x1f) * 8;
    table = CM_GLOBAL(volatile u32, 0x00de42e0 + index * 4);
    byte = *(volatile u8 *)(bit_offset + table + 4);
    if ((byte & 0x80) != 0) {
        count = (u32)cm_fn_0094c552(saved_file, 0, 2);
        if ((s32)count == result) {
            position = start;
            end = start + requested;
            requested = 0;
            while (position < end) {
                if (*(volatile u8 *)position == 0x0a)
                    ++requested;
                ++position;
            }
            byte = *(volatile u8 *)(file + 0x0d) & 0x20;
        } else {
            (void)cm_fn_0094c552(saved_file, (u32)result, 0);
            if (requested > 0x200 || (*(volatile u8 *)(file + 0x0c) & 8) == 0 ||
                (*(volatile u32 *)(file + 0x0c) & 0x400) != 0)
                requested = *(volatile u32 *)(file + 0x18);
            byte = *(volatile u8 *)(bit_offset + table + 4) & 4;
        }
        if (byte != 0)
            ++requested;
        result -= (s32)requested;
    }
    return (s32)(length + (u32)result);
}
