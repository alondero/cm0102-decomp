/* Tier-2 C recovery for preferred-image VA 0x0094c490 (cdecl, ret 0).
   Arguments are named by their [ebp+N] stack offset. The 0x24-byte local
   area [ebp-0x24, ebp) is one scratch array; frame[i] is the dword at
   [ebp-0x24+4*i]. &frame[0] is the record pointer passed to 0x0094cc21
   (lea eax, [ebp-0x24] at 0x0094c531). This routine stores only frame[2],
   frame[4], frame[5] and frame[7], in listing order; it never reads them
   itself, but the callee reads the record (frame[0], frame[1], frame[2],
   frame[5], frame[7]). frame[0] and frame[1] are never written by this
   routine in the original either: on the arg_20 == -1 path the callee
   consumes stale stack bytes there. Their values are unknown and are left
   uninitialized here exactly as in the listing, not invented. */
#include "cmre.h"

extern void cm_fn_0094c9ae(void);
extern u32 CM_CDECL cm_fn_0094cc21(const volatile u8 *record);

s32
cm_fn_0094c490(u32 arg_08, u32 arg_0c, u32 arg_10, u32 arg_14,
               u32 arg_18, u32 arg_1c, u32 arg_20)
{
  u32 frame[9];
  u32 ebx;
  u32 esi;
  u32 edi;
  u32 eax;
  u32 ecx;
  u32 edx;

  ebx = arg_08 - 0x76c;
  if ((s32)ebx < 0x46 || (s32)ebx > 0x8a) {
    return -1;
  }
  edi = arg_0c;
  esi = *(volatile u32 *)(0x00ad9858u + edi * 4u);
  esi = esi + arg_10;
  if ((ebx & 3) == 0 && (s32)edi > 2) {
    esi = esi + 1;
  }
  cm_fn_0094c9ae();

  eax = ebx * 0x16d;
  ecx = (u32)((s32)(ebx - 1) >> 2);
  edx = esi;
  ((volatile u32 *)frame)[7] = esi;
  edx = edx + ecx;
  ((volatile u32 *)frame)[5] = ebx;
  eax = eax + edx;
  edx = arg_1c;
  ecx = eax * 3u;
  eax = arg_14;
  ((volatile u32 *)frame)[2] = eax;
  ecx = eax + ecx * 8u;
  ecx = ecx * 0x3cu;
  ecx = ecx + arg_18;
  ecx = ecx * 0x3cu;
  ecx = ecx + CM_GLOBAL(volatile u32, 0x00ad91f8);
  edi = edi - 1;
  ((volatile u32 *)frame)[4] = edi;
  ecx = ecx + edx + 0x7c558180u;
  arg_08 = ecx;

  if (arg_20 == 1) {
    ecx = ecx + CM_GLOBAL(volatile u32, 0x00ad9200);
    return (s32)ecx;
  }
  if (arg_20 == 0xffffffffu && CM_GLOBAL(volatile u32, 0x00ad91fc) != 0) {
    eax = cm_fn_0094cc21((const volatile u8 *)frame);
    ecx = arg_08;
    if (eax != 0) {
      ecx = ecx + CM_GLOBAL(volatile u32, 0x00ad9200);
    }
  }
  return (s32)ecx;
}
