/* Preferred-image VA 0x0058d710, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall with five stack arguments (ret 0x14). ECX is read before it
   is written and holds `this`; [this] is a base address. The record index is a
   signed 32-bit value read through the second argument pointer; each record is
   44 bytes. The routine walks a list of 5-byte entries from the last row down to
   row 0 (signed 16-bit loop), reads an 8-bit value at entry+2 and a 16-bit value
   at entry+3, and forms key = byte + word * 12 (32-bit). It counts entries with
   key in the closed interval [lower, upper], stopping at the first key below
   lower. lower = (signed byte arg3) + (arg4 & 0xffff) * 12 and
   upper = (signed byte arg5) + (arg6 & 0xffff) * 12. The count is returned in AX
   (16 bits); EAX upper bits are not written. Field meanings stay unnamed. */
#include "cmre.h"

u16 CM_THISCALL cm_fn_0058d710(u32 *self, s32 *index_ptr, s8 arg3, u32 arg4, s8 arg5, u32 arg6)
{
  u32 index;
  u32 rec;
  u32 table;
  s16 row;
  u16 count;
  s32 lower;
  s32 upper;

  count = 0;
  lower = (s32)((u32)(s32)arg3 + (arg4 & 0xffffu) * 12u);
  upper = (s32)((u32)(s32)arg5 + (arg6 & 0xffffu) * 12u);

  /* Memory reads happen in the listing's order: index, then *self, then the row word. */
  index = (u32)*index_ptr;
  rec = *self + index * 0x2cu;
  row = (s16)(*(u16 *)(rec + 10) - 1);

  if (row >= 0) {
    table = *(u32 *)(rec + 4);
    do {
      u32 entry;
      u32 word;
      u32 byte;
      u32 key;

      entry = table + (u32)(s32)row * 5u;
      word = (u32)*(u16 *)(entry + 3);
      byte = (u32)*(u8 *)(entry + 2);
      key = byte + word * 12u;
      if ((s32)key < lower) {
        break;
      }
      if ((s32)key <= upper) {
        count = (u16)(count + 1);
      }
      row = (s16)(row - 1);
    } while (row >= 0);
  }
  return count;
}
