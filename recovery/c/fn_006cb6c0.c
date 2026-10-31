/* WIP: not accepted although the gate passes. Callers use only AX (movsx ecx, ax;
   movsx eax, di; sub ax, ...), so the faithful return is 16-bit, but the wide-EAX
   scan flags `push eax` (0x006ffd40) and `mov edi, eax` (0x00713de8). The u32 form
   zeroes EAX bits 16-31 on the a == 0 && b == 0 path, where the original keeps the
   entry EAX; random inputs rarely reach that path (pilot 2026-10-19). */
/* Preferred-image VA 0x006cb6c0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: ECX is read before it is written and is the `this` pointer. Four
   byte arguments sit in 4-byte stack slots at entry esp+4, +8, +0xc, +0x10
   (only the low byte of each slot is read). The routine ends with ret 0x10.
   With a = (byte)(arg3 - arg1) and b = (byte)(arg4 - arg2):
   - if a == 0 and b == 0, it returns the 16-bit word at [this + 0x8ed8]
     (mov ax, word ptr [ecx + 0x8ed8]);
   - otherwise EAX = 3 * a (sign-extended bytes, 32-bit), then the 16-bit word
     at 0x00b67c30 + 2 * (b + 24 * a) replaces the low half (mov ax, ...).
   Callers read EAX beyond 16 bits, so the return is declared u32. The high
   half is the sign of 3 * a on the second path. On the first path the high
   half is left from the caller's EAX (mov al/mov ax never touch it), which
   this C signature does not carry; that path is not yet expressible here.
   The meaning of the global table at 0x00b67c30 and of the field at
   this + 0x8ed8 is not named here. */
#include "cmre.h"

u32 CM_THISCALL cm_fn_006cb6c0(u8 *self, u8 arg1, u8 arg2, u8 arg3, u8 arg4)
{
  u8 a = (u8)(arg3 - arg1);
  u8 b = (u8)(arg4 - arg2);
  s32 three_a;
  s32 idx;

  if (a == 0 && b == 0) {
    return *(u16 *)(self + 0x8ed8u);
  }
  three_a = 3 * (s32)(s8)a;
  idx = (s32)(s8)b + 8 * three_a;
  return (((u32)three_a) & 0xffff0000u)
       | *(u16 *)(0x00b67c30u + (u32)(idx * 2));
}
