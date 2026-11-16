/* Stack reconstruction: L+8 holds the 717410 result, L+12 the float;
   the byte output at L+20 is the incoming argument home. The 6ea4e0
   call has eight stack arguments, and 6d6f70 has seven. */
/* Draft for preferred-image VA 0x006d1210, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))
#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
extern u8 cm_fn_0053b900(int *param_1);
extern unsigned long long CM_THISCALL cm_fn_005fd210(void *self,u32 arg1);
extern u8 CM_THISCALL cm_fn_005fd480(int *param_1,int *param_2);
extern /* WARNING: Globals starting with '_' overlap smaller symbols at the same address */ void CM_THISCALL cm_fn_006bbb60(int param_1,char param_2);
extern void CM_FASTCALL cm_fn_006bbf60(int param_1);
extern void CM_THISCALL cm_fn_006cda20(char *param_1,char *param_2,char param_3);
extern u32 CM_THISCALL cm_fn_006ce110(volatile u8 *self, s8 selector);
extern void CM_THISCALL cm_fn_006d6f70(char *param_1,u16 param_2,char *param_3,char param_4,u8 param_5,char param_6, char param_7,u32 param_8);
extern void CM_THISCALL cm_fn_006e1090(int param_1,char param_2);
extern char CM_THISCALL cm_fn_006e1a40(int param_1, int mode);
extern void CM_FASTCALL cm_fn_006e1aa0(int param_1);
extern void CM_THISCALL cm_fn_006ea4e0(int param_1,int param_2,int param_3,char *param_4,u8 param_5,char param_6, char param_7,float *param_8,char param_9);
extern void CM_THISCALL cm_fn_00716450(int param_1,float param_2);
extern u32 CM_THISCALL cm_fn_00716630(int param_1,char param_2);
extern void CM_THISCALL cm_fn_00716e10(int param_1,u32 param_2);
extern u32 CM_FASTCALL cm_fn_00717410(int param_1);
extern u32 CM_THISCALL cm_fn_00897760(int param_1,int param_2,char param_3,u32 param_4);
extern int cm_fn_0090cfc0(int param_1);

void CM_THISCALL cm_fn_006d1210(int param_1,u32 param_2)

{
  int *piVar1;
  float fVar2;
  char cVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  u32 uVar7;
  u32 uVar8;
  short sVar9;
  int iVar10;
  float fVar11;
  u8 local_10;
  int local_8;
  float local_4;
  
  if (*(char *)(param_1 + 0x8eb4) == '\0') {
    cm_fn_00716630((int)param_2, 0);
    if (*(char *)(param_1 + 0x1a3) != '\0') {
      *(u8 *)(param_1 + 0x8eb4) = 1;
      return;
    }
  }
  else {
    local_10 = 0;
    if ('\0' < *(char *)(param_1 + 0x1a3)) {
      do {
        iVar10 = param_1 + (char)local_10 * 0xb;
        sVar4 = cm_fn_00717410(*(int *)(iVar10 + 0xb1));
        *(u8 *)&param_2 = 1;
        sVar9 = -1;
        cm_fn_006ea4e0(param_1 + 0xf5b2,*(u32 *)(iVar10 + 0xb1),*(u32 *)(iVar10 + 0xb5),(char *)&param_2,
                     *(u8 *)(param_1 + (char)local_10 * 0xb + 0xbb),
                     *(u8 *)(iVar10 + 0xb9),0,&local_4,0);
        if (param_2 == '\x02') {
          sVar9 = 0x1ff9;
LAB_006d132f:
          iVar5 = cm_fn_0090cfc0((int)*(char *)(iVar10 + 0xb9));
          iVar6 = cm_fn_0090cfc0(0x14);
          if (iVar6 < iVar5) {
            cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),0x20f5,(char *)param_1,0,*(char *)(*(int *)(iVar10 + 0xb1) + 0x27) == '\0',
                         -1,-1,0);
          }
          cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),0x2109,(char *)param_1,*(u8 *)(*(int *)(iVar10 + 0xb1) + 0x2e),
                       *(u8 *)(*(int *)(iVar10 + 0xb1) + 0x27),-1,-1,0);
          cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),sVar9,(char *)param_1,*(u8 *)(*(int *)(iVar10 + 0xb1) + 0x2e),
                       *(u8 *)(*(int *)(iVar10 + 0xb1) + 0x27),-1,-1,0);
        }
        else {
          if (param_2 == '\x03') {
            sVar9 = 0x1ff7;
LAB_006d130e:
            *(u8 *)(*(char *)(*(int *)(iVar10 + 0xb1) + 0x27) + 0x1cb + param_1) = sVar4 == 1;
            goto LAB_006d132f;
          }
          if (param_2 == '\x04') {
            sVar9 = 0x1ff8;
            goto LAB_006d130e;
          }
          iVar5 = cm_fn_0090cfc0(7);
          if ((iVar5 + 2 < (int)*(char *)(iVar10 + 0xb9)) &&
             (-1 < *(char *)(*(int *)(iVar10 + 0xb1) + 0x19))) {
            sVar9 = 0x210a;
            goto LAB_006d132f;
          }
        }
        switch(*(u8 *)(iVar10 + 0xba)) {
        case 0x25:
        case 0x2a:
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x60:
        case 0x61:
switchD_006d13d1_caseD_25:
          if ((param_2 == '\x04') || (param_2 == '\x03')) {
            uVar7 = *(u32 *)(iVar10 + 0xb1);
LAB_006d1565:
            cm_fn_006cda20((char *)param_1,(char *)uVar7,0);
          }
          break;
        default:
          if (sVar9 < 0) goto switchD_006d13d1_caseD_25;
          iVar5 = cm_fn_0090cfc0(9);
          if (iVar5 == 0) {
            cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),0x20df,(char *)param_1,-1,*(u8 *)(*(int *)(iVar10 + 0xb1) + 0x27),
                         -1,-1,0);
            iVar5 = cm_fn_0090cfc0(3);
            if (iVar5 == 0) {
              cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),0x20e0,(char *)param_1,-1,*(u8 *)(*(int *)(iVar10 + 0xb1) + 0x27)
                           ,-1,-1,0);
            }
            if ((((param_2 == '\x06') || (param_2 == '\x03')) || (param_2 == '\x04')) ||
               (param_2 == '\x05')) {
              *(u8 *)(*(int *)(iVar10 + 0xb1) + 0x16) = 1;
              iVar5 = cm_fn_0090cfc0(7);
              if (iVar5 == 0) {
                cm_fn_006d6f70(*(char **)(param_1 + 0x8f7a),0x20d9,(char *)param_1,-1,
                             *(u8 *)(*(int *)(iVar10 + 0xb1) + 0x27),-1,-1,0
                            );
              }
            }
          }
          fVar2 = local_4;
          iVar6 = *(int *)(iVar10 + 0xb1);
          local_8 = 0x14;
          iVar5 = param_1 + 0x4796 + *(char *)(iVar6 + 0x27) * 0x22d8;
          do {
            if (*(int *)(iVar5 + 0x69) != 0) {
              fVar11 = fVar2;
              if (iVar5 != iVar6) {
                if (*(char *)(iVar5 + 0x19) < '\0') {
                  fVar11 = fVar2 * CM_GLOBAL(double,0x96a7a8) + CM_GLOBAL(double,0x96d0e8);
                }
                else {
                  fVar11 = (fVar2 + CM_GLOBAL(double,0x967890)) * CM_GLOBAL(double,0x968968);
                }
              }
              cm_fn_00716450(iVar5,fVar11);
            }
            iVar5 = iVar5 + 0x1be;
            local_8 = local_8 + -1;
          } while (local_8 != 0);
          if ((param_2 == '\x04') || (param_2 == '\x03')) {
            cm_fn_00716e10(*(int *)(iVar10 + 0xb1),1);
            uVar7 = *(u32 *)(iVar10 + 0xb1);
            goto LAB_006d1565;
          }
          cm_fn_00716e10(*(int *)(iVar10 + 0xb1),0);
        }
        cVar3 = (char)local_10 + '\x01';
        local_10 = (u8)cVar3;
      } while (cVar3 < *(char *)(param_1 + 0x1a3));
    }
    cVar3 = '\0';
    *(u8 *)(param_1 + 0x1a3) = 0;
    local_10 = local_10 & 0xffffff00;
    do {
      iVar10 = (int)cVar3;
      if (*(char *)(param_1 + 0x1cb + iVar10) != '\0') {
        iVar5 = iVar10 * 0xd0 + param_1;
        piVar1 = *(int **)(iVar10 * 0xd0 + 0x904d + param_1);
        if ((((piVar1 == (int *)0x0) || (*piVar1 < DAT_00ae2364 + -0x10)) ||
            (DAT_00ae2364 <= *piVar1)) ||
           ((*(char *)((int)piVar1 + 0x5f) == '\0' || (iVar6 = cm_fn_005fd210(CM_GLOBAL(void *,0xb63c98),(u32)piVar1), iVar6 != 0))))
        {
LAB_006d163b:
          if (*(char *)(iVar5 + 0x9049) == '\0') {
            cm_fn_006e1aa0(iVar5 + 0x9032);
            iVar5 = *(int *)(param_1 + 0x4792);
            if ((char)local_10 == '\0') {
              cVar3 = *(char *)(iVar5 + 0x40);
              uVar7 = *(u32 *)(iVar5 + 0x1c);
            }
            else {
              cVar3 = *(char *)(iVar5 + 0x41);
              uVar7 = *(u32 *)(iVar5 + 0x20);
            }
            iVar5 = (int)(cVar3 == '\0');
            uVar8 = cm_fn_006e1a40(param_1 + (int)(s8)local_10 * 0xd0 + 0x9032,iVar5);
            cm_fn_00897760(CM_GLOBAL(int, 0x00de1d08), uVar7, uVar8, iVar5);
            if ((char)local_10 == '\0') {
              *(u32 *)(param_1 + 0xf59e) = 0;
            }
            else {
              *(u32 *)(param_1 + 0xf5a2) = 0;
            }
            cm_fn_006ce110((volatile u8 *)param_1, (s8)local_10);
          }
          else {
            cm_fn_006e1090(iVar5 + 0x9032, 1);
          }
          *(u16 *)(param_1 + 0x8ecc + iVar10 * 2) = *(u16 *)(param_1 + 0x8ed0);
          cm_fn_006bbb60(param_1, local_10);
          cm_fn_006bbf60(param_1);
        }
        else if (*(char *)(iVar5 + 0x904b) != '\0') {
          if ((char)local_10 == '\0') {
            uVar7 = *(u32 *)(*(int *)(param_1 + 0x4792) + 0x1c);
          }
          else {
            uVar7 = *(u32 *)(*(int *)(param_1 + 0x4792) + 0x20);
          }
          iVar6 = cm_fn_0053b900((int *)uVar7);
          if ((iVar6 == 0) &&
              (iVar6 = cm_fn_005fd480(CM_GLOBAL(int *, 0x00b63c98),
                                      *(int **)(iVar5 + 0x904d)), iVar6 == 0))
          goto LAB_006d163b;
        }
      }
      *(u8 *)(param_1 + 0x1cb + iVar10) = 0;
      cVar3 = (char)local_10 + '\x01';
      local_10 = (u8)cVar3;
    } while (cVar3 < '\x02');
    *(u8 *)(param_1 + 0x8eb4) = 2;
  }
  return;
}
