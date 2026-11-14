/* fn_0062e820 (injury.cpp), preferred-image VA 0x0062e820, image base 0x00400000.
   thiscall: ECX = injury-list object. ret 0x10 -> four stack arguments:
     arg1 (L+0x20c) u32*   -> passed as param_3 of cm_fn_00417c10
     arg2 (L+0x210) byte   -> record type byte (only the low byte is read)
     arg3 (L+0x214) int    -> record value
     arg4 (L+0x218) u32*   -> optional extra value (0xffffffff when null)
   L = frame base right after `sub esp,0x208`; three saved registers occupy
   L-12..L-1, so the listing's [esp+N] references resolve as N-12 relative to L.
   Object fields are byte offsets: 0x1c = record base, 0x20 = capacity,
   0x24 = count. The three values pushed before `call 0x90d130` are not its
   arguments: they stay on the stack and become the trailing variadic
   arguments of the following cm_fn_00944c9f call. */
#include "cmre.h"

#define DAT_00ae23b4 CM_GLOBAL(s32, 0x00ae23b4)
#define DAT_00ae24d0 CM_GLOBAL(u32, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

#define FIELD(selfb, off, type) (*(volatile type *)(void *)((selfb) + (off)))

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_00945333(u32 eax_in, u32 edx_in, volatile u32 base_arg, volatile u32 count_arg, volatile u32 width_arg, volatile u32 compare_arg);
extern unsigned long long CM_CDECL cm_fn_00945667(u32 arg8, u32 argC);

void CM_THISCALL
cm_fn_0062e820(u32 param_1, u32 param_2, char param_3, int param_4, u32 *param_5)
{
  volatile u8 *self = (volatile u8 *)(void *)param_1;
  u8 scratch[0x208];
  u32 *buf_lo = (u32 *)(void *)scratch;             /* L + 0 */
  u32 *buf_mid = (u32 *)(void *)(scratch + 0x8);     /* L + 0x8 */
  u32 *buf_hi = (u32 *)(void *)(scratch + 0x108);   /* L + 0x108 */

  if (param_3 == 4) {
    return;
  }
  if (param_3 == 3) {
    if (cm_fn_0090cfc0(0, 0, 0xf) != 0) {
      return;
    }
  } else {
    if (cm_fn_0090cfc0(0, 0, 4) == 0) {
      return;
    }
  }
  {
    /* ecx = DAT_00ae23b4 + value; edx = ecx + value*0x244  =>  base + value*0x245 */
    u32 idx = (u32)param_4;
    u32 *entry = (u32 *)(void *)((u32)DAT_00ae23b4 + idx + idx * 580u);
    if (cm_fn_0053b900(entry) != 0) {
      return;
    }
  }
  {
    s32 count = FIELD(self, 0x24, s32);
    s32 capacity = FIELD(self, 0x20, s32);
    if (count >= capacity) {
      u32 base = FIELD(self, 0x1c, u32);
      u32 newcap = (u32)capacity + 0x1e;
      u32 grown;
      FIELD(self, 0x20, u32) = (u32)newcap;
      if (base == 0) {
        grown = (u32)cm_fn_00944e46((u32)newcap * 0x11u);
      } else {
        grown = (u32)cm_fn_00945667(base, (u32)newcap * 0x11u);
      }
      FIELD(self, 0x1c, u32) = grown;
      if (grown == 0) {
        u32 text;
        cm_fn_00944cff(0x009e7f28, 0, 0, buf_hi, buf_mid);
        text = cm_fn_0090d130();
        cm_fn_00944c9f((u32)&DAT_00ae24d0, 0x009870e8, text, (u32)buf_hi, (u32)buf_mid, 0xd01);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
        DAT_00b67a34 = 0;
        return;
      }
    }
  }
  {
    u32 base = FIELD(self, 0x1c, u32);
    s32 count = FIELD(self, 0x24, s32);
    u32 *record = (u32 *)(void *)(base + (u32)count * 0x11u);
    cm_fn_00417c10(record, buf_lo, (u16 *)(void *)param_2);
    {
      u32 base8 = FIELD(self, 0x1c, u32);
      s32 count8 = FIELD(self, 0x24, s32);
      volatile u8 *rec8 = (volatile u8 *)(void *)(base8 + (u32)count8 * 0x11u);
      rec8[8] = (u8)param_3;
    }
    {
      u32 base9 = FIELD(self, 0x1c, u32);
      s32 count9 = FIELD(self, 0x24, s32);
      volatile u8 *rec9 = (volatile u8 *)(void *)(base9 + (u32)count9 * 0x11u);
      *(volatile u32 *)(void *)(rec9 + 9) = (u32)param_4;
    }
    {
      u32 extra = (param_5 == (u32 *)0) ? 0xffffffffu : *param_5;
      u32 based = FIELD(self, 0x1c, u32);
      s32 countd = FIELD(self, 0x24, s32);
      volatile u8 *recd = (volatile u8 *)(void *)(based + (u32)countd * 0x11u);
      *(volatile u32 *)(void *)(recd + 0xd) = extra;
    }
    {
      u32 base3 = FIELD(self, 0x1c, u32);
      u32 newcount = FIELD(self, 0x24, u32) + 1;
      FIELD(self, 0x24, u32) = newcount;
      cm_fn_00945333(newcount, newcount, base3, newcount, 0x11, 0x00778e90);
    }
  }
}
