/* Reconstruction of preferred-image VA 0x00452a90 (module club_records.cpp).
   Pinned binary: CM0102 3.9.68 cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86, image base 0x00400000.
   Every statement is transcribed from the instructions listed at that base.

   x87 / __ftol representation (the held concern at 0x00452be4..0x00452c42):
   - 0x00452be4 and 0x00452c1d call 0x007c2150, whose result is delivered live in
      ST0.  It is declared `long double` here to represent that result.
   - 0x00452be9 calls 0x00945640 (MSVC __ftol) with that ST0 argument still live;
     the listing narrows the 64-bit EDX:EAX result with `mov bl, al`.  Per
     docs/agent/C.md that lowering is written as a `(long long)` conversion of a
     long double at the original call site, and the 64-bit result is then
     narrowed to the byte the listing actually uses (AL).  The ST0 value is dead
      after that conversion in the original.
      UNRESOLVED AUDIT GAP: this required plain-C lowering is not an explicit
      call to the original helper and is not proof of its FPU/control-word
      behavior. The listing establishes the call and AL use, not equivalence
      of compiler-emitted conversion instructions. Gate success is insufficient.
   - the second call's ST0 result (0x00452c1d) is kept as a `long double` local
     and stays live across the faultable F32 load at 0x00452c22, the FCOMP at
     0x00452c28 against the original qword constant at 0x00968948, the FCOM at
     0x00452c35 against the re-loaded F32 field, and the conditional FSTP
     (`fstp dword ptr [esi+0x1f1]` at 0x00452c42 / `fstp st(0)` at 0x00452c71).
     It is never converted to double/float in between; the single exact
     rounding to F32 happens only at that store.                                     */
#include "cmre.h"

#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b74350 CM_GLOBAL(u32, 0x00b74350)
/* qword operand of `fcomp qword ptr [0x968948]` at 0x00452c28.  Value unknown;
   read from the original image instead of inventing a literal. */
#define DAT_00968948 CM_GLOBAL(double, 0x00968948)

extern int CM_THISCALL cm_fn_004528d0(int param_1, int *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern long double CM_STDCALL cm_fn_007c2150(u32 stats, s8 kind, float incoming);
extern u32 CM_THISCALL cm_fn_007c3920(u32 self, u32 player, s8 kind);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
/* 0x0090d130 takes no arguments (its record is `char *f(void)`): the three
   pushes before its call (0x00452acd push 0x16f2, 0x00452ad2 push [esp+0x20],
   0x00452ad3 push [esp+0x120]) belong to the following 0x00944c9f, which is
   entered right after this call's `push eax` and which is cleaned together with
   them by the single `add esp, 0x24` at 0x00452af9 - C.md: pushes that A's
   (void) signature does not take belong to the next call.  The 0x00944c9f
   argument list is therefore (0xae24d0, 0x9870e8, this call's EAX,
   [esp+0x120], [esp+0x20], 0x16f2), pushed right to left. */
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_THISCALL cm_fn_00452a90(int param_1, int *param_2)
{
  /* one scratch allocation for the two 256-byte buffers the original keeps
     adjacent at [esp+0x20] (= L+0x000) and [esp+0x120] (= L+0x100) */
  u8 L_buf[0x200];
  u8 *local_200;                /* S-0x200 */
  u8 *local_100;                /* S-0x100 */
  u32 local_20c;                /* S-0x20c: 0/1 result */
  u8 *local_214;                /* S-0x214: cursor over the 50 dword slots */
  int local_210;                /* S-0x210: remaining slot count (0x32) */
  u32 rec;                      /* S-0x208: club record base */
  u32 p2;
  u32 v;
  u32 h;
  u32 obj;
  u32 stats;                    /* EDI; distinct from the record saved in ESI */
  u32 r;
  u8 b;
  long double f;

  p2 = (u32)param_2;
  local_200 = L_buf;
  local_100 = L_buf + 0x100;

  if (param_2 == (int *)0) {
    /* 0x00452aa6..0x00452af9 */
    cm_fn_00944cff((u32)0x0098b6a0, 0, 0, (void *)local_100, (void *)local_200);
    r = (u32)cm_fn_0090d130();
    /* pushes, right to left: 0x16f2, [esp+0x20], [esp+0x120], EAX, 0x9870e8,
       0xae24d0, then `call 0x00944c9f` at 0x00452ae4 */
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)0x009870e8, r,
                   (u32)local_100, (u32)local_200, 0x16f2);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    return 0;
  }

  /* 0x00452b10..0x00452b26: rec = [this+0x1c] + *param_2 * 0x6a
     (lea/shl/sub/lea chain is an unsigned 32-bit multiply by 106) */
  rec = (u32)*(u32 *)((u32)param_1 + 0x1c) + (u32)(*param_2) * 0x6a;
  if (*(s8 *)(rec + 0x10) == 0) {
    /* 0x00452b2d..0x00452b38 */
    return 0;
  }

  /* 0x00452b3b..0x00452b3d: the incoming ECX is forwarded as the receiver */
  obj = (u32)cm_fn_004528d0(param_1, param_2, 0);
  if (obj == 0) {
    /* 0x00452b48..0x00452b53 */
    return 0;
  }

  local_214 = (u8 *)(p2 + 0xd7);
  *(u32 *)(obj + 0x1f1) = 0xbf800000;
  local_20c = 0;
  local_210 = 0x32;
  do {
    v = *(u32 *)local_214;                    /* 0x00452b7b */
    if (v != 0) {
      h = *(u32 *)(v + 0x61);                 /* 0x00452b85 */
      if (h != 0) {
        if ((int)*(u32 *)v < DAT_00ae2364) {  /* 0x00452b90: signed jge */
          int hit;

          if (*(u32 *)(v + 0x39) == p2) {     /* 0x00452ba1 */
            hit = 1;
          }
          else {
            r = cm_fn_00540a50(p2, 0, 1);     /* 0x00452bab */
            /* 0x00452bb0 re-loads the field after the call */
            hit = (*(u32 *)(v + 0x39) == r);
          }
          if (hit) {
            /* 0x00452bbe: receiver [0xb74350] in ECX */
            /* 0x00452bcc saves EAX in EDI, not ESI (obj, 0x00452b42). */
            stats = cm_fn_007c3920(DAT_00b74350, h, 6);
            if (stats != 0) {
              /* 0x00452be4: ST0 = long double result */
              f = cm_fn_007c2150(stats, 1, 6.5f /* 0x40d00000 */);
              /* 0x00452be9 call 0x00945640 (__ftol); 0x00452bf2 mov bl, al.
                 The byte scratch at [esp+0x20] is read back as a dword and
                 masked with 0xff, i.e. only the low byte of the 64-bit
                 conversion result is used. */
              b = (u8)(long long)f;
              /* 0x00452bf8..0x00452c0d: signed byte field / 2 */
              if ((int)(u32)b >= (int)*(s8 *)(rec + 0x10) / 2) {
                /* 0x00452c1d: ST0 = long double result, kept live */
                f = cm_fn_007c2150(stats, 0x11, 6.5f /* 0x40d00000 */);
                /* 0x00452c22 fld dword [esi+0x1f1] / 0x00452c28 fcomp qword
                   [0x968948].  C3 (ah & 0x40) is set for equal *and*
                   unordered, so both take the store path; `m != m` carries the
                   unordered half of that test. */
                if (((double)*(float *)(obj + 0x1f1) == DAT_00968948) ||
                    ((double)*(float *)(obj + 0x1f1) !=
                     (double)*(float *)(obj + 0x1f1)) ||
                    /* 0x00452c35 fcom dword [esi+0x1f1]; skip unless f > m */
                    (f > (long double)*(float *)(obj + 0x1f1))) {
                  *(float *)(obj + 0x1f1) = (float)f;   /* 0x00452c42 fstp */
                  *(u8 *)(obj + 0x20d) = b;             /* 0x00452c4e */
                  cm_fn_007e4920(v, obj + 0x1f5);       /* 0x00452c56 */
                  *(u16 *)(obj + 0x20e) = 0;            /* 0x00452c5e */
                  local_20c = 1;                        /* 0x00452c67 */
                }
              }
            }
          }
        }
      }
    }
    local_214 = local_214 + 4;                /* 0x00452c82 */
    local_210 = local_210 + -1;               /* 0x00452c85 */
  } while (local_210 != 0);
  return local_20c;
}
