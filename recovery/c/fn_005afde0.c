/* Preferred-image VA 0x005afde0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical thiscall with one caller-supplied stack word, RET 4.
   Stack word NULL returns 0 without touching ECX or the global.
   Otherwise EAX = u32 at ECX + (global 0x00ae2364 - *arg) * 4 - 4, computed with
   32-bit wraparound. The global's and array's meanings remain unknown.
   Only EAX is read by callers. Slow-path PUSH/POP ESI preserves ESI. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_005afde0(u32 base, u32 *arg)
{
  u32 index;

  if (arg == (u32 *)0) {
    return 0;
  }
  index = CM_GLOBAL(u32, 0x00ae2364) - *arg;
  return *(u32 *)(base + index * 4u - 4u);
}
