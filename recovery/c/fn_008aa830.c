#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_008aa200(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008aa550(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_008aa830(u32 self, u32 index, u32 output, u32 size, u32 fourth, u32 fifth)
{
    u8 scratch[0x390];
    u32 source, cursor, remaining, count, value, destination, i, text;
    if ((s8)index < 0 || (s8)index > 0x13 || output == 0 || (s32)size < 100) {
        cm_fn_00944cff(0x00a8b94c, 0, 0, (u32)scratch + 0x290, (u32)scratch + 0x190);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x290, (u32)scratch + 0x190, 0x1b6c);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    cm_fn_008aa200(self, index, (u32)scratch + 0xc8, 200, 1, fifth);
    cm_fn_008aa550(self, index, (u32)scratch, 200, 0, fourth, fifth);
    if (B((u32)scratch + 0xc8) != 0) {
        if (B((u32)scratch) != 0) {
            cm_fn_00944c9f(output, 0x0098f0b8, (u32)scratch + 0xc8, (u32)scratch);
            return 1;
        }
        source = (u32)scratch + 0xc8;
    } else {
        source = (u32)scratch;
    }
    cursor = source;
    remaining = 0xffffffffu;
    do {
        value = B(cursor);
        ++cursor;
        --remaining;
    } while (remaining != 0 && value != 0);
    count = ~remaining;
    source = cursor - count;
    destination = output;
    for (i = count >> 2; i != 0; --i) {
        value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
    }
    for (i = count & 3u; i != 0; --i) {
        value = B(source);
        B(destination) = (u8)value;
        ++source;
        ++destination;
    }
    return 1;
}
