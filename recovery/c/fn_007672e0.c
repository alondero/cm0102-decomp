/* Preferred-image VA 0x007672e0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical thiscall with one caller-supplied 16-bit stack word, RET 4.
   ECX is the receiver. The signed byte at receiver+0xa1 is a count. If it is
   positive and the word at receiver+0x26 differs from the argument, the routine
   walks 11-byte records starting at receiver+0x26, increments each record's word,
   and stores 0xffffffff into the dword at record-9 and the dword at record-5.
   The count is re-read from memory on every iteration, so the stores can alias
   the count byte at receiver+0xa1 (record 12, dword at 0xa1) and change the loop
   bound. When the count is positive, EAX is set to receiver+0x26 and advanced by
   11 per iteration; EDX ends at the loop index. If the count is not positive,
   EAX and EDX keep their entry values. If the word equals the argument, EAX is
   receiver+0x26, DX contains the argument, and EDX keeps its entry upper 16 bits.
   No semantic return is produced;
   direct callers do not observe the EAX or EDX left by this callee. */
#include "cmre.h"

void CM_THISCALL cm_fn_007672e0(u8 *self, u16 arg)
{
  s32 index;
  u16 *word;

  if (*(s8 *)(self + 0xa1) > 0) {
    word = (u16 *)(self + 0x26);
    if (arg != *word) {
      index = 0;
      if (*(s8 *)(self + 0xa1) > 0) {
        do {
          *word = (u16)(*word + 1);
          *(u32 *)((u8 *)word - 9) = 0xffffffffu;
          *(u32 *)((u8 *)word - 5) = 0xffffffffu;
          index = index + 1;
          word = (u16 *)((u8 *)word + 0xb);
        } while (index < (s32)*(s8 *)(self + 0xa1));
      }
    }
  }
  return;
}
