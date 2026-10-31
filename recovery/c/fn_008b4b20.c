#include "cmre.h"
/* Preferred-image VA 0x008b4b20, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00946464(u32, u32, u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_THISCALL cm_fn_008b4b20(u32 self, u32 player, u32 output)
{
    volatile u8 scratch[0x208];
    u32 first, second, line, text, profile, id, pointer;
    u32 count, base, key, limit, result;
    if (player == 0) {
        first = (u32)(scratch + 0x108);
        second = (u32)(scratch + 8);
        line = 0x995;
        goto failure;
    }
    if (output == 0) {
        first = (u32)(scratch + 8);
        second = (u32)(scratch + 0x108);
        line = 0x99c;
        goto failure;
    }
    profile = D(player + 0x61u);
    if (profile == 0) {
        first = (u32)(scratch + 8);
        second = (u32)(scratch + 0x108);
        line = 0x9a3;
        goto failure;
    }
    id = D(profile);
    pointer = D(self + 0x651du);
    if ((B(pointer + id * 6u + 5u) & 0x10u) == 0) return 0;
    key = D(player);
    count = D(self + 0x6525u);
    base = D(self + 0x6521u);
    D((u32)scratch) = key;
    pointer = cm_fn_00946464((u32)scratch, base, count, 8, 0x008b4e00);
    if (pointer == 0) return 0;
    base = D(self + 0x6521u);
    key = D((u32)scratch);
    if (pointer > base) {
        do {
            pointer -= 8u;
            if (key - D(pointer) != 0) {
                pointer += 8u;
                break;
            }
        } while (pointer > base);
    }
    D(output) = pointer;
    count = D(self + 0x6525u);
    base = D(self + 0x6521u);
    limit = base + count * 8u - 8u;
    result = 0;
    while (pointer <= limit) {
        if (key - D(pointer) != 0) break;
        pointer += 8u;
        ++result;
    }
    return result;

failure:
    cm_fn_00944cff(0x00a8ccd0, 0, 0, first, second);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, first, second, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0;
}
