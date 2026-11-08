/* Preferred-image VA00538ec0, base00400000; 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   CDECL one stack argument. All three indexed return edges are dead through
   00538bc0 and their continuations; the 007abac0 forwarding edge ends at the
   EAX kills007a665e/007a6680 in its sole indexed caller007a664f. */
#include "cmre.h"
#define DAT_00de1f44 CM_GLOBAL(volatile u32, 0x00de1f44)
extern void CM_CDECL cm_fn_0053a610(const volatile s8 *,const volatile s8 *,volatile s16 *,volatile u8 *,volatile u8 *,volatile u8 *);
extern void CM_CDECL cm_fn_0053aa10(volatile u8 *,volatile u8 *,volatile u8 *);
extern u32 CM_THISCALL cm_fn_008b3640(u32,u32,u32);
__attribute__((target("no-sse3")))
void CM_CDECL cm_fn_00538ec0(u32 param_1)

{
  char cVar1;
  short sVar2;
  volatile s32 iVar3;
  volatile s32 iVar4;
  volatile s32 iVar5;
  volatile s32 iVar6;
  volatile s32 iVar7;
  volatile s32 iVar8;
  volatile u8 *puVar9;
  char cVar10;
  int iVar11;
  volatile s32 *piVar12;
  volatile s16 *psVar13;
  u32 uVar14;
  /* L is native ESP immediately after SUB ESP,278h. All related
     locals, 63 packed triples and 100 DWORD flags share this allocation. */
  volatile u8 scratch[0x278];
  volatile long double t;
  u32 context;
#define local_278 (*(volatile s8 *)(scratch + 0x00))
#define local_277 (*(volatile s8 *)(scratch + 0x01))
#define local_276 (*(volatile s8 *)(scratch + 0x02))
#define local_275 (*(volatile s8 *)(scratch + 0x03))
#define local_274 (*(volatile u32 *)(scratch + 0x04))
#define local_26d (*(volatile s8 *)(scratch + 0x0b))
#define local_26c (*(volatile s32 *)(scratch + 0x0c))
#define local_265 (*(volatile s8 *)(scratch + 0x13))
#define local_264 (*(volatile s32 * volatile *)(scratch + 0x14))
#define local_260 (*(volatile s32 *)(scratch + 0x18))
#define local_25c (*(volatile u32 *)(scratch + 0x1c))
#define local_258 (*(volatile s32 *)(scratch + 0x20))
#define local_251 (*(volatile u8 *)(scratch + 0x27))
#define local_250 (*(volatile s16 *)(scratch + 0x28))
#define local_24e (scratch + 0x2a)
#define aiStack_190 ((volatile s32 *)(scratch + 0xe8))
  if ((param_1 != 0) && (local_25c = *(volatile s32 *)(param_1 + 0x61), local_25c != 0)) {
    context = DAT_00de1f44;
    if (context != 0) {
      cm_fn_008b3640(context,param_1,0);
    }
    iVar11 = *(volatile s32 *)(param_1 + 0x61);
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x2f);
    local_26c = (s32)*(volatile s8 *)((u32)iVar11 + 0x1b);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x00969548); /* __ftol 00538f16 */
    iVar3 = (s32)(u32)(long long)t;
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x36);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x00968940); /* __ftol 00538f2f */
    iVar4 = (s32)(u32)(long long)t;
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x2e);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x00969540); /* __ftol 00538f4a */
    iVar5 = (s32)(u32)(long long)t;
    t = (long double)local_26c;
    t *= CM_GLOBAL(volatile double, 0x00969538); /* __ftol 00538f5b */
    iVar6 = (s32)(u32)(long long)t;
    cVar10 = *(volatile s8 *)((u32)iVar11 + 0x30);
    cVar1 = *(volatile s8 *)((u32)iVar11 + 0x3b);
    if (cVar1 < cVar10) cVar10 = cVar1;
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x40);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x00969548); /* __ftol 00538f83 */
    iVar7 = (s32)(u32)(long long)t;
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x3f);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x009689b0); /* __ftol 00538f9c */
    iVar8 = (s32)(u32)(long long)t;
    local_274 = (u32)(s32)cVar10;
    t = (long double)(s32)local_274;
    local_274 = (u32)iVar8 - (u32)iVar7;
    t *= CM_GLOBAL(volatile double, 0x00969530); /* __ftol 00538fba */
    iVar7 = (s32)(u32)(long long)t;
    local_274 = (u32)(s32)*(volatile s8 *)((u32)iVar11 + 0x44) +
        ((u32)iVar4 - (u32)iVar3 - (u32)iVar5 - (u32)iVar6) +
        (local_274 - (u32)iVar7);
    t = (long double)(s32)local_274;
    t *= CM_GLOBAL(volatile double, 0x00969528); /* __ftol 00538fdb */
    iVar3 = (s32)(u32)(long long)t;
    iVar3 = (s32)((u32)((s32)*(volatile s16 *)(local_25c + 5) / 2) -
                      (u32)iVar3 + 0x37u) / 2;
    if (iVar3 < -0xe) {
      iVar4 = iVar3 / 0xf;
      iVar5 = iVar4;
      if (iVar4 < -1) {
        iVar5 = -2;
      }
      if (iVar5 + local_26c < 1) {
        cVar10 = '\x01';
      }
      else {
        iVar5 = iVar4;
        if (iVar4 < -1) {
          iVar5 = -2;
        }
        cVar10 = (char)iVar5 + (char)local_26c;
      }
      *(volatile s8 *)((u32)iVar11 + 0x1b) = cVar10;
      iVar11 = iVar4;
      if (iVar4 < -1) {
        iVar11 = -2;
      }
      cVar10 = *(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x36);
      if (iVar11 + cVar10 < 1) {
        cVar10 = '\x01';
      }
      else {
        iVar11 = iVar4;
        if (iVar4 < -1) {
          iVar11 = -2;
        }
        cVar10 = cVar10 + (char)iVar11;
      }
      *(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x36) = cVar10;
      iVar11 = iVar4;
      if (iVar4 < -1) {
        iVar11 = -2;
      }
      cVar10 = *(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x40);
      if (iVar11 + cVar10 < 1) {
        cVar10 = '\x01';
      }
      else {
        iVar11 = iVar4;
        if (iVar4 < -1) {
          iVar11 = -2;
        }
        cVar10 = cVar10 + (char)iVar11;
      }
      *(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x40) = cVar10;
      iVar11 = iVar4;
      if (iVar4 < -1) {
        iVar11 = -2;
      }
      iVar5 = *(volatile s32 *)(param_1 + 0x61);
      if (iVar11 + *(volatile s8 *)((u32)iVar5 + 0x44) < 1) {
        *(volatile u8 *)((u32)iVar5 + 0x44) = 1;
      }
      else {
        if (iVar4 < -1) {
          iVar4 = -2;
        }
        *(volatile s8 *)((u32)iVar5 + 0x44) = *(volatile s8 *)((u32)iVar5 + 0x44) + (char)iVar4;
      }
    }
    local_274 = iVar3 - 0x14;
    sVar2 = (short)(((int)*(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x36) *
                    (int)*(volatile s8 *)(*(volatile s32 *)(param_1 + 0x61) + 0x1b)) / 2);
    if (*(volatile s16 *)(local_25c + 5) < sVar2) {
      iVar11 = (int)sVar2 - (int)*(volatile s16 *)(local_25c + 5);
      if (0x18 < iVar11) {
        iVar11 = 0x19;
      }
      local_274 = local_274 - iVar11;
    }
    if ((int)local_274 < -0x28) {
      local_274 = 0xffffffd8;
    }
    else {
      local_274 = ((0 < (int)local_274) - 1) & local_274;
    }
    puVar9 = local_24e;
    iVar11 = 0x3f;
    do {
      *(volatile u16 *)(puVar9 + -2) = 0xff81;
      *puVar9 = 0x81;
      puVar9 = puVar9 + 3;
      iVar11 = iVar11 + -1;
    } while (iVar11 != 0);
    cm_fn_0053a610((const volatile s8 *)local_25c,(const volatile s8 *)param_1,(volatile s16 *)(scratch+0x28),scratch+0x13,scratch+0x27,scratch);
    local_277 = '\0';
    local_26d = local_265 + '\x02';
    do {
      local_26c = 0;
      local_258 = 0;
      local_260 = 0x1e;
      cVar10 = local_278;
      do {
        local_264 = (volatile s32 *)0x0;
        local_276 = '\0';
        local_275 = -1;
        if (local_26d < cVar10) {
          psVar13 = (volatile s16 *)(local_24e + local_26d * 3 + -2);
          piVar12 = aiStack_190 + local_26d;
          cVar10 = local_26d;
          do {
            cVar1 = local_275;
            if ((cVar10 != '\x1e') && (cVar10 != '\x18')) {
              sVar2 = *psVar13;
              uVar14 = (u32)sVar2;
              if ((int)(short)local_260 <
                  (int)((uVar14 ^ (int)uVar14 >> 0x1f) - ((int)uVar14 >> 0x1f))) {
                *piVar12 = 0;
                if ((cVar1 == -1) || (*(volatile s16 *)(local_24e + cVar1 * 3 + -2) < sVar2)) {
                  local_275 = cVar10;
                }
              }
              else {
                local_264 = (volatile s32 *)((int)local_264 + uVar14);
                local_276 = local_276 + '\x01';
                *piVar12 = 1;
              }
            }
            cVar10 = cVar10 + '\x01';
            piVar12 = piVar12 + 1;
            psVar13 = (volatile s16 *)((int)psVar13 + 3);
          } while (cVar10 < local_278);
          cVar10 = local_278;
          if ('\v' < local_276) break;
        }
        local_260 = local_260 + 5;
      } while (local_276 < '\f');
      if (-1 < local_275) {
        aiStack_190[(s32)local_275] = 1;
      }
      local_260 = local_274 - (int)local_264 / (int)local_276;
      cVar10 = local_26d;
      if (local_26d < local_278) {
        psVar13 = (volatile s16 *)(local_24e + local_26d * 3 + -2);
        piVar12 = aiStack_190 + local_26d;
        local_264 = piVar12;
        iVar11 = local_258;
        iVar3 = local_26c;
        do {
          if (((cVar10 != '\x1e') && (cVar10 != '\x18')) && (cVar10 != local_275)) {
            if (*piVar12 == 1) {
              sVar2 = *psVar13 + (short)local_260;
              *psVar13 = sVar2;
              if (sVar2 < -0x7d) {
                *psVar13 = -0x7d;
              }
              else if (sVar2 < 0x7e) {
                *psVar13 = sVar2;
              }
              else {
                *psVar13 = 0x7d;
              }
            }
            else if (((local_277 == '\0') && (sVar2 = *psVar13, 0 < sVar2)) &&
                    (*(volatile s8 *)(local_25c + 0xf) < '\x0f')) {
              if ((cVar10 < '\x11') || ('\x12' < cVar10)) {
                iVar3 = iVar3 + 1;
                iVar11 = iVar11 + sVar2;
              }
              else {
                local_26c = (int)sVar2;
                iVar3 = iVar3 + 1;
                local_264 = piVar12;
                t = (long double)local_26c;
                t = t + t; /* __ftol 00539333 */
                iVar4 = (s32)(u32)(long long)t;
                iVar11 = iVar11 + iVar4;
                piVar12 = local_264;
              }
            }
            else {
              *piVar12 = 1;
            }
            if ((local_250 < 0xf) && (*(volatile u8 *)((u32)psVar13 + 2u) == 1)) {
              *psVar13 = -0x78;
            }
          }
          cVar10 = cVar10 + '\x01';
          piVar12 = piVar12 + 1;
          psVar13 = (volatile s16 *)((int)psVar13 + 3);
          local_264 = piVar12;
        } while (cVar10 < local_278);
        local_258 = iVar11;
        local_26c = iVar3;
        cVar10 = local_26d;
      }
      while (((iVar11 = local_25c, cVar10 < local_278 && (1 < local_26c)) && (0x55 < local_258))) {
        if (((cVar10 != '\x1e') && (cVar10 != '\x18')) && (aiStack_190[(s32)cVar10] != 1)) {
          psVar13 = (volatile s16 *)(local_24e + cVar10 * 3 + -2);
          *psVar13 = *psVar13 - (short)((local_258 + -0x55) / local_26c);
          if (*psVar13 < 0) {
            *psVar13 = 0;
          }
        }
        cVar10 = cVar10 + '\x01';
      }
      local_277 = local_277 + '\x01';
    } while (local_277 < '\x02');
    cm_fn_0053aa10((volatile u8 *)local_25c,(volatile u8 *)param_1,scratch+0x28);
    if (*(volatile s8 *)((u32)iVar11 + 0xf) == 0x14) {
      s8 right = *(volatile s8 *)((u32)iVar11 + 0x3a);
      s8 left = *(volatile s8 *)((u32)iVar11 + 0x2a);
      s32 average = ((s32)left + (s32)right) / 2;
      s8 first = *(volatile s8 *)((u32)iVar11 + 0x1e);
      *(volatile u8 *)((u32)iVar11 + 0x2a) = (u8)((u8)left - (u8)average);
      s8 second = *(volatile s8 *)((u32)iVar11 + 0x24);
      local_277 = first;
      *(volatile u8 *)((u32)iVar11 + 0x3a) = (u8)((u8)right - (u8)average);
      s8 third = *(volatile s8 *)((u32)iVar11 + 0x39);
      average = ((s32)first + (s32)second + (s32)third) / 3;
      *(volatile u8 *)((u32)iVar11 + 0x1e) = (u8)((u8)first - (u8)average);
      *(volatile u8 *)((u32)iVar11 + 0x39) = (u8)((u8)third - (u8)average);
      *(volatile u8 *)((u32)iVar11 + 0x24) = (u8)((u8)second - (u8)average);
    }
    context = DAT_00de1f44;
    if (context != 0) {
      cm_fn_008b3640(context,param_1,1);
    }
  }
  return;
}
