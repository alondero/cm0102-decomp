/* Preferred-image VA 0x009469FA, image base 0x00400000.
   Listing: mov eax, [esp+4]; mov [0x00AD6C30], eax; ret. Cdecl (ret 0), one
   stack argument. Stores the 32-bit argument to the global at 0x00AD6C30 and
   returns that same value in EAX (a caller at 0x00686BFA reads EAX). */
#include "cmre.h"

u32 CM_CDECL cm_fn_009469fa(u32 arg1)
{
  CM_GLOBAL(u32, 0x00ad6c30) = arg1;
  return arg1;
}
