/* Preferred-image VA 0x00556210, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef unsigned long long (__attribute__((cdecl, regparm(3))) *callback_556210)(u32, u32, u32);
unsigned long long CM_THISCALL cm_fn_00556210(u32 self, u32 argument)
{
    u32 index = *(volatile u32 *)argument;
    u32 base = CM_GLOBAL(volatile u32, 0x00adadfcu);
    u32 entry = *(volatile u32 *)(base + index * 4u);
    u32 value, row, receiver, target;
    if (!entry) return ((unsigned long long)index << 32);
    value = *(volatile u8 *)(entry + 0x50u);
    if (value == 0xffu)
        return ((unsigned long long)index << 32) | (entry & 0xffffff00u) | value;
    value = (u32)(s32)(s8)value;
    row = *(volatile u32 *)self;
    receiver = *(volatile u32 *)(row + value * 4u);
    if (!receiver) return ((unsigned long long)value << 32) | row;
    target = *(volatile u32 *)receiver;
    return ((callback_556210)*(volatile u32 *)target)(row, target, receiver);
}
