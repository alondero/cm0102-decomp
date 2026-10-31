#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u32 CM_THISCALL cm_fn_00690b70(u32 self, u32 argument)
{
    u32 base = D(self);
    u32 index = D(argument);
    u32 offset = index * 73u;
    u16 word = W(base + offset + 6);
    s16 signed_word = (s16)word;
    u32 eax = (offset & 0xffff0000u) | word;
    u32 result;
    if (signed_word < 0x0bb8) result = 0;
    else if (signed_word < 0x0dac) result = 1;
    else if (signed_word < 0x10b3) result = 2;
    else if (signed_word < 0x1964) result = 3;
    else if (signed_word < 0x2134) result = 4;
    else return ((eax & 0xffffff00u) | (signed_word >= 0x251c)) + 5u;
    return (eax & 0xffffff00u) | result;
}
