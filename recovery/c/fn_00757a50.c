/* fn_00757a50 -- module media.cpp, preferred-image VA 0x00757a50, image base 0x00400000.
   Pinned CM0102 3.9.68 binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Reconstructed from the original instructions only.  `ret 0x14` pops five stack
   dwords and ECX is read before it is written, so this is a thiscall member with
   five stack arguments.  Frame: `sub esp,0x208` then `push esi`; with E = esp
   after the prologue the scratch block is [E+4,E+0x20c): an 8-byte local at
   [E+4], a 0x100-byte buffer at [E+0xc] and a second 0x100-byte buffer at
   [E+0x10c].  Stack arguments are at [E+0x210]..[E+0x220].  The two pushes for
   0x00417c10 (which returns with its two stack bytes already popped, so ESP is E
   again at 0x00757bfa) shift the later argument reads: [esp+0x218] at
   0x00757bfa is argument 3, and [esp+0x224]/[esp+0x228] at 0x00757c19/0x00757c3e
   are arguments 4 and 5 (ESP = E-8 there). */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define LAB_00757ee0 CM_GLOBAL(u8, 0x00757ee0)

/* Callees stay explicit boundaries. */
extern void CM_THISCALL cm_fn_00417c10(u32 *self, void *arg1, void *arg2);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern void CM_CDECL cm_fn_00945333(u32 arg1, u32 arg2, u32 arg3, u32 arg4);
extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
extern void cm_fn_00945934(u32 param_1);

void CM_THISCALL cm_fn_00757a50(u32 *self, u32 *arg1, u32 *arg2, u32 arg3,
                                s8 arg4, u8 arg5)
{
  u8 frame[0x208];          /* [E+4,E+0x20c) */
  u8 *local8 = frame;       /* [E+4]    : 8 bytes    */
  u8 *buf0 = frame + 8;     /* [E+0xc]  : 0x100 bytes */
  u8 *buf1 = frame + 0x108; /* [E+0x10c]: 0x100 bytes */
  void *mem;
  u32 uVar2;
  u32 base;
  u32 idx;
  u32 val;

  if (self[0] == 0) {
    mem = (void *)(u32)cm_fn_00944e46(0x1cc);
    self[0] = (u32)mem;
    if (mem == 0) {
      /* 0x00757a8b: five pushes, caller cleanup `add esp,0x14` at 0x00757a90.
         arg4 = [esp+0x10c] (buf1), arg5 = [esp+0xc] (buf0). */
      cm_fn_00944cff(0x00a17becu, 0, 0, buf1, buf0);
      /* 0x00757a93..0x00757aa5: pushes 0x6e11, buf0, buf1 (top = buf1), so
         0x0090d130 gets (buf1, buf0, 0x6e11).  Those three pushes are cleaned
         only by `add esp,0x24` at 0x00757acb, i.e. not before 0x00944c9f, so
         the variadic slots of the format at 0x009870e8 after its first
         argument are buf1, buf0 and 0x6e11. */
      uVar2 = (u32)cm_fn_0090d130((u32)buf1, (u32)buf0, 0x6e11);
      /* 0x00757aaa..0x00757ab5: pushes eax (0x0090d130 result), 0x009870e8,
         0x00ae24d0 -> (destination, format, value, buf1, buf0, 0x6e11). */
      cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, uVar2, (u32)buf1, (u32)buf0,
                     0x6e11u);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
      DAT_00b67a34 = 0;
      cm_fn_005cc7d0();
      cm_fn_005a8f60();
      /* 0x00757ae2 push -1 / 0x00757ae4 call 0x00945934.  The callee is a plain
         cdecl boundary, not declared noreturn and given no invented cleanup, so
         a returning callee keeps the original returning stack effect.  The
         listing falls through to 0x00757ae9, which the code below does. */
      cm_fn_00945934(0xffffffffu);
    }
    self[1] = 0x14;
  }
  if (self[1] == self[2]) {
    /* 0x00757afe..0x00757b0d: realloc(data, (capacity+0x14)*0x17). */
    mem = (void *)(u32)cm_fn_00945667(self[0], (self[1] + 0x14) * 0x17);
    self[0] = (u32)mem;
    if (mem == 0) {
      /* 0x00757b31: arg4 = [esp+0xc] (buf0), arg5 = [esp+0x10c] (buf1). */
      cm_fn_00944cff(0x00a17becu, 0, 0, buf0, buf1);
      /* 0x00757b39..0x00757b4b: pushes 0x6e1c, buf1, buf0 (top = buf0), so
         0x0090d130 gets (buf0, buf1, 0x6e1c); cleaned by `add esp,0x24` at
         0x00757b71, so those slots remain for 0x00944c9f's variadic reads. */
      uVar2 = (u32)cm_fn_0090d130((u32)buf0, (u32)buf1, 0x6e1c);
      /* 0x00757b50..0x00757b5b: (destination, format, value, buf0, buf1, 0x6e1c). */
      cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, uVar2, (u32)buf0, (u32)buf1,
                     0x6e1cu);
      cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
      DAT_00b67a34 = 0;
      cm_fn_005cc7d0();
      cm_fn_005a8f60();
      /* 0x00757b88 push -1 / 0x00757b8a call 0x00945934; same contract. */
      cm_fn_00945934(0xffffffffu);
    }
    self[1] = self[1] + 0x14;
  }

  /* 0x00757b93..0x00757bb2. */
  if (arg1 != 0) {
    val = *arg1;
  }
  else {
    val = 0xffffffffu;
  }
  base = self[0];
  idx = self[2];
  *(u32 *)(base + idx * 0x17 + 8) = val;

  /* 0x00757bb6..0x00757bda. */
  if (arg2 != 0) {
    val = *arg2;
  }
  else {
    val = 0xffffffffu;
  }
  base = self[0];
  idx = self[2];
  *(u32 *)(base + idx * 0x17 + 0xc) = val;

  /* 0x00757bde..0x00757bf2: ECX = data + idx*0x17 receiver; the two stack
     arguments are [esp+8] (the 8-byte local) and 0x00ae2c90. */
  base = self[0];
  idx = self[2];
  cm_fn_00417c10((u32 *)(base + idx * 0x17), local8, (void *)&DAT_00ae2c90);

  /* 0x00757bf7..0x00757c12.  The two pushes for 0x00945333 (0x00757ee0 and
     0x17) are issued before this store. */
  base = self[0];
  idx = self[2];
  *(u32 *)(base + idx * 0x17 + 0x13) = arg3;

  /* 0x00757c19: `movsx dx, byte ptr [esp+0x224]` (ESP = E-8 -> argument 4),
     then the sign-extended word store. */
  base = self[0];
  idx = self[2];
  *(u16 *)(base + idx * 0x17 + 0x11) = (u16)(s16)arg4;

  /* 0x00757c3e: `mov al, byte ptr [esp+0x228]` (ESP = E-8 -> argument 5). */
  base = self[0];
  idx = self[2];
  *(u8 *)(base + idx * 0x17 + 0x10) = arg5;

  /* 0x00757c49..0x00757c56: count is bumped and stored before the call; the
     data pointer is read before that store. */
  base = self[0];
  idx = self[2] + 1;
  self[2] = idx;
  cm_fn_00945333(base, idx, 0x17, (u32)&LAB_00757ee0);
}
