#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u32 CM_THISCALL cm_fn_00690bd0(u32 self, u32 argument)
{
    u32 base = D(self);
    u32 index = D(argument);
    u32 offset = index * 73u;
    u16 word = W(base + offset + 0x12);
    s16 signed_word = (s16)word;
    u32 eax = (offset & 0xffff0000u) | word;
    u32 result;
    if (signed_word < -2000) result = 0;
    else if (signed_word < -1000) result = 1;
    else if (signed_word < -500) result = 2;
    else if (signed_word < 500) result = 3;
    else if (signed_word < 1000) result = 4;
    else return ((eax & 0xffffff00u) | (signed_word >= 2000)) + 5u;
    return (eax & 0xffffff00u) | result;
}

