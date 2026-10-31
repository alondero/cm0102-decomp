#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

void CM_THISCALL cm_fn_00767f60(u32 self, u8 first, u16 second)
{
    u32 pointer = *(volatile u32 *)self;
    if (pointer != 0) {
        cm_fn_009452ca(pointer);
        *(volatile u32 *)self = 0;
    }
    *(volatile u32 *)(self + 8u) = 0;
    *(volatile u32 *)(self + 4u) = 0;
    *(volatile u8 *)(self + 0xcu) = first;
    *(volatile u16 *)(self + 0xdu) = second;
}
