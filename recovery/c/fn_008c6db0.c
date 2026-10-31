#include "cmre.h"

/* Callees: each thiscall/fastcall receiver is the ECX value set in the listing
   before the call. Stack arguments are dwords (u32), as pushed. */
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_004cc9e0(u32 self, u32 arg1, u32 arg2);
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 self);

/* Every exit path reaches `return 0` with EAX zero: the loop only ends when
   cm_fn_0068d1b0 returns 0 in EAX, and the other exits leave EAX at a zero
   test result. */
u32 CM_STDCALL
cm_fn_008c6db0(u32 param_1)
{
  u32 f4;
  u32 f8;
  u32 fc;
  u32 f0c;
  u32 g1;
  u32 g2;
  u32 g3;
  u32 x;
  u32 r1;
  u32 r2;
  u32 r3;
  u32 r4;
  u32 edi;
  u32 ecx;
  u32 edx;
  u32 eax;

  f0c = *(volatile u32 *)(param_1 + 0xcu);
  g2 = CM_GLOBAL(volatile u32, 0x00ae23bc);
  x = g2 + f0c * 110u;
  g1 = CM_GLOBAL(volatile u32, 0x00ae1050);
  r1 = cm_fn_004cf5a0(g1, x);
  CM_GLOBAL(volatile u32, 0x00ae1080) = r1;
  if (r1 != 0) {
    f4 = *(volatile u32 *)(r1 + 4u);
    g3 = CM_GLOBAL(volatile u32, 0x00ae23b4);
    edi = g3 + f4 * 581u;
  } else {
    edi = 0;
  }
  f8 = *(volatile u32 *)(param_1 + 8u);
  fc = *(volatile u32 *)(param_1 + 0xcu);
  g1 = CM_GLOBAL(volatile u32, 0x00ae1050);
  r2 = cm_fn_004cc9e0(g1, fc, f8);
  if (r2 != 0) {
    *(volatile u8 *)(r2 + 0x50u) = 4;
  }
  r3 = *(volatile u32 *)(param_1 + 4u);
  if (r3 != 0) {
    ecx = *(volatile u32 *)(r3 + 2u);
    *(volatile u32 *)(r3 + 0xau) = ecx;
    r4 = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 4u));
    while (r4 != 0) {
      edx = *(volatile u32 *)edi;
      eax = *(volatile u32 *)r4;
      g1 = CM_GLOBAL(volatile u32, 0x00ae1050);
      r2 = cm_fn_004cc9e0(g1, eax, edx);
      if (r2 != 0) {
        *(volatile u8 *)(r2 + 0x50u) = 4;
      }
      r4 = cm_fn_0068d1b0(*(volatile u32 *)(param_1 + 4u));
    }
  }
  return 0;
}
