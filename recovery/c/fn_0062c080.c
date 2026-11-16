/* fn_0062c080 - recovered from original instructions.
   Preferred-image VA 0x0062c080, image base 0x00400000.
   Pinned binary: CM0102 3.9.68, sha256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ret 8 -> two stack arguments; ECX read before written -> this (thiscall).

   Frame (0062c080 sub esp,0x20; 4 saved registers pushed):
     the original's 0x20-byte scratch is [esp+0x10..esp+0x2f] after the pushes.
     Slot offsets below are relative to L, immediately after sub esp,0x20.
     FIDELITY HOLD: the automatic array does not bind original incoming stack
     bytes. At 0x0062c23d a first score <=50 leaves L+4 stale; at 0x0062c254
     only L+0xc is written before the DWORD reads at 0x0062c273/0x0062c27f.
     A supported original-stack input boundary is required for those paths;
     neither zero initialization nor compiler frame placement is a repair. */
#include "cmre.h"

#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00ae2c94 CM_GLOBAL(u32, 0x00ae2c94)

/* 0x0062c1be: push [0x00ae2c94]; push [0x00ae2c90]; ecx = 0x00dd6ec8 */
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
/* 0x0062c1d6: push edi; push dx([edi+6]); push esi; ecx = ebp */
extern u32 CM_THISCALL cm_fn_0062bd10(u32 self, u32 record, volatile u32 field,
                                     volatile u32 increment);
/* 0x0062c24d: push esi; push [esp+0x14]; push ecx; ecx = ebp */
extern u8 CM_THISCALL cm_fn_0062d140(u32 self, volatile u8 type,
                                    volatile u8 category, u32 person);
/* 0x0062c28a: 8 stack pushes, ecx = ebp */
extern int CM_THISCALL cm_fn_0062d250(int *param_1, u32 param_2, int param_3,
                                      int *param_4, int param_5, int param_6,
                                      u32 param_7, char param_8, int param_9);
/* 0x0062c25f: push 0; push esi; ecx = ebp */
extern u32 CM_THISCALL cm_fn_0062e700(u32 self, u32 person, u32 flag);
/* 0x0062c1fa / 0x0062c292: ecx = [ebp+0x28] */
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
/* 0x0062c164 push 3 / 0x0062c193 push 5, one cdecl dword, caller "add esp,4";
   the original consumes only EAX of the result. */
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);

void CM_THISCALL cm_fn_0062c080(u32 self, u8 *param_2, u8 *param_3)
{
  /* Scratch adjacency only: untouched bytes here are NOT established to equal
     original stack bytes. See the fidelity hold above. */
  u8 L[0x20];
  u8 *rec;   /* edx at 0x0062c0a9: current record, stride 0x49 */
  u8 idx;    /* cl */
  u8 bl;
  s32 iv2;
  u32 edi_v;

  /* 0062c08d */
  *(volatile s32 *)(L + 0x14) = 2;
  /* 0062c095: edx = [edi+0x1c0] */
  rec = param_3 + 0x1c0;
  /* 0062c09b */
  *(volatile u32 *)(L + 0x10) = (u32)rec;

  for (;;) {
    /* 0062c09f */
    idx = 0;
    /* 0062c0a1 */
    *(volatile u32 *)(L + 0x08) = (u32)rec;
    /* 0062c0a5: esp=L-0x10, so [esp+0x13] is L+3. */
    *(volatile u8 *)(L + 0x03) = idx;

    for (;;) {
      u32 pr;
      u8 *recdi;
      u32 fld;

      /* 0062c0a9 */
      iv2 = *(volatile s32 *)(rec - 0x30);
      /* 0062c0af */
      *(volatile s32 *)(L + 0x1c) = iv2;
      if (iv2 == -1) goto l_2ab;                      /* 0062c0b3 */

      /* 0062c0b9..0062c0c8: esi = [0x00ae23bc] + iv2*0x6e, 32-bit wrapping */
      pr = (u32)DAT_00ae23bc + (u32)iv2 * 0x6eu;
      if (pr != 0) {                                  /* 0062c0cb */
        /* 0062c0cf..0062c0d9 */
        if (*(volatile s32 *)pr >= DAT_00ae2364) goto l_2ab;
      }

      /* 0062c0df: EDI is the second stack argument here */
      if (*(volatile u8 *)(param_3 + 0x4c) != 0) {
        if ((s8)idx >= 0xb) goto l_108;               /* 0062c0e6 */
        bl = 0x5a;                                    /* 0062c0eb */
        goto l_127;
      }
      /* 0062c0ef */
      if ((s8)idx >= 0xb) goto l_108;
      bl = *(volatile u8 *)(rec + 4);                 /* 0062c0f4 */
      if ((s8)bl > -1) goto l_127;                    /* 0062c0f7 */
      bl = *(volatile u8 *)(rec + 0);                 /* 0062c0fc */
      if ((s8)bl > -1) goto l_127;                    /* 0062c0fe */
      bl = *(volatile u8 *)(param_3 + 0x4d);          /* 0062c103 */
      goto l_127;

l_108:
      {
        u8 a2 = *(volatile u8 *)(rec + 2);            /* 0062c108 */
        if (a2 == 0xff) {                             /* 0062c10b */
          bl = 0;                                     /* 0062c10f */
          goto l_127;
        }
        bl = *(volatile u8 *)(rec + 4);               /* 0062c113 */
        if ((s8)bl > -1) goto l_125;                  /* 0062c116 */
        bl = *(volatile u8 *)(rec + 0);               /* 0062c11b */
        if ((s8)bl > -1) goto l_125;                  /* 0062c11d */
        bl = *(volatile u8 *)(param_3 + 0x4d);        /* 0062c122 */
l_125:
        bl = (u8)(bl - a2);                           /* 0062c125 */
      }
l_127:
      {
        s32 diff, hi, q;
        u32 cx;

        /* 0062c127 */
        diff = 10000 - (s32)*(volatile s16 *)(rec + 6);
        /* 0062c130..0062c149: imul 0x51eb851f / sar edx,5 / sign fixup */
        hi = (s32)(((long long)0x51eb851fLL * (long long)diff) >> 32);
        q = hi >> 5;
        q = (s32)((u32)q + ((u32)q >> 31));
        /* 0062c13e: movsx cx,bl rewrites only CX; ECX high half came from diff */
        cx = ((u32)diff & 0xFFFF0000u) | (u32)(u16)(s16)(s8)bl;
        /* 0062c14e */
        edi_v = (u32)q + cx;
      }

      /* 0062c14b */
      if (*(volatile u8 *)(param_2 + 0x40) == 0) {
        /* 0062c156: movsx eax,di; cdq; sub; sar 1 -> (signed short) / 2 */
        edi_v = (u32)((s32)(s16)edi_v / 2);
      }
      else {
        u32 r3 = (u32)cm_fn_0090cfc0(3);              /* 0062c162 */
        s32 b18 = (s32)*(volatile s8 *)(pr + 0x18);   /* 0062c169 */
        r3 = r3 + 0x21;                               /* 0062c16d */
        if (b18 > (s32)r3) {                          /* 0062c173 */
          /* 0062c177: movsx eax,di; [esp+0x28]=eax; fild dword [esp+0x28];
             fmul qword [0x00968928]; call 0x00945640 (__ftol); edi = eax */
          *(volatile s32 *)(L + 0x18) = (s32)(s16)edi_v;
          {
            long double t = (long double)*(volatile s32 *)(L + 0x18)
                          * CM_GLOBAL(double, 0x00968928);
             /* Original helper, same pinned binary as the file header:
                preferred-image VAs 0x00945647 saves CW, 0x0094564f sets
                chop rounding, 0x00945659 FISTP qword consumes ST0,
                0x0094565c restores CW, 0x0094565f/62 return EDX:EAX.
                ADR 0006 permits this long long lowering of call 0x0062c188;
                it is not an ordinary C call with a fabricated stack input.
                Emission/sensitivity validation remains outstanding. */
             edi_v = (u32)(s32)(long long)t;
          }
        }
        else {
          u32 r5 = (u32)cm_fn_0090cfc0(5);            /* 0062c191 */
          s32 b18b = (s32)*(volatile s8 *)(pr + 0x18);/* 0062c198 */
          s32 lim = 0x17 - (s32)r5;                   /* 0062c19c..0062c1a4 */
          if (b18b < lim) edi_v = edi_v + edi_v;      /* 0062c1a6 / 0062c1aa */
        }
      }

      {
        /* 0062c1ac: reads 0x00ae2c94 first, then 0x00ae2c90 */
        u32 g94 = DAT_00ae2c94;
        u32 g90 = DAT_00ae2c90;
        if (cm_fn_00456740(0x00dd6ec8u, g90, g94) != 0) edi_v = 0;  /* 0062c1c7 */
      }

      /* 0062c1ca: edi = [esp+0x1c] (the record mirror) */
      recdi = (u8 *)*(volatile u32 *)(L + 0x08);
      fld = (u32)*(volatile u16 *)(recdi + 6);        /* 0062c1d0 */
      cm_fn_0062bd10(self, pr, fld, edi_v);           /* 0062c1d6 */

      /* 0062c1db */
      if (*(volatile u8 *)(recdi - 0x11) != 0) {
        u32 head = *(volatile u32 *)(self + 0x28);    /* 0062c1e6 */
        if (head != 0) {                              /* 0062c1e9 */
          u32 t2 = *(volatile u32 *)(head + 0x02);    /* 0062c1f1 */
          *(volatile u32 *)(head + 0x0a) = t2;        /* 0062c1f4 */
          {
            u32 item = cm_fn_0068d1b0(*(volatile u32 *)(self + 0x28));
            while (item != 0) {
              /* 0062c207: reload the record id every pass */
              if (*(volatile s32 *)item == *(volatile s32 *)(L + 0x1c)) {
                u8 typ = *(volatile u8 *)(item + 4);  /* 0062c211 */
                if (!(typ == 7 &&                     /* 0062c214 */
                      *(volatile s8 *)(*(volatile u32 *)(pr + 0x61) + 0x0f)
                        < 0xf)) {                     /* 0062c219 */
                  u8 al5 = *(volatile u8 *)(item + 5);          /* 0062c222 */
                  if ((s8)al5 > 0x5f) *(volatile u8 *)(L + 0x04) = 2;
                  else if ((s8)al5 > 0x41) *(volatile u8 *)(L + 0x04) = 1;
                  else if ((s8)al5 > 0x32) *(volatile u8 *)(L + 0x04) = 0;
                  {
                    /* 0062c244: DWORD read across the category byte at +4 and
                       the three bytes the original never initializes. */
                    u32 cat = *(volatile u32 *)(L + 0x04);
                    u8 r = cm_fn_0062d140(self, typ, (u8)cat, pr); /* 0062c24d */
                    /* 0062c252 / 0062c254: BYTE store of the result */
                    *(volatile u8 *)(L + 0x0c) = r;
                    if ((s8)r >= 0) {
                      u32 p2v;
                      u32 v;
                      cm_fn_0062e700(self, pr, 0);            /* 0062c25f */
                      p2v = *(volatile u32 *)(param_2);       /* 0062c264 */
                      /* 0062c273 / 0062c27f: DWORD read of the same slot the
                         byte above only partly wrote. */
                      v = *(volatile u32 *)(L + 0x0c);
                      if (p2v == 0xfffffffeu)                 /* 0062c26a */
                        cm_fn_0062d250((int *)self, v, 0, (int *)pr, 0,
                                       1, 0xffffffffu, 0, 0);
                      else
                        cm_fn_0062d250((int *)self, v, 0, (int *)pr,
                                       (int)(u32)param_2, 1, 0xffffffffu, 0, 0);
                    }
                  }
                }
              }
              /* 0062c28f / 0062c292: receiver reloaded from [ebp+0x28] */
              item = cm_fn_0068d1b0(*(volatile u32 *)(self + 0x28));
            }
          }
        }
      }

      /* 0062c29f: cl and edx restored from the scratch mirrors */
      idx = *(volatile u8 *)(L + 0x03);
      rec = (u8 *)*(volatile u32 *)(L + 0x08);
l_2ab:
      idx = (u8)(idx + 1);                            /* 0062c2ab */
      rec = rec + 0x49;                               /* 0062c2ad */
      *(volatile u8 *)(L + 0x03) = idx;               /* 0062c2b3 */
      *(volatile u32 *)(L + 0x08) = (u32)rec;         /* 0062c2b7 */
      if ((s8)idx >= 0x14) break;                     /* 0062c2bb */
    }

    /* 0062c2c1 */
    {
      u32 outerp = *(volatile u32 *)(L + 0x10);
      s32 cnt = *(volatile s32 *)(L + 0x14);
      outerp = outerp + 0x625;                        /* 0062c2c9 */
      cnt = cnt - 1;                                  /* 0062c2cf */
      *(volatile u32 *)(L + 0x10) = outerp;           /* 0062c2d0 */
      *(volatile s32 *)(L + 0x14) = cnt;              /* 0062c2d4 */
      if (cnt == 0) return;                           /* 0062c2d8 */
      rec = (u8 *)outerp;
    }
  }
}
