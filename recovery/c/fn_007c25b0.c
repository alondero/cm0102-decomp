#include "cmre.h"
/* Preferred-image VA 0x007c25b0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
/* Seven stack arguments and the ECX explicitly loaded by the original caller. */
extern u32 CM_THISCALL cm_fn_007c2750(u32, u32, u32, u32, s32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_007c3920(u32, u32, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_007c25b0(u32 self, u32 player, u32 category, u32 field,
                              u32 destination, s32 capacity, u32 split,
                              u32 denominator, u32 zero)
{
    u8 scratch[0x200];
    u8 *first, *second;
    u32 source, cursor, remaining, count, n, pointer, id, total, value, line;
    u8 byte;
    char *version;
    if (destination == 0 || capacity < 51) {
        first = scratch; second = scratch + 0x100; line = 0x7c5;
        goto error;
    }
    source = (u8)field == 17 ? 0x0099cc4c : 0x0098ba0c;
    cursor = source;
    remaining = 0xffffffffu;
    do {
        if (remaining == 0) break;
        --remaining;
        byte = B(cursor);
        ++cursor;
    } while (byte != 0);
    count = ~remaining;
    source = cursor - count;
    pointer = destination;
    for (n = count >> 2; n != 0; --n) {
        value = D(source);
        D(pointer) = value;
        source += 4u; pointer += 4u;
    }
    for (n = count & 3u; n != 0; --n) {
        byte = B(source);
        B(pointer) = byte;
        ++source; ++pointer;
    }
    if (player == 0) {
        first = scratch + 0x100; second = scratch; line = 0x7d7;
        goto error;
    }
    id = D(player);
    total = D(0x00ae236c);
    if ((s32)id >= (s32)total) return 1;
    pointer = cm_fn_007c3920(self, player, category);
    if (pointer == 0) return 0;
    return cm_fn_007c2750(self, pointer, field, destination, capacity,
                         split, denominator, zero);
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
