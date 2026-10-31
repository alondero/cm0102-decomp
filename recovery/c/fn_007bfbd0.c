#include "cmre.h"

/* cdecl record filter. Stack arguments on entry: count (signed byte, low byte of
   the first argument), src (first source record), gate (must be non-null; it is
   only tested, never read again), dst (output buffer). The result is the number
   of copied records, returned in AL (u8).
   Record fields by byte offset from the record base (26 bytes = 0x1a per record):
   0x04 signed 32-bit index, compared with the global at 0x00ae2364 (must be
   0..global), 0x14 32-bit flags (bit 0x400 required, bit 0x40000 clear).
   A matching record is copied whole (24 bytes as dwords, then 2 bytes) to
   dst + 26 * copied, where `copied` is the count before this copy. */

#define RECORD_SIZE 26u
#define RECORD_FLAGS_OFF 0x14u
#define RECORD_INDEX_OFF 0x04u

u8 CM_CDECL cm_fn_007bfbd0(s8 count, u32 src, u32 gate, u32 dst)
{
    u8 copied = 0;
    s32 remaining;
    u32 elem;
    s32 index;
    u32 flags;
    u32 slot;
    u32 dstp;
    u32 i;

    if (count == 0) {
        return 0;
    }
    if (src == 0) {
        return 0;
    }
    if (gate == 0) {
        return 0;
    }
    if ((s32)count <= 0) {
        return 0;
    }

    remaining = (s32)count;
    elem = src;
    do {
        index = (s32)*(volatile u32 *)(elem + RECORD_INDEX_OFF);
        if (index >= 0 && index <= (s32)CM_GLOBAL(volatile s32, 0x00ae2364)) {
            flags = *(volatile u32 *)(elem + RECORD_FLAGS_OFF);
            if ((flags & 0x40000u) == 0 && (flags & 0x400u) != 0) {
                slot = (u32)(s32)(s8)copied;
                copied = (u8)(copied + 1u);
                dstp = dst + slot * RECORD_SIZE;
                for (i = 0; i < 24u; i = i + 4u) {
                    *(volatile u32 *)(dstp + i) = *(volatile u32 *)(elem + i);
                }
                *(volatile u16 *)(dstp + 24u) = *(volatile u16 *)(elem + 24u);
            }
        }
        elem = elem + RECORD_SIZE;
        remaining = remaining - 1;
    } while (remaining != 0);

    return copied;
}
