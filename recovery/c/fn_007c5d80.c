#include "cmre.h"
/* Preferred-image VA 0x007c5d80, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern void CM_CDECL cm_fn_005e8290(const char *, const char *, u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, void *, void *);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
#define W(a) (*(volatile u16 *)(u32)(a))
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_007c5d80(u32 self, u32 player, s8 kind)
{
    u8 scratch[0x200];
    u8 *first, *second;
    u32 index, base, slots, target, offset, slot, pointer, count, n, line;
    char *version;
    if (player == 0) {
        first = scratch + 0x100; second = scratch; line = 0xe92;
        goto error;
    }
    index = D(player);
    base = D(self + 0x30u);
    slots = base + index * 39u;
    if ((u32)((s32)kind - 1) > 5u) goto invalid;
    target = D(0x007c5fc8u + ((u32)(s32)kind - 1u) * 4u);
    switch (target) {
    case 0x007c5df1: offset = 0; break;
    case 0x007c5df5: offset = 4; break;
    case 0x007c5dfa: offset = 8; break;
    case 0x007c5dff: offset = 12; break;
    case 0x007c5e04: offset = 16; break;
    case 0x007c5e09: offset = 20; break;
    default: goto invalid;
    }
    slot = slots + offset;
    index = D(slot);
    /* Only -1 requests allocation; other negative indices still address data. */
    if (index != 0xffffffffu) goto existing;
    if (D(self + 0x38u) != 0) {
        count = D(self + 0xcu);
        pointer = D(self + 4u);
        for (index = 0; (s32)index < (s32)count; ++index, pointer += 32u) {
            if (W(pointer + 0xeu) == 0xffffu) {
                for (n = 0; n < 8; ++n) D(pointer + n * 4u) = 0;
                D(slot) = index;
                count = D(self + 0x38u);
                D(self + 0x38u) = count - 1u;
                return pointer;
            }
        }
    }
    count = D(self + 8u);
    index = D(self + 0xcu);
    if ((s32)index >= (s32)count) {
        pointer = D(self + 4u);
        count += 100u;
        D(self + 8u) = count;
        pointer = (u32)cm_fn_00945667(pointer, count << 5);
        D(self + 4u) = pointer;
        if (pointer == 0) {
            cm_fn_00944cff(0x00a78310, 0, 0, scratch, scratch + 0x100);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0, 0x009a8120, version,
                          scratch, scratch + 0x100, 0xef4, 0x00a784fc);
            cm_fn_0066f4e0(0x00ae28bc, 0x009a8108);
            cm_fn_0066f4e0(0x00ae26c4, 0x009a7ff8, 0x00ae24d0);
            cm_fn_005e8290((const char *)0x00ae28bc, (const char *)0x00ae26c4, 0);
            return 0;
        }
    }
    index = D(self + 0xcu);
    D(slot) = index;
    count = D(self + 0xcu);
    base = D(self + 4u);
    D(self + 0xcu) = count + 1u;
    index = D(slot);
    pointer = base + (index << 5);
    for (n = 0; n < 8; ++n) D(pointer + n * 4u) = 0;
existing:
    index = D(slot);
    base = D(self + 4u);
    return (index << 5) + base;
invalid:
    first = scratch; second = scratch + 0x100; line = 0xeca;
error:
    cm_fn_00944cff(0x00a78310, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
