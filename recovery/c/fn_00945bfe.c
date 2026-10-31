#include "cmre.h"

extern u32 CM_CDECL cm_fn_0094ab50(u32 buffer, u32 source, u32 amount);
extern u32 CM_CDECL cm_fn_0094b307(u32 *state);
extern u32 CM_CDECL cm_fn_0094b3e0(u32 state_value, u32 buffer, u32 amount);

u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size, u32 state_address)
{
    u32 total = count * element_size;
    u32 remaining = total;
    u32 original_size = element_size;
    u32 output = buffer;
    u32 block_size;
    u32 state = state_address;

    if (total == 0)
        return 0;

    if ((*(volatile u16 *)(state + 0x0c) & 0x010cu) != 0)
        block_size = *(volatile u32 *)(state + 0x18);
    else
        block_size = 0x1000u;

    while (remaining != 0) {
        u32 flags = *(volatile u16 *)(state + 0x0c);
        u32 available;

        if ((flags & 0x010cu) != 0 &&
            (available = *(volatile u32 *)(state + 4)) != 0) {
            u32 amount = remaining;
            if (available < amount)
                amount = available;
            (void)cm_fn_0094ab50(output, *(volatile u32 *)state, amount);
            remaining -= amount;
            *(volatile u32 *)(state + 4) -= amount;
            *(volatile u32 *)state += amount;
            output += amount;
            continue;
        }

        if (remaining < block_size) {
            u32 byte_value = cm_fn_0094b307((u32 *)state);
            if (byte_value == 0xffffffffu)
                goto error;
            *(volatile u8 *)output = (u8)byte_value;
            block_size = *(volatile u32 *)(state + 0x18);
            output += 1u;
            remaining -= 1u;
            continue;
        }

        u32 amount = remaining;
        if (block_size != 0)
            amount -= remaining % block_size;
        u32 state_value = *(volatile u32 *)(state + 0x10);
        u32 read_result = cm_fn_0094b3e0(state_value, output, amount);
        if (read_result == 0) {
            *(volatile u32 *)(state + 0x0c) |= 0x10u;
            goto error;
        }
        if (read_result == 0xffffffffu) {
            *(volatile u32 *)(state + 0x0c) |= 0x20u;
            goto error;
        }
        remaining -= read_result;
        output += read_result;
    }

    return original_size;

error:
    return (total - remaining) / count;
}
