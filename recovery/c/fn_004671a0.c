/* Preferred-image VA 0x004671A0, image base 0x00400000.
   Listing: mov eax, [esp+4]; mov [ecx], eax; ret 4. Thiscall with one stack
   argument (callee pops 4 bytes). Stores the argument as a 32-bit value at the
   receiver's offset 0. Callers do not read EAX, so the return is void. */
#include "cmre.h"

void CM_THISCALL cm_fn_004671a0(void *self, u32 arg1)
{
  *(u32 *)self = arg1;
}
