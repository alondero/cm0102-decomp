/* fn_0057fd60 -- CM0102 3.9.68 preferred-image VA 0x0057fd60, image base
   0x00400000, pinned binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   ABI evidence from the original instructions:
     * 0x0057fd85 `mov esi, ecx` -- the object arrives in ECX before any write
        to it, so the object is the first register input.
      * 0x0057fd96 `mov dx, [esi+0xb8]` and 0x0057fda4
        `add dx, [esi+0x40]` replace only the low half of incoming EDX;
        0x0057fdb5 `push edx` passes its preserved upper half to
        0x0057fdb7 `call 0x00549ef0`. No preceding instruction writes EDX.
        Use fastcall to expose that second register input explicitly while
        retaining ECX and the original two stack arguments (ret 8).
     * 0x0057ffc9 `ret 8` -- two 4-byte stack arguments, read at 0x0057fd7d
       ([esp+0x230] == esp0+4) and 0x0057fe26 / 0x0057ffa1 ([esp+0x23c]).
     * Result width: the 0x0057ff1f exit is `xor al, al`, i.e. only AL is
       defined (MSVC lowers `return false` for a 1-byte result that way); the
       0x0057ffa1 exit forwards cm_fn_006847c0's EAX unchanged.  Declared u8 so
       only the byte the original actually defines is compared.

   Heap/blob dispatch kept as dispatch:
     * 0x0057ff55/0x0057ff57/0x0057ff59 is `mov edx, [esi]`, `mov ecx, esi`,
       `call dword ptr [edx + 0x5c]` -- an indirect thiscall through the
       object's own vtable with no stack arguments.  It is reproduced as such
       (vtable pointer read from the object, slot 0x5c, receiver = object), so
       no callee identity, prototype or behaviour is assumed here; the concrete
       slot target (0x005806d0 physical in the accepted-constructor vtable at
       0x0096a0d4, per the hold note) stays a runtime boundary.
     * 0x0057ff2c/0x0057ff2e is `mov ecx, esi` then `call 0x0057ed10` after
       pushes of 6, 0 and the loop index.  The listing performs no stack
       cleanup, so the callee pops 12 bytes: thiscall with three stack
       arguments.  The navigation prototype `(int *, int)` only covers one of
       them, so the declaration below follows the instructions at the call
       site instead; 0x0057ed10 itself (and its slot 0x3c -> 0x0057d190
       dependency) remains an explicit unresolved boundary.
     * 0x0057fe43/0x0057fe7e/0x0057feba callees are the boundary calls their
       push sets describe.  No engine logic is invented. */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void CM_FASTCALL cm_fn_0057dab0(int param_1);
extern u32 CM_THISCALL cm_fn_0057e7e0(u32 self);
/* thiscall, three stack arguments, 12 bytes popped by the callee (see above) */
extern u32 CM_THISCALL cm_fn_0057ed10(void *self, int a, int b, int c);
extern u32 CM_THISCALL cm_fn_0057f600(u32 self);
extern void CM_THISCALL cm_fn_0057f920(void *self);
extern u32 CM_THISCALL cm_fn_0057fa10(u32 self);
extern u8 CM_STDCALL cm_fn_005ae6d0(u32 date, u32 id);
extern void cm_fn_005e8290(const char * param_1, const char * param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006827d0(int param_1, int param_2);
extern u32 CM_FASTCALL cm_fn_006835c0(int *param_1);
extern u32 CM_THISCALL cm_fn_006847c0(int *param_1, short *param_2, int param_3);
/* 0x0057fef1 `call 0x0090d130` with no argument of its own: the three pushes
   made at 0x0057fe73/0x0057fe78/0x0057fe79 (line, buffer, buffer) are
   deferred MSVC arguments of the following cdecl call; the single
   `add esp, 0x24` at 0x0057ff16 accounts for those three plus its own three
   and the three of 0x005e8290.  0x0090d130 itself reads no stack argument
   (its only traced data read is at 0x0090d135), so it stays the legacy
   `char *(void)` boundary. */
extern char * CM_CDECL cm_fn_0090d130(void);
/* destination, format, then four pushed varargs (message, buffer, buffer,
   line) from 0x0057fef6..0x0057ff01. */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

#define CM_DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define CM_DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define CM_DAT_00ae2a58 CM_GLOBAL(u32, 0x00ae2a58)
#define CM_STR_009c4674 ((const char *)0x009c4674)
#define CM_STR_009870e0 ((const char *)0x009870e0)
#define CM_STR_009870e8 ((const char *)0x009870e8)
/* SEH handler installed at 0x0057fd62 (push 0x9589a6) */
#define CM_SEH_HANDLER_009589a6 ((void *)0x009589a6)

u8 CM_FASTCALL cm_fn_0057fd60(u8 *self, u32 incoming_edx, u32 *param_2, int param_3)
{
  volatile struct {
    void *prev;
    void *handler;
    u32 state;
  } frame;
  u8 scratch[0x218];
  u32 *const copy1 = (u32 *)(void *)(scratch + 0x00);
  u32 *const copy0 = (u32 *)(void *)(scratch + 0x08);
  u32 *const res = (u32 *)(void *)(scratch + 0x10);
  u8 *const bufA = scratch + 0x18;
  u8 *const bufB = scratch + 0x118;
  u32 p2lo;
  u32 p2hi;
  u16 wordsum;
  u16 res_hi;
  u8 *tail_a;
  u8 *tail_b;
  u32 tail_line;
  u32 result;

  /* 0x0057fd60..0x0057fd6e: SEH registration (prev, handler, state=-1) */
  frame.prev = CM_FS(void *, 0);
  frame.handler = CM_SEH_HANDLER_009589a6;
  frame.state = 0xffffffffu;
  CM_FS(void *, 0) = (void *)&frame;

  /* 0x0057fd88 */
  p2lo = param_2[0];
  p2hi = param_2[1];
  copy0[0] = p2lo;                                   /* 0x0057fd8e [esp+0x18] */
  copy0[1] = p2hi;                                   /* 0x0057fd92 [esp+0x1c] */

  wordsum = *(u16 *)(self + 0xb8);                   /* 0x0057fd96 */
  {
    u16 ax_word = *(u16 *)(self + 0xb6);             /* 0x0057fd9d */
    wordsum = (u16)(wordsum + *(u16 *)(self + 0x40));/* 0x0057fda4 */

    frame.state = 0;                                 /* 0x0057fdae [esp+0x230] */
    (void)cm_fn_00549ef0(res,                          /* ecx = lea [esp+0x20] */
                         ((u32)p2lo & 0xffff0000u) | (u32)ax_word,
                         (incoming_edx & 0xffff0000u) | (u32)wordsum); /* 0x0057fdb7 */
  }

  /* 0x0057fdbc..0x0057fdcf */
  p2lo = param_2[0];
  p2hi = param_2[1];
  res_hi = *(u16 *)(scratch + 0x12);                 /* 0x0057fdc2 high word of res[0] */
  copy1[0] = p2lo;                                   /* 0x0057fdc7 [esp+0x10] */
  copy1[1] = p2hi;                                   /* 0x0057fdcb [esp+0x14] */

  /* 0x0057fdd4 `cmp cx, ax`, 0x0057fdd7 state byte = 1, 0x0057fddf `jb` */
  *(volatile u8 *)(&frame.state) = 1;
  if (*(u16 *)(scratch + 0x02) < res_hi) goto L_ff1f;
  if (*(u16 *)(scratch + 0x02) > res_hi) goto L_fdfb;
  /* 0x0057fdeb..0x0057fdf5: signed low-word compare, ecx == 0 required */
  {
    u32 less = ((s16)*(u16 *)(scratch + 0x00) < (s16)*(u16 *)(scratch + 0x10)) ? 1u : 0u;
    if (less != 0) goto L_ff1f;
  }

L_fdfb:
  /* 0x0057fdfb..0x0057fe17 */
  {
    u32 e0 = param_2[0];
    u32 e1 = param_2[1];
    int different;

    copy1[0] = e0;                                   /* 0x0057fe01 */
    different = (*(u16 *)(scratch + 0x02) != res_hi); /* 0x0057fe05 `cmp word[esp+0x12], ax` */
    copy1[1] = e1;                                   /* 0x0057fe0a */
    if (different) goto L_ff80;                      /* 0x0057fe0e */
    if ((u16)e0 != (u16)*(u32 *)(scratch + 0x10)) goto L_ff80; /* 0x0057fe14 `cmp cx, di` */
  }
  if (*(u32 *)(self + 0x45) != 0) goto L_ff80;       /* 0x0057fe1d */
  if (param_3 == 0) goto L_ff80;                     /* 0x0057fe26 */

  /* 0x0057fe33 */
  (void)cm_fn_0057f600((u32)self);
  cm_fn_0057dab0((int)(u32)self);                    /* 0x0057fe3c ecx = esi */
  if (cm_fn_0057e7e0((u32)self) == 0) {              /* 0x0057fe43 */
    /* 0x0057fe4c: arg4 = bufB (esp+0x128), arg5 = bufA (esp+0x28) */
    cm_fn_00944cff((u32)CM_STR_009c4674, 0, 0, bufB, bufA);
    /* 0x0057fe68: deferred cdecl arguments arg4 = bufB, arg5 = bufA, arg6 = 0x5a8 */
    tail_a = bufB;
    tail_b = bufA;
    tail_line = 0x5a8;
    goto L_fef1;
  }
  /* 0x0057fe7c */
  if (cm_fn_006835c0((int *)(void *)self) == 0) {
    /* 0x0057fe87: arg4 = bufA, arg5 = bufB */
    cm_fn_00944cff((u32)CM_STR_009c4674, 0, 0, bufA, bufB);
    /* 0x0057fea3: deferred cdecl arguments arg4 = bufA, arg5 = bufB, arg6 = 0x5af */
    tail_a = bufA;
    tail_b = bufB;
    tail_line = 0x5af;
    goto L_fef1;
  }
  /* 0x0057feb7 */
  if (cm_fn_006827d0((int)(u32)self, 0) == 0) {
    /* 0x0057fec3: arg4 = bufA, arg5 = bufB */
    cm_fn_00944cff((u32)CM_STR_009c4674, 0, 0, bufA, bufB);
    /* 0x0057fedf: deferred cdecl arguments arg4 = bufA, arg5 = bufB, arg6 = 0x5b6 */
    tail_a = bufA;
    tail_b = bufB;
    tail_line = 0x5b6;
    goto L_fef1;
  }

  /* 0x0057ff26: eight row resets */
  {
    int i = 0;
    do {
      (void)cm_fn_0057ed10(self, i, 0, 6);           /* 0x0057ff2e */
      i = i + 1;
    } while (i < 8);
  }
  (void)cm_fn_009452ca(*(u32 *)(self + 0xf7));       /* 0x0057ff40 */
  *(u32 *)(self + 0xf7) = 0;                          /* 0x0057ff4a */
  cm_fn_0057f920(self);                               /* 0x0057ff50 */
  {
    /* 0x0057ff55 `mov edx, [esi]`; 0x0057ff57 `mov ecx, esi`;
       0x0057ff59 `call dword ptr [edx + 0x5c]` -- vtable slot, thiscall,
       no stack arguments.  Slot target left as an opaque boundary. */
    u32 *vt = *(u32 **)(void *)self;
    ((u32 (CM_THISCALL *)(void *))vt[0x5c / 4])((void *)self);
  }
  (void)cm_fn_009452ca(*(u32 *)(self + 0xee));       /* 0x0057ff63 */
  *(u32 *)(self + 0xee) = 0;                          /* 0x0057ff6b */
  *(u32 *)(self + 0xf2) = 0;                          /* 0x0057ff71 */
  *(u32 *)(self + 0x45) = 1;                          /* 0x0057ff77 */
  goto L_ffa1;

L_fef1:
  /* 0x0057fef1: shared error reporting tail */
  {
    char *msg = cm_fn_0090d130();                     /* 0x0057fef1, no arguments */
    /* 0x0057ff01: dest, format, msg, buffer, buffer, line */
    cm_fn_00944c9f((u32)(void *)&CM_DAT_00ae24d0,
                   (u32)CM_STR_009870e8, (u32)msg,
                   (u32)(void *)tail_a, (u32)(void *)tail_b, tail_line);
    cm_fn_005e8290(CM_STR_009870e0,
                   (const char *)(void *)&CM_DAT_00ae24d0, 0);     /* 0x0057ff11 */
    CM_DAT_00b67a34 = 0;                                           /* 0x0057ff19 */
  }
  goto L_ff1f;

L_ff80:
  /* 0x0057ff80 */
  {
    u32 *holder = (u32 *)(void *)(*(u32 *)(self + 4));
    u32 arg2 = *holder;                                /* 0x0057ff87 `mov edx, [ecx]` */
    u8 taken;

    (void)CM_DAT_00ae2a58;                             /* 0x0057ff89 `mov ecx, [0xae2a58]` */
    taken = cm_fn_005ae6d0((u32)(void *)(scratch + 0x08), arg2); /* 0x0057ff91 */
    if (taken != 0) {                                  /* 0x0057ff96/0x0057ff98 */
      (void)cm_fn_0057fa10((u32)self);                 /* 0x0057ff9c */
    }
  }

L_ffa1:
  result = cm_fn_006847c0((int *)(void *)self, (short *)(void *)param_2, param_3);
  goto L_epilogue;

L_ff1f:
  result = 0;                                          /* 0x0057ff1f `xor al, al` */

L_epilogue:
  CM_FS(void *, 0) = frame.prev;                       /* 0x0057ffbc */
  return (u8)result;
}
