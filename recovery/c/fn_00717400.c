/* WIP: return width, caller 0x006b8e5a reads EAX beyond the 16-bit AX result. */
/* Preferred-image VA 0x00717400, image base 0x00400000.
   Listing: mov ax, word ptr [ecx + 0x1bc]; ret. Thiscall with no stack arguments;
   returns the 16-bit value at receiver offset 0x1bc (AX only). */
#include "cmre.h"

u16 CM_THISCALL cm_fn_00717400(void *self)
{
  return *(u16 *)((u32)self + 0x1bc);
}
