/* fn_0090b530 at preferred-image VA 0x0090b530, image base 0x00400000.
   Reconstructed from the original instructions; CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Scratch/stack locals (L = ESP after `sub esp,0x398`), offsets preserved:
     L+0x003  byte      second-array match count
     L+0x004  dword     record scan index
     L+0x008  dword[50] first matched-record array
     L+0x0d0  dword[50] second matched-record array
     L+0x198  byte[256] error buffer 1
     L+0x298  byte[256] error buffer 2
   The matched-record arrays are deliberately left uninitialized: the original
   unconditionally copies the first ten dwords of each array after the scans,
   even when a scan selected fewer than ten (004afbd0 then leaves the tail
   untouched).  No initialization, padding or low-count suppression is added. */
#include "cmre.h"

#define DAT_009cf590 CM_GLOBAL(u32, 0x009cf590)
#define DAT_009cf594 CM_GLOBAL(u32, 0x009cf594)
#define DAT_009cf598 CM_GLOBAL(u32, 0x009cf598)
#define DAT_00ae235c CM_GLOBAL(s32, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(u32, 0x00ae23d0)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char * CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void * CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_FASTCALL cm_fn_0090b530(u32 self)
{
  u8 *obj = (u8 *)self;
  u8 scratch[0x398];
  u32 iVar1;
  u32 iVar2;
  u32 rec;
  u32 p;
  u32 off;
  u32 d;
  u32 val;
  u32 count1;
  u32 count2;
  s32 loopIndex;
  s32 i;
  s32 k;
  u8 c1;
  u8 bl;

  *(u32 *)(scratch + 0x004) = 0;
  *(u16 *)(obj + 0x3e) = 0x20;

  iVar1 = (u32)DAT_009cf594 * 0x6b + (u32)DAT_00ae23d0;
  iVar2 = (u32)DAT_009cf598 * 0x6b + (u32)DAT_00ae23d0;

  c1 = 0;
  scratch[0x003] = 0;
  loopIndex = 0;

  if (DAT_00ae235c > 0) {
    rec = (u32)DAT_00ae23b4;
    do {
      if (c1 == 0x32 && scratch[0x003] == 0x32)
        break;
      p = *(u32 *)(rec + 0x57);
      if (p == iVar1 && (s8)c1 < 0x32) {
        *(u32 *)(scratch + 0x008 + (u32)(s32)(s8)c1 * 4) = rec;
        c1 = (u8)(c1 + 1);
      }
      else if (p == iVar2 && (s8)scratch[0x003] < 0x32) {
        *(u32 *)(scratch + 0x0d0 + (u32)(s32)(s8)scratch[0x003] * 4) = rec;
        scratch[0x003] = (u8)(scratch[0x003] + 1);
      }
      rec += 0x245;
      loopIndex = loopIndex + 1;
    } while (loopIndex < DAT_00ae235c);
  }

  /* `movsx ax, cl`: the upper half of EAX still holds iVar1's upper half. */
  count1 = (iVar1 & 0xffff0000u) | (u32)(u16)(s16)(s8)c1;
  /* `movsx dx, <count2 byte>`: only the low half is rebuilt. */
  count2 = (u32)(s16)(s8)scratch[0x003];

  cm_fn_004afbd0((u32)(void *)(scratch + 0x008), count1, 10);
  cm_fn_004afbd0((u32)(void *)(scratch + 0x0d0), count2, 10);

  d = (u32)(void *)cm_fn_00945501((u32)(s32)*(s16 *)(obj + 0x3e), 6);
  *(u32 *)(obj + 0xa7) = d;
  if (d == 0) {
    cm_fn_00944cff((u32)0x00aa20e0, 0, 0,
                   (void *)(scratch + 0x298), (void *)(scratch + 0x198));
    val = (u32)(void *)cm_fn_0090d130();
    cm_fn_00944c9f((u32)0x00ae24d0, (u32)0x009870e8, val,
                   (u32)(void *)(scratch + 0x298),
                   (u32)(void *)(scratch + 0x198), (u32)0x138);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);
    DAT_00b67a34 = 0;
    return 0xffffffffu;
  }

  bl = 0;

  /* Unconditional ten-dword copy of the first matched-record array. */
  for (k = 0; k < 10; k++) {
    d = *(volatile u32 *)(obj + 0xa7);
    val = *(u32 *)(scratch + 0x008 + (u32)k * 4);
    off = (u32)(s32)(s8)bl * 6;
    bl = (u8)(bl + 1);
    *(u32 *)(d + off) = val;
    d = *(volatile u32 *)(obj + 0xa7);
    *(u8 *)(d + off + 4) = 2;
    d = *(volatile u32 *)(obj + 0xa7);
    *(u8 *)(d + off + 5) = 0;
  }

  /* Unconditional ten-dword copy of the second matched-record array. */
  for (k = 0; k < 10; k++) {
    d = *(volatile u32 *)(obj + 0xa7);
    val = *(u32 *)(scratch + 0x0d0 + (u32)k * 4);
    off = (u32)(s32)(s8)bl * 6;
    bl = (u8)(bl + 1);
    *(u32 *)(d + off) = val;
    d = *(volatile u32 *)(obj + 0xa7);
    *(u8 *)(d + off + 4) = 2;
    d = *(volatile u32 *)(obj + 0xa7);
    *(u8 *)(d + off + 5) = 0;
  }

  /* Remaining records whose +0x57 object begins with DAT_009cf590. */
  i = 0;
  rec = (u32)DAT_00ae23b4;
  if (DAT_00ae235c > 0) {
    do {
      p = *(u32 *)(rec + 0x57);
      if (p != 0) {
        val = *(u32 *)(void *)p;
        if (val == (u32)DAT_009cf590) {
          off = (u32)(s32)(s8)bl * 6;
          bl = (u8)(bl + 1);
          d = *(volatile u32 *)(obj + 0xa7);
          *(u32 *)(d + off) = rec;
          d = *(volatile u32 *)(obj + 0xa7);
          *(u8 *)(d + off + 4) = 1;
          d = *(volatile u32 *)(obj + 0xa7);
          *(u8 *)(d + off + 5) = 0;
        }
      }
      i = i + 1;
      rec += 0x245;
    } while (i < DAT_00ae235c);
  }

  return 1;
}
