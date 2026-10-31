/* Preferred-image VA 0x00412ef0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov dword ptr [ecx], 0x00967864; ret. ECX is read before it is
   written and holds the destination address. The routine stores the immediate
   0x00967864 as a 32-bit word at [ECX]. No stack bytes are popped and no return
   value is produced. The meaning of 0x00967864 is not named here. */
#include "cmre.h"

void CM_THISCALL cm_fn_00412ef0(u32 *dst)
{
  *dst = 0x00967864u;
}
