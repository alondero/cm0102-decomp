#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

void CM_THISCALL cm_fn_008e8540(u32 self)
{
    u32 pointer;
    *(volatile u32 *)self = 0x00970458u;
    pointer = *(volatile u32 *)(self + 4u);
    if (pointer != 0) cm_fn_009452ca(pointer);
}
