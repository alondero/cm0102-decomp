/* Preferred-image VA 0x007A6E20, image base 0x00400000.
   Listing: mov eax, [ecx+0x20]; test eax, eax; setne al; ret. Thiscall, no stack
   arguments. Only AL is written (set-if-not-zero of the dword at receiver offset
   0x20), so the return is 8-bit. */
#include "cmre.h"

u8 CM_THISCALL cm_fn_007a6e20(void *self)
{
  u32 value = *(u32 *)((u32)self + 0x20);
  return (u8)(value != 0);
}
