/* WIP: regs mismatch (eax), case 8 */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address);

/* One transfer of `size` bytes, count 1, on `context`. The caller's second
   argument selects the callee: zero takes cm_fn_00945bfe, otherwise
   cm_fn_00932c30. The result is compared with zero by the caller. */
static u32 cm_io_step(u32 selector, u32 destination, u32 size, u32 context)
{
    if (selector == 0u) {
        return cm_fn_00945bfe(destination, size, 1u, context);
    }
    return cm_fn_00932c30(destination, size, 1u, context);
}

/* Per record (stride 0x245 bytes, indexed by the third argument), a fixed
   sequence of transfers at the offsets below, then a 16-bit field at +0x80
   that is loaded from the first argument's low byte, scaled by 500, and
   stored back. The first argument is re-read at the top of each iteration:
   the transfer to &param_1 overwrites its low byte. */
u32 CM_CDECL cm_fn_0054dc90(u32 param_1, u32 param_2, u32 param_3, u32 param_4)
{
    u32 index = 0u;
    u32 record;
    u32 first_arg;
    u32 cursor;
    u32 k;

    if (param_4 != 0u) {
        do {
            first_arg = param_1;
            record = param_3 + index * 0x245u;

            if (cm_io_step(param_2, record, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x04u, 0x33u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x38u, 0x1au, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x53u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x57u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x5bu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x5fu, 1u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x60u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x64u, 1u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x65u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x69u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x6du, 1u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x6eu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x72u, 1u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x73u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x77u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x7bu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x7fu, 1u, first_arg) == 0u) return 0u;

            /* The destination is the first argument's own stack slot. */
            if (cm_io_step(param_2, (u32)&param_1, 1u, first_arg) == 0u) return 0u;

            {
                u16 value = (u16)(u8)param_1;
                *(volatile u16 *)(record + 0x80u) = value;
                if (value == 0u) {
                    *(volatile u16 *)(record + 0x80u) = 1u;
                }
                value = *(volatile u16 *)(record + 0x80u);
                value = (u16)(value * 500u);
                *(volatile u16 *)(record + 0x80u) = value;
            }

            if (cm_io_step(param_2, record + 0x82u, 1u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x83u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x87u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x8bu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x8fu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x93u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x97u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x9bu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x9fu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xa3u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xa7u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xabu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xafu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xb3u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xb7u, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xbbu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xbfu, 4u, first_arg) == 0u) return 0u;

            cursor = record + 0xc3u;
            for (k = 0u; k < 3u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            if (cm_io_step(param_2, record + 0xcfu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0xd3u, 4u, first_arg) == 0u) return 0u;

            cursor = record + 0xd7u;
            for (k = 0u; k < 0x32u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            cursor = record + 0x19fu;
            for (k = 0u; k < 5u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            cursor = record + 0x1b3u;
            for (k = 0u; k < 7u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            cursor = record + 0x1cfu;
            for (k = 0u; k < 3u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            if (cm_io_step(param_2, record + 0x1dbu, 4u, first_arg) == 0u) return 0u;
            if (cm_io_step(param_2, record + 0x1dfu, 1u, first_arg) == 0u) return 0u;

            cursor = record + 0x1e0u;
            for (k = 0u; k < 0x14u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            cursor = record + 0x230u;
            for (k = 0u; k < 4u; k++) {
                if (cm_io_step(param_2, cursor, 4u, first_arg) == 0u) return 0u;
                cursor += 4u;
            }

            if (cm_io_step(param_2, record + 0x240u, 4u, first_arg) == 0u) return 0u;

            index = index + 1u;
        } while (index < param_4);
    }
    return 1u;
}
