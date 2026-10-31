#include "cmre.h"

/* Stream transfer helper. The listing pushes the stream argument first, then the
   count, the size and the destination, and chooses the callee on the mode argument
   (non-zero calls 0x00932c30, zero calls 0x00945bfe). Argument order is preserved. */
extern u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

static u32 cm_xfer(u32 mode, u32 destination, u32 size, u32 count, u32 stream)
{
    if (mode != 0) {
        return cm_fn_00932c30(destination, size, count, stream);
    }
    return cm_fn_00945bfe(destination, size, count, stream);
}

/* Records are 0x6b bytes apart. The cursor is the record base plus 0x38 (the listing
   keeps ESI at that point and advances it by 0x6b after each record). Offsets below
   are relative to the record base. The one-byte transfer target is the third argument
   slot itself (lea [esp+0x28] three pushes deep); its low byte is then stored. */
u32 CM_CDECL cm_fn_0054ffd0(u32 arg_stream, u32 arg_mode, u32 arg_base, u32 arg_count)
{
    u32 cursor;
    u32 index;

    if (arg_count == 0) {
        return 1;
    }
    cursor = arg_base + 0x38u;
    index = 0;
    do {
        if (cm_xfer(arg_mode, cursor - 0x38u, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor - 0x34u, 0x33u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor, 0x1au, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x1bu, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x1fu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x20u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x21u, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x25u, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x29u, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, cursor + 0x2du, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(arg_mode, (u32)&arg_base, 1u, 1u, arg_stream) == 0) return 0;
        *(volatile u16 *)(cursor + 0x31u) = (u16)*(volatile u8 *)&arg_base;

        cursor = cursor + 0x6bu;
        index = index + 1u;
    } while (index < arg_count);

    return 1;
}
