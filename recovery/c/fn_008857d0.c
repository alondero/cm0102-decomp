#include "cmre.h"

/* Object field read through ECX: byte at offset 0x4f from the object pointer,
   shifted right by 4 within AL. The routine takes no stack arguments and
   returns the byte in AL (u8). */
u8 CM_THISCALL cm_fn_008857d0(u32 self)
{
    u8 value = *(volatile u8 *)(self + 0x4fu);
    return (u8)(value >> 4);
}
