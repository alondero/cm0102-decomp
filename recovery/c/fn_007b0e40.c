/* fn_007b0e40 - player_search.cpp, preferred image VA 0x007b0e40, image base 0x00400000.
   Pinned binary: CM0102 3.9.68 cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

   Frame map. L = esp immediately after `sub esp,0x320` at 0x007b0e55 (the four saved
   registers pushed at 0x007b0e5b..0x007b0e5e occupy L-16..L-4). Every `lea/mov [esp+d]`
   below was reduced to an L-relative byte offset with the pushes that are live at that
   instruction (0x007b0fab `push esi` is live until 0x007b1011; 0x007b10b2 `push
   0x7bce50` is live until 0x007b1102; the two CM_THISCALL 0x00417c10 calls leave no
   residue: 0x007b1102 `add esp,0x10` plus each callee's own `ret 8` balance them):

     L+03  byte  loop index ([esp+0x13] at esp=L-16)
     L+04  dword cm_fn_00540a50 result ([esp+0x2c] at esp=L-40)
     L+08  .. L+34  45-byte comparator record fed to cm_fn_00946464 (count 0x2d)
     L+18  date struct initialised by cm_fn_0054a110 ([esp+0x28] at esp=L-16)
     L+20  date struct initialised by cm_fn_0054a110 ([esp+0x30] at esp=L-16)
     L+38  dword copy of [this+0x1fa] (write-only in this body)
     L+40, L+48, L+50  cm_fn_00417c10 operands (never written by this body)
     L+58  .. L+11f  cm_fn_00541f90 output (50 dwords, [esp+edx*4+0x68])
     L+120, L+220  cm_fn_00944cff/cm_fn_0090d130 error buffers
   The 45-byte comparator is built as: byte 2 at +0x00, (u32)0 at +0x01, (u32)0 at
   +0x05, byte 0 at +0x09, byte 0 at +0x0a, byte 0xff-or-[eax+0x113] at +0x0c.
   Comparator byte +0x0b (L+0x33) has no store anywhere in this body; the original
   reads that uninitialised stack byte at 0x007b10d6 (`mov dl, byte [esp+0x47]`, esp=L-20)
    and copies it to the stored row at +0x2b (0x007b10da).
    UNRESOLVED FIDELITY BLOCKER: leaving a new C allocation uninitialised does not
    bind it to that original preexisting stack byte. The supplied plain-C boundary
    provides neither the incoming original ESP nor its frame storage. Do not zero
    this byte or claim that passing the gate proves its provenance. Intervening
    callee write extents remain separate boundaries. */

#include "cmre.h"

#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00b7433c CM_GLOBAL(u32, 0x00b7433c)
#define LAB_007bce50 CM_GLOBAL(u8, 0x007bce50)

/* 0x009870e0 / 0x009870e8 / 0x00a701b0 are string literals in the image. */
#define s_009870e0 ((char *)0x009870e0)
#define s_009870e8 ((char *)0x009870e8)
#define s_00a701b0 ((char *)0x00a701b0)

/* Scalar field view of the `this` record ([edi+off]); volatile so that each original
   reload (0x007b0e7c/0x007b0e9?, 0x007b0ec2 reloads of +0x1ee/+0x1f6, ...) stays a
   distinct access, as the DWORD stores through the recovered pointer fields require. */
#define T32(off) (*(volatile u32 *)((u8 *)param_1 + (off)))

/* Scratch frame views, L-relative (see frame map). */
#define L8(off) (*(volatile u8 *)(L + (off)))
#define L32(off) (*(volatile u32 *)(L + (off)))

/* 0x00417c10 is CM_THISCALL with three declared parameters: the first travels in ECX
   (each call site sets it with `lea ecx,..` immediately before `call`), the other two
   are the pushes at 0x007b0f94/0x007b0f99, 0x007b1087/0x007b108b and
   0x007b109c/0x007b10a0 (right-to-left: param_3 pushed first). */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
/* 0x0054a110 is CM_FASTCALL: ECX carries the only pointer (`lea ecx,[esp+0x28]` at
   0x007b0e61, `lea ecx,[esp+0x30]` at 0x007b0e6c), no argument is pushed and nothing
   is cleaned up, so it pops 0 bytes. */
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_00541f90(u32 club, volatile u32 output, volatile u32 extended);
extern u32 CM_CDECL cm_fn_00542740(u32 club);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* 0x00795750: ECX = receiver loaded from [0x00b7433c] at 0x007b0fcd, then two pushes
   (0x007b0fab `push esi`, 0x007b0fd3 `push eax`): (self, date, key). */
extern u32 CM_THISCALL cm_fn_00795750(u32 self, u32 date, u32 key);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
/* 0x0090d130 takes no arguments: the three pushes at 0x007b1162/0x007b1167/0x007b1168
   (0x6e0, ecx, edx) precede its `call` but cannot be its arguments, and they are still
   live at 0x007b1179 - the single `add esp,0x24` at 0x007b118e covers them together
   with the 0x00944c9f and 0x005e8290 arguments. They are therefore the 4th..6th
   arguments of 0x00944c9f (right-to-left: pushed first = last parameter). */
extern char *cm_fn_0090d130(void);
/* 0x00944c9f: pushes in order 0x6e0, L+0x120, L+0x220, result of 0x0090d130
   (0x007b116e), 0x009870e8 (0x007b116f), 0x00ae24d0 (0x007b1174) ->
   (destination = 0x00ae24d0, format = 0x009870e8, then four varargs
   result, L+0x220, L+0x120, 0x6e0). */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
/* 0x00944cff: pushes L+0x120, L+0x220, 0, 0, 0x00a701b0 (0x007b1143..0x007b1147). */
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
/* 0x00945333: four stack arguments (0x007b10b2 0x007bce50, 0x007b10f3 0x2d,
   0x007b10f5 esi = new count, 0x007b10f6 ecx = [this+0x1ee]) plus the register inputs
   EAX (0x007b10f1 `mov eax,esi`) and EDX left by 0x007b10d6 `mov dl,[esp+0x47]`. */
extern unsigned long long CM_CDECL cm_fn_00945333(u32 eax_in, u32 edx_in,
                                                   volatile u32 base_arg,
                                                   volatile u32 count_arg,
                                                   volatile u32 width_arg,
                                                   volatile u32 compare_arg);
/* 0x00945667: pushes 0x00945333-style (edx = 0x2d * new capacity at 0x007b103a,
   eax = [this+0x1ee] at 0x007b103d) -> (base, bytes). */
extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
/* 0x00945934: `push -1` at 0x007b11a1 is the only argument; the function body ends
   immediately after this call (0x007b11a3 + 5 = 0x007b11a8 = end of the 872-byte
   body), so the original has no return path from here inside the recovered range. */
extern void cm_fn_00945934(u32 param_1);
/* 0x00946464: pushes 0x7bce50, 0x2d, [this+0x1f6], [this+0x1ee], L+8. */
extern u32 cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4,
                          void *param_5);

u32 CM_FASTCALL cm_fn_007b0e40(u32 *param_1)
{
  u8 L[0x320];                                /* `sub esp,0x320` at 0x007b0e55; uninitialised */
  volatile struct {
    void *prev;                               /* L+0x320, fs:[0] predecessor */
    void *handler;                            /* L+0x324 = 0x0095f436 */
    u32 state;                                /* L+0x328, [esp+0x338] */
  } frame;
  u32 uVar2;
  u32 uRet;                                   /* ebp: [0x00ae23bc + idx*0x6e + 0x39] */
  u32 uOut;                                   /* esi: cm_fn_00541f90 output entry */
  u32 uFlags;                                 /* [this+0x1fe] value in use */
  u32 uTmp;
  u32 uEdx;                                   /* EDX at the 0x007b10fd call site */
  u32 uCount;
  u32 uBase;
  u32 uByte;
  s32 iTmp;
  u8 *pRec;

  /* SEH registration, listing order 0x007b0e40..0x007b0e4e. */
  frame.state = 0xffffffffu;
  frame.handler = (void *)0x0095f436;
  frame.prev = CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)&frame;

  cm_fn_0054a110((u16 *)(L + 0x18));          /* 0x007b0e61 */
  frame.state = 0;                            /* 0x007b0e70 */
  cm_fn_0054a110((u16 *)(L + 0x20));          /* 0x007b0e77 */
  uFlags = T32(0x1fe);                        /* 0x007b0e7c, while SEH state is 0 */
  frame.state = 1;                            /* 0x007b0e8a */

  if ((uFlags & 0x4000000u) != 0) {           /* 0x007b0e84..0x007b0e9b */
    uVar2 = 0;
    goto done;
  }
  uRet = *(u32 *)(DAT_00ae23bc + T32(0x00) * 0x6e + 0x39);   /* 0x007b0ea0..0x007b0eb0 */
  if (uRet == 0) {                            /* 0x007b0eb4 */
    uVar2 = 0;
    goto done;
  }
  uTmp = cm_fn_00540a50(uRet, 0, 1);          /* 0x007b0ebf..0x007b0ec3 */
  L32(0x04) = uTmp;                           /* 0x007b0ecf: [esp+0x2c] at esp=L-40 */
  cm_fn_00541f90(uRet, (u32)(L + 0x58), 0);   /* 0x007b0ec8..0x007b0ed3 */
  L8(0x03) = 0;                               /* 0x007b0edb..0x007b0edd */

  do {
    uOut = L32(0x58 + (s32)(s8)L8(0x03) * 4); /* 0x007b0ee1..0x007b0ee4 */
    if (uOut != 0) {                          /* 0x007b0ee8 */
      iTmp = (s32)cm_fn_00542740(uRet);       /* 0x007b0ef0..0x007b0ef9 */
      if (iTmp != 0) {
        L32(0x38) = T32(0x1fa);               /* 0x007b0efd..0x007b0f05 */
        uFlags = T32(0x1fe);                  /* 0x007b0f09 */
        if ((uFlags & 0x10000000u) != 0) {    /* 0x007b0f0f..0x007b0f19 */
          if ((*(u8 *)(uOut + 0x5e) & 2) != 0) goto next;   /* 0x007b0f1b..0x007b0f1f */
        }
        if ((uFlags & 0x20000000u) != 0) {    /* 0x007b0f25..0x007b0f2e */
          if ((*(u8 *)(uOut + 0x5e) & 1) != 0) goto next;   /* 0x007b0f30..0x007b0f36 */
        }
      }
      else {                                  /* 0x007b0f3b */
        if (L32(0x04) != 0) {
          uFlags = T32(0x1fe);                /* 0x007b0f41 */
          L32(0x38) = T32(0x1fa);             /* 0x007b0f47..0x007b0f4f */
          if ((uFlags & 0x10000000u) != 0) {  /* 0x007b0f53..0x007b0f5d */
            if (*(u32 *)(uOut + 0x39) == L32(0x04)) goto next; /* 0x007b0f5f..0x007b0f68 */
          }
          if ((uFlags & 0x20000000u) != 0) {  /* 0x007b0f6e..0x007b0f77 */
            if (*(u32 *)(uOut + 0x39) == uRet) goto next;      /* 0x007b0f79..0x007b0f7c */
          }
        }
      }
      /* 0x007b0f82..0x007b0f8d: cm_fn_007e4920(entry, L+8) fills the comparator prefix
         at L+8. Its write extent belongs to the callee; this body only re-reads the
         first 16 bytes of that prefix at 0x007b106f..0x007b108f, and the date fields
         below are initialised separately. */
      cm_fn_007e4920(uOut, (u32)(L + 8));
      /* 0x007b0f90..0x007b0f9e: ECX = L+0x20, param_3 = 0x00ae2c90, param_2 = L+0x50. */
      cm_fn_00417c10((u32 *)(L + 0x20), (u32 *)(L + 0x50), (u16 *)0x00ae2c90);
      L8(0x28) = 2;                           /* 0x007b0fac */
      L32(0x29) = 0;                          /* 0x007b0fb4 */
      L32(0x2d) = 0;                          /* 0x007b0fb8 */
      L8(0x31) = 0;                           /* 0x007b0fbc */
      L8(0x32) = 0;                           /* 0x007b0fc3 */
      /* 0x007b0fcd..0x007b0fd4: ECX = [0x00b7433c], pushes record pointer then uOut. */
      uTmp = cm_fn_00795750(DAT_00b7433c,
                            DAT_00ae23bc + T32(0x00) * 0x6e,
                            uOut);
      if (uTmp != 0) {                        /* 0x007b0fd9..0x007b0fe7 */
        L8(0x34) = *(u8 *)(uTmp + 0x113);
      }
      else {
        L8(0x34) = 0xff;                      /* 0x007b0fe9 */
      }
      uCount = T32(0x1f6);                    /* 0x007b0fee */
      if (uCount != 0) {
        /* 0x007b0ff8..0x007b100c: param_1 = L+8, param_2 = [this+0x1ee],
           param_3 = uCount, param_4 = 0x2d, param_5 = &0x007bce50. */
        if (cm_fn_00946464((u32)(L + 8), T32(0x1ee), uCount, 0x2d,
                           (void *)&LAB_007bce50) != 0) goto next;   /* 0x007b1014 */
      }
      if (T32(0x1f6) == T32(0x1f2)) {         /* 0x007b101c..0x007b102a */
        uTmp = T32(0x1f2) + 0xfa;             /* 0x007b102c */
        T32(0x1f2) = uTmp;                    /* 0x007b1031 */
        uTmp = (u32)cm_fn_00945667(T32(0x1ee), uTmp * 0x2d);   /* 0x007b1037..0x007b1045 */
        T32(0x1ee) = uTmp;                    /* 0x007b104f */
        if (uTmp == 0) {                      /* 0x007b1055 -> 0x007b1135 */
          cm_fn_00944cff((u32)s_00a701b0, 0, 0, (void *)(L + 0x220),
                         (void *)(L + 0x120));                    /* 0x007b1135..0x007b1151 */
          uTmp = (u32)cm_fn_0090d130();                           /* 0x007b1154..0x007b1169 */
          cm_fn_00944c9f((u32)(&DAT_00ae24d0), (u32)s_009870e8,
                         uTmp, (u32)(L + 0x220), (u32)(L + 0x120),
                         0x6e0);                                 /* 0x007b116e..0x007b1179 */
          cm_fn_005e8290((const char *)s_009870e0,
                         (const char *)&DAT_00ae24d0, 0);         /* 0x007b117e..0x007b1189 */
          DAT_00b67a34 = 0;                                       /* 0x007b1191 */
          cm_fn_005cc7d0();                                       /* 0x007b1197 */
          cm_fn_005a8f60();                                       /* 0x007b119c */
          cm_fn_00945934(0xffffffffu);                            /* 0x007b11a1..0x007b11a3 */
          /* UNRESOLVED FIDELITY BLOCKER: CALL at preferred-image VA 0x007b11a3
             returns to 0x007b11a8, not 0x007b11b0. The supplied pinned listing
             ends before that continuation and does not prove nonreturn.
             The inherited return below is NOT faithful: it fabricates EAX=0
             and leaves fs:[0] linked to this C frame. Neither goto done,
             a noreturn annotation, a trap nor a loop is an evidenced repair.
             Requires original continuation/nonreturn evidence and, if returning,
             an authorized original-frame/continuation binding. */
          return 0;
        }
      }
      /* 0x007b105b..0x007b106d: esi = [this+0x1f6] * 0x2d + [this+0x1ee]. */
      pRec = (u8 *)(T32(0x1f6) * 0x2d + T32(0x1ee));
      *(u32 *)(pRec + 0x00) = L32(0x08);      /* 0x007b106f..0x007b1073 */
      *(u32 *)(pRec + 0x04) = L32(0x0c);      /* 0x007b1075..0x007b1079 */
      *(u32 *)(pRec + 0x08) = L32(0x10);      /* 0x007b107c..0x007b1080 */
      *(u32 *)(pRec + 0x0c) = L32(0x14);      /* 0x007b1083..0x007b108f */
      cm_fn_00417c10((u32 *)(pRec + 0x10), (u32 *)(L + 0x40),
                     (u16 *)(L + 0x18));      /* 0x007b1087..0x007b1097 */
      cm_fn_00417c10((u32 *)(pRec + 0x18), (u32 *)(L + 0x48),
                     (u16 *)(L + 0x20));      /* 0x007b109c..0x007b10a9 */
      pRec[0x20] = L8(0x28);                  /* 0x007b10ae: ESP=L-0x10, +0x38 */
      uEdx = L32(0x29);                       /* 0x007b10ba */
      *(u32 *)(pRec + 0x21) = uEdx;           /* 0x007b10be */
      uEdx = L32(0x2d);                       /* 0x007b10c1 */
      *(u32 *)(pRec + 0x25) = uEdx;           /* 0x007b10c5 */
      pRec[0x29] = L8(0x31);                  /* 0x007b10c8..0x007b10cc */
      pRec[0x2a] = L8(0x32);                  /* 0x007b10cf..0x007b10d3 */
      uByte = L8(0x33);                       /* 0x007b10d6: never initialised by 0x7b0e40 */
      pRec[0x2b] = (u8)uByte;                 /* 0x007b10da */
      pRec[0x2c] = L8(0x34);                  /* 0x007b10dd..0x007b10e1 */
      uEdx = (uEdx & 0xffffff00u) | uByte;    /* EDX: 0x007b10c1 load, DL from 0x007b10d6 */
      uCount = T32(0x1f6);                    /* 0x007b10e4 */
      uBase = T32(0x1ee);                     /* 0x007b10ea */
      uCount = uCount + 1;                    /* 0x007b10f0 */
      T32(0x1f6) = uCount;                    /* 0x007b10f7 */
      cm_fn_00945333(uCount, uEdx, uBase, uCount, 0x2d,
                     (u32)(&LAB_007bce50));   /* 0x007b10f3..0x007b10fd */
    }
next:
    L8(0x03) = (u8)(L8(0x03) + 1);            /* 0x007b1105..0x007b110d */
  } while ((s8)L8(0x03) < 0x32);              /* 0x007b1111 */
  uVar2 = 1;                                  /* 0x007b1117 */

done:
  CM_FS(void *, 0) = frame.prev;              /* 0x007b111c..0x007b112e */
  return uVar2;
}
