/* Preferred-image VA 0x00518690, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov dword ptr [ecx], 0x9693c8; jmp 0x48c650. Thiscall with one
   pointer argument in ECX: stores the constant 0x009693c8 into the first dword of
   the object, then tail-jumps to the base cleanup at 0x0048c650 with ECX unchanged.
   No stack bytes are popped here; the callee's own return pops the return address.
   The tail jump passes the callee's EDX:EAX through, which is not declared as a
   return value here. The meaning of 0x009693c8 remains unknown. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_0048c650(void *);

void CM_THISCALL cm_fn_00518690(void *self_arg)
{
  *(u32 *)self_arg = 0x009693c8u;
  cm_fn_0048c650(self_arg);
}
