/* fn_005e9a80, module comp\cups\gre_cup.cpp.
   Recovered from the original instructions at preferred-image VA 0x005e9a80
   (image base 0x00400000), CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Scratch locals keep the original frame addresses:
     L+0x000  byte buffer passed to cm_fn_00944cff / cm_fn_0090d130
     L+0x100  second byte buffer
     L+0x200  100 DWORD slots; only the first `bl` entries are written by the
              first scan, yet 0x005e9b7b copies 16 of them unconditionally.
              They stay uninitialized here exactly as in the original frame. */
#include "cmre.h"

/* Callees stay explicit boundaries.  cm_fn_004afbd0 takes three stack arguments
   here: the original pushes 0x10 at 0x005e9b60, then EDX (0x005e9b69), then EAX
   (0x005e9b6a), and removes 0xc bytes at 0x005e9b70, so the callee at that site
   sees base/count/wanted on the stack.  The entering EAX (`lea` at 0x005e9b62)
   and EDX (`movsx dx, bl` at 0x005e9b5c, upper half kept) are forwarded in the
   registers at the call. */
extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted);
extern void cm_fn_005e8290(const char * param_1, const char * param_2, int param_3);
extern char * cm_fn_0090d130(void * arg1, void * arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void * arg4, void * arg5);
extern void * CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

#define D_009cf86c CM_GLOBAL(u32, 0x009cf86c)
#define D_009cf870 CM_GLOBAL(u32, 0x009cf870)
#define D_009cf874 CM_GLOBAL(u32, 0x009cf874)
#define D_00ae235c CM_GLOBAL(u32, 0x00ae235c)
#define D_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define D_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define A_009cdab0 ((u32)0x009cdab0)
#define A_009870e0 ((u32)0x009870e0)
#define A_009870e8 ((u32)0x009870e8)
#define A_00ae24d0 ((u32)0x00ae24d0)

u32 CM_THISCALL cm_fn_005e9a80(u32 self)
{
  u32 L[0x390 / 4];                              /* uninitialized scratch, 0x390 bytes */
  u8 * const buf_a = (u8 *)L + 0x000;
  u8 * const buf_b = (u8 *)L + 0x100;
  int * const slots = (int *)((u8 *)L + 0x200);
  s8 bl;
  u32 i;
  u32 eax;
  u32 base;
  u32 count;
  u32 edx;
  u32 ebx;
  u32 ebp;

  bl = 0;
  *(volatile u32 *)(self + 0xf2) = 0x2c;
  eax = (u32)cm_fn_00945501(4, 0x2c);
  *(volatile u32 *)(self + 0xee) = eax;
  if (eax == 0) {
    cm_fn_00944cff(A_009cdab0, 0, 0, (void *)buf_a, (void *)buf_b);
    i = (u32)cm_fn_0090d130(buf_a, buf_b, 0x2f2);
    /* The three pushes for cm_fn_0090d130 (0x005e9ad5-0x005e9adb) are still on
       the stack here; a single `add esp,0x24` at 0x005e9b02 clears them after
       cm_fn_005e8290.  The format at 0x009870e8 therefore reads them as its
       variadic arguments: name, extension, line. */
    cm_fn_00944c9f(A_00ae24d0, A_009870e8, i, (u32)buf_a, (u32)buf_b, 0x2f2);
    cm_fn_005e8290((const char *)A_009870e0, (const char *)A_00ae24d0, 0);
    D_00b67a34 = 0;
    return 0;
  }

  base = D_00ae23b4;
  count = D_00ae235c;
  edx = 0;
  if ((s32)count > 0) {
    u32 grade = D_009cf874;
    do {
      u32 p = *(volatile u32 *)(base + 0x57);
      if (p != 0 && *(u32 *)p == grade && bl < 0x64) {
        u32 k = (u32)(s32)bl;
        bl = (s8)(bl + 1);
        slots[k] = (int)base;
      }
      edx = edx + 1;
      base = base + 0x245;
    } while ((s32)(s16)(u16)edx < (s32)count);
  }

  edx = (edx & 0xffff0000u) | (u32)(u16)(s16)bl;
  eax = (u32)slots;
  cm_fn_004afbd0(eax, edx, 0x10);

  eax = 0;
  do {
    u32 v = (u32)slots[eax];
    u32 dst = *(volatile u32 *)(self + 0xee);
    *(u32 *)(dst + eax * 4) = v;
    eax = eax + 1;
  } while (eax < 0x10);

  count = D_00ae235c;
  base = D_00ae23b4;
  ebx = 0;
  if ((s32)count > 0) {
    do {
      u32 p = *(volatile u32 *)(base + 0x57);
      if (p != 0) {
        p = *(u32 *)p;
        if (p == D_009cf870 && (s32)eax < (s32)*(volatile u32 *)(self + 0xf2)) {
          u32 dst = *(volatile u32 *)(self + 0xee);
          eax = eax + 1;
          *(u32 *)(dst + eax * 4 - 4) = base;
          count = D_00ae235c;
        }
      }
      ebx = ebx + 1;
      base = base + 0x245;
    } while ((s32)(s16)(u16)ebx < (s32)count);
  }

  base = D_00ae23b4;
  ebp = 0;
  if ((s32)count > 0) {
    do {
      u32 p = *(volatile u32 *)(base + 0x57);
      if (p != 0) {
        p = *(u32 *)p;
        if (p == D_009cf86c && (s32)eax < (s32)*(volatile u32 *)(self + 0xf2)) {
          u32 dst = *(volatile u32 *)(self + 0xee);
          eax = eax + 1;
          *(u32 *)(dst + eax * 4 - 4) = base;
          count = D_00ae235c;
        }
      }
      ebp = ebp + 1;
      base = base + 0x245;
    } while ((s32)(s16)(u16)ebp < (s32)count);
  }

  if ((s32)eax != (s32)*(volatile u32 *)(self + 0xf2)) {
    cm_fn_00944cff(A_009cdab0, 0, 0, (void *)buf_b, (void *)buf_a);
    i = (u32)cm_fn_0090d130(buf_b, buf_a, 0x322);
    /* Same deferred stack arguments (0x005e9c52-0x005e9c58, cleaned at 0x005e9c7f);
       the buffers are exchanged relative to the allocation-failure path. */
    cm_fn_00944c9f(A_00ae24d0, A_009870e8, i, (u32)buf_b, (u32)buf_a, 0x322);
    cm_fn_005e8290((const char *)A_009870e0, (const char *)A_00ae24d0, 0);
    D_00b67a34 = 0;
  }
  return 1;
}
