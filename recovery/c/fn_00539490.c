/* WIP: five test iterations used. First mismatch is case 12 at -O0: original faults on byte write to 0x5403000e after local initialization; recovered faults at 0x0203ffff. The draft local-stack buffer mapping/order remains unresolved. */
/* Draft for preferred-image VA 0x00539490, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern void CM_CDECL cm_fn_0053a8b0(u8 *source, u32 unused, u8 *destination, u8 *marker);
extern void CM_CDECL cm_fn_0053a980(void *dst, u32 arg2_unused, const void *src);

/* NOTE: Type propagation algorithm not settling */

void cm_fn_00539490(int param_1)

{
  short *psVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  u8 *puVar5;
  int iVar6;
  int iVar7;
  char cVar8;
  char cVar9;
  u32 uVar10;
  int iVar11;
  char local_26b;
  char local_26a;
  char local_269;
  int local_268;
  int local_264;
  short *local_25c;
  int local_258;
  short local_250;
  u8 local_24e [55];
  short local_217;
  int aiStack_190 [100];
  
  if ((param_1 != 0) && (local_258 = *(int *)(param_1 + 0x69), local_258 != 0)) {
    puVar5 = local_24e;
    iVar7 = 0x3f;
    do {
      *(u16 *)(puVar5 + 0xfffffffe) = 0xff81;
      *puVar5 = 0x81;
      puVar5 = puVar5 + 3;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
    cm_fn_0053a8b0((u8 *)local_258,(u32)param_1,(u8 *)&local_250,(u8 *)&local_26a);
    local_269 = '\0';
    local_25c = (short *)((int)&local_250 + local_26a * 3);
    do {
      local_264 = 0;
      sVar4 = *local_25c;
      local_268 = 0x28;
      do {
        iVar7 = 0;
        cVar8 = '\0';
        local_26b = -1;
        sVar3 = sVar4;
        cVar9 = local_26a;
        if (sVar4 != -0x7f) {
          do {
            uVar10 = (u32)sVar3;
            if ((int)(short)local_268 <
                (int)((uVar10 ^ (int)uVar10 >> 0x1f) - ((int)uVar10 >> 0x1f))) {
              aiStack_190[(u8)cVar9] = 0;
              if ((cVar9 == '\x13') &&
                 ((local_26b == -1 || (*(short *)((int)&local_250 + local_26b * 3) < local_217)))) {
                local_26b = '\x13';
              }
            }
            else {
              iVar7 = iVar7 + uVar10;
              cVar8 = cVar8 + '\x01';
              aiStack_190[(u8)cVar9] = 1;
            }
            sVar3 = *(short *)((int)&local_250 + (char)(cVar9 + '\x01') * 3);
            cVar9 = cVar9 + '\x01';
          } while (sVar3 != -0x7f);
          if ('\x05' < cVar8) break;
        }
        local_268 = local_268 + 10;
      } while (cVar8 < '\x06');
      iVar11 = 0;
      if (-1 < local_26b) {
        aiStack_190[(u8)local_26b] = 1;
      }
      cVar9 = local_26a;
      while (sVar4 != -0x7f) {
        if (cVar9 != local_26b) {
          iVar6 = (int)cVar9;
          if (aiStack_190[(u8)iVar6] == 1) {
            iVar2 = iVar6 + -0x250;
            psVar1 = (short *)((int)&local_250 + iVar6 * 2 + iVar2 + 0x250);
            *psVar1 = *psVar1 + (-5 - (short)(iVar7 / (int)cVar8));
            psVar1 = (short *)((int)&local_250 + iVar6 * 2 + iVar2 + 0x250);
            sVar4 = *(short *)((int)&local_250 + iVar6 * 2 + iVar2 + 0x250);
            if (sVar4 < -0x7d) {
              *psVar1 = -0x7d;
            }
            else if (sVar4 < 0x7e) {
              *psVar1 = sVar4;
            }
            else {
              *psVar1 = 0x7d;
            }
          }
          else if ((local_269 == '\0') &&
                  (0 < *(short *)((int)&local_250 + iVar6 * 2 + iVar6 + -0x250 + 0x250))) {
            iVar11 = iVar11 + 1;
            local_264 = local_264 + *(short *)((int)&local_250 + iVar6 * 2 + iVar6 + -0x250 + 0x250)
            ;
          }
          else {
            aiStack_190[(u8)iVar6] = 1;
          }
          if ((*(char *)(param_1 + 0x3d) == '\n') || (*(char *)(param_1 + 0x28) == '\n')) {
            if (cVar9 == '\x13') {
              if (local_217 < 1) {
                local_217 = 0;
              }
            }
            else {
              psVar1 = (short *)((int)&local_250 + iVar6 * 3);
              sVar4 = *psVar1;
              if (-0x51 < sVar4) {
                sVar4 = -0x50;
              }
              *psVar1 = sVar4;
            }
          }
        }
        cVar9 = cVar9 + '\x01';
        sVar4 = *(short *)((int)&local_250 + cVar9 * 3);
      }
      sVar4 = *local_25c;
      cVar9 = local_26a;
      while (((sVar4 != -0x7f && (iVar11 != 0)) && (0x1c < local_264))) {
        if (aiStack_190[(u8)cVar9] != 1) {
          psVar1 = (short *)((int)&local_250 + cVar9 * 3);
          *psVar1 = *psVar1 - (short)((local_264 + -0x1c) / iVar11);
          if (*psVar1 < 0) {
            *psVar1 = 0;
          }
        }
        cVar9 = cVar9 + '\x01';
        sVar4 = *(short *)((int)&local_250 + cVar9 * 3);
      }
      local_269 = local_269 + '\x01';
    } while (local_269 < '\x02');
    cm_fn_0053a980((void *)local_258,(u32)param_1,&local_250);
  }
  return;
}
