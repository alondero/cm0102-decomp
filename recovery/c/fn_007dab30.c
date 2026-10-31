#include "cmre.h"

u32 CM_STDCALL cm_fn_007dab30(u32 input, u32 output)
{
    u32 offset = 0, count = 0;
    u8 quoted = 0;
    for (;;) {
        u8 ch = *(volatile u8 *)(input + offset);
        ++offset;
        if (ch == 0x22) {
            if (quoted) break;
            quoted = 1;
            continue;
        }
        if ((ch == 0x20 || ch == 0x0d) && !count)
            continue;
        if (ch == 0x0a) {
            if (!count) continue;
        } else if (!ch) {
            break;
        }
        if ((s32)count > 0x61)
            break;
        if (quoted) {
            *(volatile u8 *)(output + count) = ch;
            ++count;
        }
    }
    *(volatile u8 *)(output + count) = 0;
    return input + offset + 1;
}
