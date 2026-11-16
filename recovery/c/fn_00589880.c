/* fn_00589880 -- preferred-image VA 0x00589880, image base 0x00400000,
   CM0102 3.9.68, binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   Frame allocation / stack-probe helper 0x00945610.
   The original body opens with

     00589880  mov eax, 0x1760
     00589885  call 0x945610
     0058988a  mov eax, dword ptr [esp + 0x1768]

   which is MSVC's large-frame stack-probe lowering (__chkstk): EAX carries the
   frame size, the helper probes the pages above the relocated ESP, rewrites the
   return slot it uses as its probe base, then moves ESP by 0x1760.  The frame
   size is fixed independently by the epilogue `add esp, 0x1760` followed by
   `ret` (0x00589adc / 0x00589ae2, and the twin at 0x00589bac / 0x00589bb2),
   and by every root formal displacement below ([esp+0x1768] = arg2 after the
   probe, [esp+0x1778] with the five entry pushes = arg1, [esp+0x1788] with 7
   live pushed dwords = arg3, ...).

   The helper is a compiler-inserted construct, not an engine callee: its EAX
   count, its own probe reads and its ESP/return-slot relocation cannot be
   expressed through a C call interface (a C call would leave ESP unmoved and
   would place the count outside EAX), so it is represented by the plain C
   frame of exactly 0x1760 bytes below -- the same class of lowering as the
   `call 0x00945640` / __ftol case in docs/adr/0006.  The helper is therefore
   not omitted and not stubbed: the allocation, its extent and the probe
   transport are the frame itself, and every original frame-relative root
   formal access is kept at its original displacement beneath it. */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u8 *param_2, int param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char *CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

/* One 0x1760-byte allocation (the probed frame), with the touched sub-regions
   at their original offsets relative to the frame base F (ESP after the
   probe).  Bytes are left uninitialized exactly like the original. */
typedef struct cm_frame_00589880 {
  u32 v00;            /* F+0x00 */
  u32 v04;            /* F+0x04 */
  u32 v08;            /* F+0x08 */
  u32 v0c;            /* F+0x0c */
  u32 v10;            /* F+0x10 */
  u32 v14;            /* F+0x14 */
  u32 v18;            /* F+0x18 */
  u32 v1c;            /* F+0x1c */
  u8  b020[0x34];     /* F+0x20 .. F+0x53 */
  u8  b054[0x64];     /* F+0x54 .. F+0xb7 */
  u8  b0b8[0x190];    /* F+0xb8 .. F+0x247 */
  u8  b248[0x190];    /* F+0x248 .. F+0x3d7 */
  u8  b3d8[0x388];    /* F+0x3d8 .. F+0x75f */
  u8  bRest[0x1000];  /* untoued remainder of the 0x1760-byte frame */
} cm_frame_00589880;

/* cdecl (ret 0).  The only formals read are the dwords at E+4, E+8 and E+0xc
   (arg1 objects, arg2 dispatch, arg3 objects); E+0x10 is never read. */
void cm_fn_00589880(u8 *arg1, u32 arg2, u8 *arg3)
{
  cm_frame_00589880 L;
  u32 r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
  u32 pA, pB, edi, pbase;

  if (arg2 != 0) {
    /* 0x005898a8 .. 0x00589931: twelve thiscall reads of the arg1 object,
       indices 0..0xb, interleaved with the local stores at their original
       slots (the index-0 push at 0x00589895 precedes the dispatch test and
       serves the first call in both branches). */
    r0 = (u32)cm_fn_00785830(arg1, 0);          /* -> ebp */
    r1 = (u32)cm_fn_00785830(arg1, 1);
    L.v08 = r1;                                 /* 0x005898bc  [esp+0x1c] */
    r2 = (u32)cm_fn_00785830(arg1, 2);
    L.v04 = r2;                                 /* 0x005898c9  [esp+0x18] */
    r3 = (u32)cm_fn_00785830(arg1, 3);
    L.v0c = r3;                                 /* 0x005898d6  [esp+0x20] */
    r4 = (u32)cm_fn_00785830(arg1, 4);          /* -> edi */
    r5 = (u32)cm_fn_00785830(arg1, 5);
    L.v1c = r5;                                 /* 0x005898ee  [esp+0x30] */
    r6 = (u32)cm_fn_00785830(arg1, 6);
    L.v00 = r6;                                 /* 0x005898fb  [esp+0x14] */
    r7 = (u32)cm_fn_00785830(arg1, 7);
    L.v10 = r7;                                 /* 0x00589908  [esp+0x24] */
    r8 = (u32)cm_fn_00785830(arg1, 8);
    L.v18 = r8;                                 /* 0x00589915  [esp+0x2c] */
    r9 = (u32)cm_fn_00785830(arg1, 9);
    L.v14 = r9;                                 /* 0x00589922  [esp+0x28] */
    r10 = (u32)cm_fn_00785830(arg1, 10);        /* -> ebx */
    r11 = (u32)cm_fn_00785830(arg1, 11);

    /* 0x00589936 .. 0x0058995f: table bases, [0x00ae23b4] read once. */
    pA = CM_GLOBAL(u32, 0x00ae23b4);
    pB = pA + r10 * 581u;                       /* (k*9)<<4 +k, *4, +k  */
    pA = pA + r4 * 581u;

    /* 0x00589962: dest F+0x54, format 0x0098806c, one stack argument. */
    cm_fn_00944c9f((u32)L.b054, 0x98806cu, r11);

    /* 0x00589986 and 0x005899ae: args read at 0x00589967/0x0058996b/0x0058997f
       and 0x0058998b/0x0058999f/0x005899a3/0x005899a7 exactly as listed. */
    cm_fn_0053e140(L.v08, L.v04, L.v0c, (u32)L.b0b8, 0x190u, 1u, r0);
    pbase = L.v00;                              /* 0x0058998b  [esp+0x38] */
    cm_fn_0053e140(L.v10, L.v18, L.v14, (u32)L.b248, 0x190u, 1u, pbase);

    /* 0x005899bd .. 0x005899da: format chosen by the local at F+0x1c. */
    if (L.v1c == 1) {
      cm_fn_0066f4e0((u32)L.b020, 0x9c6880u, 0xad9c64u);
    } else {
      cm_fn_0066f4e0((u32)L.b020, 0x9c6844u, 0xad9c64u);
    }

    /* 0x005899e2 .. 0x00589a27: fourteen-push formatting call into F+0x3d8. */
    {
      s8 c1, c2, c3;

      c1 = *(s8 *)(pA + 0x52);                  /* 0x005899f1  movsx ecx */
      L.v00 = pA + 4;                           /* 0x005899fd  [F+0] */
      c2 = *(s8 *)(pB + 0x52);                  /* 0x00589a06  movsx eax */
      c3 = *(s8 *)(pA + 0x37);                  /* 0x00589a0a  movsx edx */
      cm_fn_0066f4e0((u32)L.b3d8, 0x9c66d4u,
                     (u32)c3, (u32)c3, (u32)c1, (u32)c1,
                     (u32)c2, (u32)c2,
                     (u32)L.b020, pA + 4u, (u32)L.b0b8, pA + 0x38u,
                     (u32)L.b054, pB + 0x38u);
    }

    /* 0x00589a2c: ECX = arg3 read before the frame-list cleanup; the call's
       stack argument is F+0x3d8.  The 64-bit result is discarded. */
    (void)cm_fn_004ae660(arg3, (u32)L.b3d8);

    /* 0x00589a47 .. 0x00589a6c: person index r0 through the original
       lea/shl chain (r0*5 -> *11 -> *55 -> *110) against [0x00ae23bc]. */
    {
      u32 t5 = r0 * 5u;
      u32 t11 = r0 + t5 * 2u;
      u32 t55 = t11 + t11 * 4u;
      u32 off = t55 * 2u;

      if (cm_fn_0053e990(CM_GLOBAL(u32, 0x00ae23bc) + off,
                         L.v08, L.v04, L.v0c, 0u) != 0) {
        /* 0x00589a78: fresh [0x00ae23bc] read for the parameter. */
        u32 v = CM_GLOBAL(u32, 0x00ae23bc) + off;
        edi = (u32)arg3;                        /* 0x00589a87 */
        cm_fn_004ae8a0((void *)edi, L.b0b8, 0x7da, v);
      } else {
        edi = (u32)arg3;                        /* 0x00589a9d */
      }

      /* 0x00589aa4, 0x00589ab9 (param_2 = [F+0] = pA+4), 0x00589ac7. */
      cm_fn_004ae8a0((void *)edi, (u8 *)(pA + 0x38), 0x7d5, pA);
      cm_fn_004ae8a0((void *)edi, (u8 *)L.v00, 0x7d5, pA);
      cm_fn_004ae8a0((void *)edi, (u8 *)(pB + 4), 0x7d5, pB);
    }
    return;
  }

  /* 0x00589ae3: arg2 == 0 branch (the pending index-0 push is still live). */
  edi = (u32)arg1;
  r0 = (u32)cm_fn_00785830(arg1, 0);            /* -> esi */
  r1 = (u32)cm_fn_00785830(arg1, 1);            /* -> ebx */
  r2 = (u32)cm_fn_00785830(arg1, 2);            /* -> ebp */
  r3 = (u32)cm_fn_00785830(arg1, 3);            /* -> edi */

  /* 0x00589b27: (r1, r2, r3, F+0xb8, 0x190, kind 5, person r0). */
  cm_fn_0053e140(r1, r2, r3, (u32)L.b0b8, 0x190u, 5u, r0);

  /* 0x00589b44: dest F+0x3d8, format 0x009c6694, source F+0xb8. */
  cm_fn_0066f4e0((u32)L.b3d8, 0x9c6694u, (u32)L.b0b8);

  /* 0x00589b49: ECX = arg3; stack argument F+0x3d8. */
  (void)cm_fn_004ae660(arg3, (u32)L.b3d8);

  /* 0x00589b60 .. 0x00589b7a: person index r0 against [0x00ae23bc]. */
  {
    u32 t5 = r0 * 5u;
    u32 t11 = r0 + t5 * 2u;
    u32 t55 = t11 + t11 * 4u;
    u32 off = t55 * 2u;

    if (cm_fn_0053e990(CM_GLOBAL(u32, 0x00ae23bc) + off,
                       r1, r2, r3, 0u) != 0) {
      /* 0x00589b86: fresh [0x00ae23bc] read; 0x00589b95: ECX = arg3. */
      u32 v = CM_GLOBAL(u32, 0x00ae23bc) + off;
      cm_fn_004ae8a0(arg3, L.b0b8, 0x7da, v);
    }
  }
}
