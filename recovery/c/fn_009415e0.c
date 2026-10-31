#include "cmre.h"

void CM_CDECL cm_fn_009415e0(u32 input, u32 destination, s32 length)
{
    u32 position, source, remaining, words;
    if (length <= 0) return;
    position = *(volatile u32 *)(input + 4u);
    source = *(volatile u32 *)input + position;
    remaining = (u32)length;
    words = remaining >> 2;
    while (words-- != 0) {
        *(volatile u32 *)destination = *(volatile u32 *)source;
        source += 4u;
        destination += 4u;
    }
    remaining &= 3u;
    while (remaining-- != 0) {
        *(volatile u8 *)destination = *(volatile u8 *)source;
        ++source;
        ++destination;
    }
    *(volatile u32 *)(input + 4u) = position + (u32)length;
}
