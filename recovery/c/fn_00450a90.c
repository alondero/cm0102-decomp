/* Preferred-image VA 0x00450A90, image base 0x00400000.
   Listing: mov eax, ecx; mov ecx, [esp+4]; mov [eax], ecx; ret 4. Thiscall with one
   stack argument (callee pops 4 bytes). Stores the argument as a 32-bit value at
   the receiver's offset 0, and leaves the receiver itself in EAX. Callers at
   0x0044D889, 0x004501AA, 0x004503CF and 0x0045056F read EAX, so the return is
   the 32-bit receiver address. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_00450a90(void *self, u32 arg1)
{
  *(u32 *)self = arg1;
  return (u32)self;
}
