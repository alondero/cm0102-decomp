#include "cmre.h"

/* Object field read through ECX: byte at offset 0x8ea9 from the object pointer.
   The routine takes no stack arguments and returns the byte in AL (u8). */
u8 CM_THISCALL cm_fn_00700340(u32 self)
{
    return *(volatile u8 *)(self + 0x8ea9u);
}
