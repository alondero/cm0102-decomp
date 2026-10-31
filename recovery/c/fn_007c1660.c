#include "cmre.h"
/* Preferred-image VA 0x007c1660, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
#define B(a) (*(volatile u8 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

static u32 scan_count(u32 address)
{
    u32 remaining = 0xffffffffu;
    u8 byte;
    do {
        if (remaining == 0) break;
        --remaining;
        byte = B(address);
        ++address;
    } while (byte != 0);
    return ~remaining;
}

static void copy_count(u32 destination, u32 source, u32 count)
{
    u32 n, value;
    u8 byte;
    for (n = count >> 2; n != 0; --n) {
        value = D(source);
        D(destination) = value;
        source += 4u; destination += 4u;
    }
    for (n = count & 3u; n != 0; --n) {
        byte = B(source);
        B(destination) = byte;
        ++source; ++destination;
    }
}

u32 CM_THISCALL cm_fn_007c1660(u32 self, u32 player, u32 destination, s32 capacity)
{
    u8 scratch[0x234];
    u32 index, total, base, slots, row, nibble, count, end;
    u8 byte;
    char *version;
    B(destination) = 0;
    if (player == 0) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x134, scratch + 0x34);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x134, scratch + 0x34, 0x41d);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    if (destination == 0 || capacity < 51) {
        cm_fn_00944cff(0x00a78310, 0, 0, scratch + 0x34, scratch + 0x134);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version,
                      scratch + 0x34, scratch + 0x134, 0x424);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
        D(0x00b67a34) = 0;
    }
    index = D(player);
    total = D(0x00ae236c);
    if ((s32)index >= (s32)total)
        return cm_fn_00944c9f(destination, 0x0098ba0c);
    base = D(self + 0x30u);
    slots = base + index * 39u;
    for (row = 0; row < 5; ++row) {
        /* Reload +24 twice and +25 twice, including after formatter writes. */
        byte = B(slots + 0x24u + (row >> 1));
        nibble = (row & 1u) != 0 ? byte >> 4 : byte & 15u;
        if (nibble == 0) continue;
        cm_fn_00944c9f((u32)scratch, 0x00a7841c, nibble);
        count = scan_count((u32)scratch);
        end = destination + scan_count(destination) - 1u;
        copy_count(end, (u32)scratch, count);
    }
    byte = B(destination);
    if (byte != 0) {
        count = scan_count(destination) - 1u;
        if (count >= 2u) {
            count = scan_count(destination) - 1u;
            B(destination + count - 1u) = 0;
            return 0;
        }
    }
    count = scan_count(0x0098ba0c);
    copy_count(destination, 0x0098ba0c, count);
    return count;
}
