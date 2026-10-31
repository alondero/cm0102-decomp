/* Preferred-image VA 0x007a7eb0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: one stack argument (a pointer to a 32-bit index, read at [esp+4]).
   The routine loads that index, multiplies it by 79 (lea/shl/sub: 5 * 16 - 1),
   adds 0xb and the 32-bit value stored at global 0x00ae2d0c, and reads one byte
   from that address. It returns bits 4..5 of that byte in AL (shr 4, and 3).
   RET 4 pops the one argument (stdcall). The upper EAX bits are not part of the
   declared u8 return. The global's meaning and the record layout remain unknown. */
#include "cmre.h"

u8 CM_STDCALL cm_fn_007a7eb0(s32 *param_1)
{
  u32 index;
  u32 base;
  u8 value;

  index = (u32)*param_1;
  base = (u32)CM_GLOBAL(s32, 0x00ae2d0c);
  value = *(u8 *)(index * 79u + 0xbu + base);
  return (u8)((value >> 4) & 3u);
}
