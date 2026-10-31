/* Preferred-image VA 0x007160E0, image base 0x00400000.
   Listing: mov al, [esp+4]; mov [ecx+0x2b], al; ret 4. Thiscall with one stack
   argument (callee pops 4 bytes). Stores the low byte of the argument at receiver
   offset 0x2B. Callers do not read EAX, so the return is void. */
#include "cmre.h"

void CM_THISCALL cm_fn_007160e0(void *self, u32 arg1)
{
  *(u8 *)((u32)self + 0x2b) = (u8)arg1;
}
