/* fn_005603e0 — display.cpp (CM 01/02 3.9.68), preferred-image VA 0x005603e0.
   regparm(3) names the actual entry EAX/EDX/ECX. ECX is the display object; the
   only stack argument (RET 4) is read as a 16-bit word (`mov dx,[esp+4]`). The
   -1 path jumps straight to the epilogue, so EAX keeps the incoming value and
   the routine returns u32. EDX is only ever written (DX), never needed. */
#include "cmre.h"

extern u32 cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(char *param_1, char *param_2, u32 param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define s_Error_009870e0 ((const char *)0x009870e0)
#define s_Fmt_009870e8 0x009870e8u
#define s_Src_009c36ec 0x009c36ecu

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_005603e0(
    u32 incoming_eax, u32 incoming_edx, u8 *obj, u32 param_2)
{
  u8 scratch[0x200];
  u8 *bufA = scratch;         /* lea eax, [esp]         */
  u8 *bufB = scratch + 0x100; /* lea ecx, [esp + 0x100] */
  u32 value;
  u32 result;
  s16 count;

  (void)incoming_edx;

  if ((u16)param_2 == 0xffffu) {
    return incoming_eax;      /* cmp dx, -1 ; je 0x560480 (EAX untouched) */
  }

  count = *(s16 *)(obj + 0x12f4f6); /* mov ax, word ptr [ecx + 0x12f4f6] */

  if (count < 0xfa) {         /* cmp ax, 0xfa ; jl 0x56046e */
    *(s16 *)(obj + 0x12e9a2 + (s32)count * 2) = (s16)(u16)param_2;
    *(s16 *)(obj + 0x12f4f6) = (s16)(*(s16 *)(obj + 0x12f4f6) + 1);
    return (u32)(s32)count;   /* movsx eax, ax */
  }

  cm_fn_00944cff(s_Src_009c36ec, 0, 0, bufB, bufA);
  value = (u32)cm_fn_0090d130((char *)bufB, (char *)bufA, 0x302u);
  /* The original leaves 90d130's three arguments on the stack, so the
     variable-argument callee sees them after the explicitly pushed value:
     [esp+0xc]=value, [esp+0x10]=bufB, [esp+0x14]=bufA, [esp+0x18]=0x302. */
  cm_fn_00944c9f((u32)(&DAT_00ae24d0), s_Fmt_009870e8, value, (u32)bufB,
                 (u32)bufA, 0x302u);
  result = cm_fn_005e8290(s_Error_009870e0, (const char *)(&DAT_00ae24d0), 0);
  DAT_00b67a34 = 0;
  return result;              /* EAX still holds the callee's result */
}
