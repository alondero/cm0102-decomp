#include "cmre.h"

extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

u32 CM_STDCALL cm_fn_008ae140(u32 source, volatile u32 destination)
{
    u32 fourth = *(volatile u8 *)(source + 7u);
    u32 third = *(volatile u8 *)(source + 6u);
    u32 second = *(volatile u8 *)(source + 5u);
    u32 first = *(volatile u8 *)(source + 4u);
    u32 dest = destination;
    return cm_fn_00944c9f(dest, 0x00a695e4u, first, second, third, fourth);
}
