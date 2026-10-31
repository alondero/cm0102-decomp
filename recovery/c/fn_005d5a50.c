#include "cmre.h"

/* MOV EAX,[ECX+0x45] returns a dword read from the receiver at offset 0x45. */
u32 CM_THISCALL cm_fn_005d5a50(volatile u8 *self)
{
    return *(volatile u32 *)(self + 0x45);
}
