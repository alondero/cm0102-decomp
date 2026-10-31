#include "cmre.h"

/* Object field read through ECX: word at offset 0x19a from the object pointer.
   The routine takes no stack arguments and returns only AX (u16). */
u16 CM_THISCALL cm_fn_006f4a30(u32 self)
{
    return *(volatile u16 *)(self + 0x19au);
}
