#include "cmre.h"

/* MOV EAX,[ECX+0x40] returns a dword read from the receiver at offset 0x40. */
u32 CM_THISCALL cm_fn_008676f0(volatile u8 *self)
{
    return *(volatile u32 *)(self + 0x40);
}
