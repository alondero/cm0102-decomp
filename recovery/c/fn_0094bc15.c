/* fn_0094bc15 - __fload_withFB, preferred-image VA 0x0094bc15.
   Loads the double at the EDX pointer onto the x87 stack; for infinities and
   NaNs it rebuilds the 80-bit extended form by hand (exponent 0x7fff, sign
   preserved) instead of a plain FLD. The rebuilt value stays in ST(0) as the
   80-bit extended form: the original never narrows it to a double. */
#include "cmre.h"

typedef union {
  long double ext;
  struct {
    u32 sig_lo;
    u32 sig_hi;
    u16 exp;
  } f;
} cm_ext80;

long double CM_FASTCALL cm_fn_0094bc15(void *unused_ecx, double *value)
{
  (void)unused_ecx;
  volatile u32 *words = (volatile u32 *)value;
  u32 hi = words[1];

  if ((hi & 0x7ff00000u) != 0x7ff00000u) {
    return (long double)*value;
  }

  cm_ext80 e;
  u32 exp_bits = words[1];
  e.f.exp = (u16)(0x7fffu | (exp_bits >> 16));
  u32 sig_bits = words[1];
  u32 lo = words[0];
  e.f.sig_hi = (sig_bits << 11) | (lo >> 21);
  e.f.sig_lo = lo << 11;
  return e.ext;
}
