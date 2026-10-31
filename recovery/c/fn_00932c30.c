#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 count, u32 element_size,
                                  u32 state_address);

static u32 refill_00932c30(u32 stream)
{
    return cm_fn_00945bfe(stream + 0x41705u, 1u, 0x30d40u,
                          *(volatile u32 *)stream);
}

u32 CM_CDECL cm_fn_00932c30(u32 destination, u32 size, u32 count, u32 stream)
{
    u32 total = size * count;
    s32 signed_total = (s32)total;
    u32 selected;
    u32 chunk;
    u32 offset;
    u32 end;
    s32 amount;

    if (stream == 0u || *(volatile u32 *)(stream + 4u) != 0u)
        return 0u;

    selected = *(volatile u32 *)(stream + 0x0cu);
    offset = *(volatile u32 *)(stream + 8u);
    chunk = *(volatile u32 *)(stream + 0x29u + selected * 0x10cu);
    end = chunk - total;

    if ((s32)offset <= (s32)end) {
        amount = signed_total;
    } else {
        amount = (s32)(chunk - offset);
        if (amount <= 0)
            return 0u;
    }
    {
        u32 new_offset = *(volatile u32 *)(stream + 8u) + (u32)amount;
        u32 mode = *(volatile u32 *)(stream + 0x19u);
        *(volatile u32 *)(stream + 8u) = new_offset;

    if (mode != 4u) {
        s32 quotient = amount / (s32)size;
        u32 state_address = *(volatile u32 *)stream;
        *(volatile u32 *)(stream + 8u) = new_offset +
            (u32)quotient * size - (u32)amount;
        return cm_fn_00945bfe(destination, size, (u32)quotient, state_address);
    }

    if (amount > 0) {
        u32 emitted = 0u;
        volatile u8 token_spill;
        u32 run_total = *(volatile u32 *)(stream + 0x14u);
        if (run_total != 0u) {
            u32 run_index = *(volatile u32 *)(stream + 0x10u);
            if ((s32)run_index < (s32)run_total) {
                if ((s32)emitted >= amount)
                    goto mode4_done;
                for (;;) {
                    u32 destination_now = destination;
                    u8 value = *(volatile u8 *)(stream + 0x18u);
                    *(volatile u8 *)(destination_now + emitted) = value;
                    run_index = *(volatile u32 *)(stream + 0x10u);
                    run_total = *(volatile u32 *)(stream + 0x14u);
                    emitted++;
                    run_index++;
                    *(volatile u32 *)(stream + 0x10u) = run_index;
                    if ((s32)run_index >= (s32)run_total ||
                        (s32)emitted >= amount)
                        break;
                }
                if ((s32)emitted >= amount)
                    goto mode4_done;
            }
        }

    scan_token:
        if ((s32)emitted >= amount)
            goto mode4_done;
        *(volatile u32 *)(stream + 0x14u) = 0u;
        *(volatile u32 *)(stream + 0x10u) = 0u;
    read_token:
        {
            u32 cursor = *(volatile u32 *)(stream + 0xa3185u);
            u32 limit = *(volatile u32 *)(stream + 0xa318du);
            u8 token;
            if ((s32)cursor < (s32)limit) {
                token = *(volatile u8 *)(stream + 0x41705u + cursor);
                token_spill = token;
                cursor++;
                *(volatile u32 *)(stream + 0xa3185u) = cursor;
            } else {
                *(volatile u32 *)(stream + 0xa318du) =
                    refill_00932c30(stream);
                token = *(volatile u8 *)(stream + 0x41705u);
                *(volatile u32 *)(stream + 0xa3185u) = 1u;
                token_spill = token;
            }

            if (token <= 0x80u) {
                u32 destination_now = destination;
                emitted++;
                *(volatile u8 *)(destination_now + emitted - 1u) = token;
                run_total = *(volatile u32 *)(stream + 0x14u);
                if (run_total != 0u) {
                    if ((s32)emitted < amount)
                        goto emit_run;
                    goto mode4_done;
                }
                if ((s32)emitted >= amount)
                    goto mode4_done;
                goto read_token;
            }

            u32 token_value = token_spill;
            cursor = *(volatile u32 *)(stream + 0xa3185u);
            limit = *(volatile u32 *)(stream + 0xa318du);
            run_total = token_value - 0x80u;
            *(volatile u32 *)(stream + 0x14u) = run_total;
            if ((s32)cursor < (s32)limit) {
                *(volatile u8 *)(stream + 0x18u) =
                    *(volatile u8 *)(stream + 0x41705u + cursor);
                *(volatile u32 *)(stream + 0xa3185u) = cursor + 1u;
            } else {
                *(volatile u32 *)(stream + 0xa318du) =
                    refill_00932c30(stream);
                {
                    u8 value = *(volatile u8 *)(stream + 0x41705u);
                    *(volatile u32 *)(stream + 0xa3185u) = 1u;
                    *(volatile u8 *)(stream + 0x18u) = value;
                }
            }
            run_total = *(volatile u32 *)(stream + 0x14u);
            if (run_total != 0u) {
                if ((s32)emitted < amount)
                    goto emit_run;
                goto mode4_done;
            }
            if ((s32)emitted >= amount)
                goto mode4_done;
            goto read_token;
        }

    emit_run:
        {
            u32 run_limit = *(volatile u32 *)(stream + 0x14u);
            if (run_limit == 0u)
                goto scan_token;
            u32 run_index = *(volatile u32 *)(stream + 0x10u);
            if ((s32)run_index >= (s32)run_limit)
                goto scan_token;
            if ((s32)emitted >= amount)
                goto mode4_done;
            do {
                u32 destination_now = destination;
                u8 value = *(volatile u8 *)(stream + 0x18u);
                *(volatile u8 *)(destination_now + emitted) = value;
                run_index = *(volatile u32 *)(stream + 0x10u);
                run_limit = *(volatile u32 *)(stream + 0x14u);
                emitted++;
                run_index++;
                *(volatile u32 *)(stream + 0x10u) = run_index;
                if ((s32)run_index >= (s32)run_limit)
                    break;
            } while ((s32)emitted < amount);
            if ((s32)emitted >= amount)
                goto mode4_done;
            goto scan_token;
        }

    mode4_done:
        ;
    }
    return (u32)(amount / (s32)size);
    }
}
