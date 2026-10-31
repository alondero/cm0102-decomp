/* Preferred-image VA 0x007feeb0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
u32 CM_THISCALL cm_fn_007feeb0(u32 self, u32 source)
{
    u32 offset = (u32)(s32)(s16)CM_GLOBAL(volatile u16, self + 0x382au) * 768u;
    u32 object = CM_GLOBAL(volatile u32, self + offset + 0x7e2u);
    u32 length = 0, cursor, dest, i;
    u8 value;
    if (!object)
        return offset;
    dest = object + 0x1fcu;
    if (!source) {
        CM_GLOBAL(volatile u8, dest) = 0;
        return offset;
    }
    do {
        value = CM_GLOBAL(volatile u8, source + length);
        ++length;
    } while (value && length != 0xffffffffu);
    cursor = 0;
    for (i = 0; i < (length >> 2); ++i) {
        u32 word = CM_GLOBAL(volatile u32, source + cursor);
        CM_GLOBAL(volatile u32, dest + cursor) = word;
        cursor += 4;
    }
    for (i = 0; i < (length & 3u); ++i) {
        u8 byte = CM_GLOBAL(volatile u8, source + cursor);
        CM_GLOBAL(volatile u8, dest + cursor) = byte;
        ++cursor;
    }
    return length;
}
