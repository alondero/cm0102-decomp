#include "cmre.h"

/* Record stride 0x44 from the array base in arg3. Field offsets inside one
   record (relative to the record base): 0x00 u32, 0x04 u16, 0x06 u16,
   0x08 u16 (stored from a byte), 0x0a u16 (stored from a byte),
   0x0c u16 (stored from a byte), 0x0e..0x22 one byte each, 0x23 u32,
   0x27 u32, 0x2b u32, 0x2f u32, 0x33 u32, 0x37 u32, 0x3b u32, 0x3f u32,
   0x43 one byte.
   arg2 (stack slot at entry+8) is the mode flag and is also the one-byte
   scratch the reads at record offsets 0x08..0x0c go through. */

extern u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

/* Nonzero flag calls 0x00932c30 (size, count 1); zero flag calls 0x00945bfe
   (count = size, element size 1). Both read `size` bytes into `dest`. */
static u32 read_part(u32 flag, u32 dest, u32 size, u32 stream)
{
    if (flag != 0) {
        return cm_fn_00932c30(dest, size, 1u, stream);
    }
    return cm_fn_00945bfe(dest, size, 1u, stream);
}

u32 CM_CDECL cm_fn_0054e9a0(u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u32 stream;
    u32 flag;
    u32 index;

    stream = arg1;
    flag = arg2;
    index = 0;
    if (arg4 == 0) {
        return 1;
    }
    do {
        u32 elem = arg3 + index * 0x44u;

        if (read_part(flag, elem + 0x00, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x04, 2u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x06, 2u, stream) < 1u) return 0;

        /* Byte reads go into the arg2 stack slot, then are stored as words. */
        if (read_part(flag, (u32)&arg2, 1u, stream) < 1u) return 0;
        *(volatile u16 *)(elem + 0x08) = (u16)*(volatile u8 *)&arg2;
        if (read_part(flag, (u32)&arg2, 1u, stream) < 1u) return 0;
        *(volatile u16 *)(elem + 0x0a) = (u16)*(volatile u8 *)&arg2;
        if (read_part(flag, (u32)&arg2, 1u, stream) < 1u) return 0;
        *(volatile u16 *)(elem + 0x0c) = (u16)*(volatile u8 *)&arg2;

        if (read_part(flag, elem + 0x0e, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x0f, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x10, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x11, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x12, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x13, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x14, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x15, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x16, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x17, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x18, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x19, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1a, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1b, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1c, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1d, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1e, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x1f, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x20, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x21, 1u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x22, 1u, stream) < 1u) return 0;

        if (read_part(flag, elem + 0x23, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x27, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x2b, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x2f, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x33, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x37, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x3b, 4u, stream) < 1u) return 0;
        if (read_part(flag, elem + 0x3f, 4u, stream) < 1u) return 0;

        if (read_part(flag, elem + 0x43, 1u, stream) < 1u) return 0;

        index = index + 1;
    } while (index < arg4);
    return 1;
}
