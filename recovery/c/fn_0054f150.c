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

/* Records are 0x46 bytes apart from the base argument. Offsets below are byte
   offsets within one record. The mode is read once (EBX in the listing); the
   one-byte transfer target is the second argument slot itself (lea [esp+0x24]
   three pushes deep), whose low byte is then stored as a word. */
u32 CM_CDECL cm_fn_0054f150(u32 arg_stream, u32 arg_mode, u32 arg_base, u32 arg_count)
{
    u32 mode;
    u32 index;
    u32 rec;

    if (arg_count == 0) {
        return 1;
    }
    mode = arg_mode;
    index = 0;
    do {
        rec = arg_base + index * 0x46u;

        if (cm_xfer(mode, rec + 0x00u, 4u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x04u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x05u, 2u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x07u, 2u, 1u, arg_stream) == 0) return 0;

        if (cm_xfer(mode, (u32)&arg_mode, 1u, 1u, arg_stream) == 0) return 0;
        *(volatile u16 *)(rec + 0x09u) = (u16)*(volatile u8 *)&arg_mode;
        if (cm_xfer(mode, (u32)&arg_mode, 1u, 1u, arg_stream) == 0) return 0;
        *(volatile u16 *)(rec + 0x0bu) = (u16)*(volatile u8 *)&arg_mode;
        if (cm_xfer(mode, (u32)&arg_mode, 1u, 1u, arg_stream) == 0) return 0;
        *(volatile u16 *)(rec + 0x0du) = (u16)*(volatile u8 *)&arg_mode;
        if (cm_xfer(mode, rec + 0x0fu, 1u, 1u, arg_stream) == 0) return 0;

        if (cm_xfer(mode, rec + 0x10u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x11u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x12u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x13u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x14u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x15u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x16u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x17u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x18u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x19u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1au, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1bu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1cu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1du, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1eu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x1fu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x20u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x21u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x22u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x23u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x24u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x25u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x26u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x27u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x28u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x29u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2au, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2bu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2cu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2du, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2eu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x2fu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x30u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x31u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x32u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x33u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x34u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x35u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x36u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x37u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x38u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x39u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3au, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3bu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3cu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3du, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3eu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x3fu, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x40u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x41u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x42u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x43u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x44u, 1u, 1u, arg_stream) == 0) return 0;
        if (cm_xfer(mode, rec + 0x45u, 1u, 1u, arg_stream) == 0) return 0;

        index = index + 1u;
    } while (index < arg_count);

    return 1;
}
