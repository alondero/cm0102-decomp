/* WIP: original sets only AL; EAX high bits derive from the second stack argument, but caller-read EDX is inherited and plain C cannot preserve it through the required 64-bit return. */
#include "cmre.h"

u8 CM_CDECL cm_fn_008adbb0(u32 selector, u32 value)
{
    static const u32 first[] = {
        1u,2u,4u,8u,0x10u,0x20u,0x40u,0x80u,0x100u,0x200u,0x400u,0x800u,
        0x1000u,0x2000u,0x4000u,0x8000u,0x10000u,0x20000u,0x40000u,0x80000u,
        0x100000u,0x200000u,0x400000u,0x800000u,0x1000000u,0x2000000u,
        0x4000000u,0x8000000u,0x10000000u,0x20000000u,0x40000000u,0x80000000u
    };
    for (u32 i = 0; i < 32; ++i)
        if (selector == first[i] && value == 0) return (u8)i;
    static const u32 second[] = {
        1u,2u,4u,8u,0x10u,0x20u,0x40u,0x80u,0x100u,0x200u,0x400u,0x800u,
        0x1000u,0x2000u,0x4000u,0x8000u
    };
    if (selector == 0)
        for (u32 i = 0; i < 16; ++i)
            if (value == second[i]) return (u8)(0x20u + i);
    return 0xff;
}
