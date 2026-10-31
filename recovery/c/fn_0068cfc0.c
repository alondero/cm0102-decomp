/* Preferred-image VA 0x0068cfc0, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
unsigned long long CM_THISCALL cm_fn_0068cfc0(u32 self, volatile u32 source, u32 length_arg)
{
    volatile u16 length_word = (u16)length_arg;
    u32 length = (u32)(s32)(s16)length_word, count, destination, source_value, offset = 0;
    unsigned long long allocation;
    *(volatile u32 *)(self + 0xau) = 0;
    *(volatile u32 *)(self + 6u) = 0;
    allocation = cm_fn_00944cf1(length);
    destination = (u32)allocation;
    *(volatile u16 *)(self + 4u) = length_word;
    source_value = source;
    *(volatile u32 *)self = destination;
    count = length >> 2;
    while (count--) {
        u32 value = *(volatile u32 *)(source_value + offset);
        *(volatile u32 *)(destination + offset) = value;
        offset += 4u;
    }
    count = length & 3u;
    while (count--) {
        u8 value = *(volatile u8 *)(source_value + offset);
        *(volatile u8 *)(destination + offset) = value;
        ++offset;
    }
    return (allocation & 0xffffffff00000000ull) | self;
}
