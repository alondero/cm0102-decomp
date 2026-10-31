/* Preferred-image VA 0x0094f72a, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_CDECL cm_fn_00951f87(u32 left, u32 right, u32 output);
unsigned long long CM_CDECL cm_fn_0094f72a(u32 words, s32 bit)
{
    s32 index = bit / 32;
    u32 shift = (31u - (u32)(bit % 32)) & 31u;
    u32 pointer = words + (u32)index * 4u;
    u32 carry = cm_fn_00951f87(CM_GLOBAL(volatile u32, pointer), 1u << shift, pointer);
    --index;
    while (index >= 0 && carry) {
        pointer = words + (u32)index * 4u;
        carry = cm_fn_00951f87(CM_GLOBAL(volatile u32, pointer), 1, pointer);
        --index;
    }
    return ((unsigned long long)pointer << 32) | carry;
}
