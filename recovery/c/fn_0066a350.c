/* Preferred-image VA 0x0066a350, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: cmp byte ptr [ecx + 9], 0x10; setg al; inc eax; ret.
   Thiscall with no stack input (RET 0). ECX is the object pointer and is read
   before it is written. The routine compares the signed byte at ECX + 9 with 16,
   sets AL to 1 when it is greater and 0 otherwise, then increments EAX.
   Only the low byte is declared as the return width, since the increment's
   result in the low byte is 1 or 2 and the upper EAX bits are not produced by
   this routine's own logic. The struct field's meaning remains unknown. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_0066a350(u8 *self)
{
  return (u8)((*(signed char *)(self + 9) > 0x10) + 1);
}
