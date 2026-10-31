/* Preferred-image VA 0x006826d0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: mov eax, [esp+4]; lea/sub/lea/lea computes EAX*59 (0x3b) in EDX;
   mov eax, [ecx+0xb1]; add eax, edx; ret 4. Thiscall with one stack argument
   (popped with ret 4): ECX is the object pointer and the argument is a 32-bit
   value. EAX returns the dword at this+0xb1 plus arg*0x3b, modulo 2^32.
   Direct callers do not observe EDX left by this callee: the caller at
   0x0048cdcc rewrites DL before reading DL. Field meaning remains unknown. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg)
{
  u32 scaled = arg * 0x3b;
  return *(u32 *)((u8 *)self + 0xb1) + scaled;
}
