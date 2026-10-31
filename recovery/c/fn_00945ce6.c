/* Preferred-image VA 0x00945CE6; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00947b52(u32 byte_word, u32 stream);
extern unsigned long long CM_CDECL cm_fn_0094ab50(u32 destination,
                                                  u32 source, u32 count);
extern u32 CM_CDECL cm_fn_0094af9e(u32 stream);
extern unsigned long long CM_CDECL cm_fn_0094b5d6(u32 stream_index,
                                                  u32 destination, u32 count);

u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream)
{
    u32 total = size * count;
    u32 remaining = total;
    u32 edi = total;
    u32 chunk;

    if (total == 0)
        return 0;

    if ((*(volatile u16 *)(stream + 0x0c) & 0x10c) != 0)
        chunk = *(volatile u32 *)(stream + 0x18);
    else
        chunk = 0x1000;

    for (;;) {
        u32 flags = *(volatile u32 *)(stream + 0x0c) & 0x108;

        if (flags != 0) {
            u32 stream_remaining = *(volatile u32 *)(stream + 4);
            if (stream_remaining != 0) {
                u32 amount = remaining;
                if (stream_remaining < remaining)
                    amount = stream_remaining;
                (void)cm_fn_0094ab50(*(volatile u32 *)stream, destination, amount);
                *(volatile u32 *)(stream + 4) -= amount;
                *(volatile u32 *)stream += amount;
                remaining -= amount;
                destination += amount;
                edi = total;
                if (remaining == 0)
                    return count;
                continue;
            }
        }

        if (remaining < chunk) {
            s32 byte_value = (s8)*(volatile u8 *)destination;
            u32 result = cm_fn_00947b52((u32)byte_value, stream);
            if (result == 0xffffffffu)
                return (edi - remaining) / size;

            ++destination;
            chunk = *(volatile u32 *)(stream + 0x18);
            --remaining;
            if ((s32)chunk <= 0)
                chunk = 1;
        } else {
            if (flags != 0 && cm_fn_0094af9e(stream) != 0)
                return (edi - remaining) / size;

            {
                u32 amount = remaining;
                if (chunk != 0)
                    amount = remaining - remaining % chunk;
                edi = amount;
                {
                    u32 bytes_written = (u32)cm_fn_0094b5d6(
                        *(volatile u32 *)(stream + 0x10), destination, amount);
                    if (bytes_written == 0xffffffffu) {
                        *(volatile u32 *)(stream + 0x0c) |= 0x20;
                        return (total - remaining) / size;
                    }
                    destination += bytes_written;
                    remaining -= bytes_written;
                    if (bytes_written < amount) {
                        *(volatile u32 *)(stream + 0x0c) |= 0x20;
                        return (total - remaining) / size;
                    }
                }
            }
            edi = total;
        }

        if (remaining == 0)
            return count;
    }
}
