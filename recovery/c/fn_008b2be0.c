/* WIP: caller reads EAX after this ret (VA 0x008b33b1); normal-path EAX is left by branch-dependent partial/full writers in the table loop, and error returns leave the incidental EAX from helper 005e8290. No listing-derived single C return expression is established, so do not guess a normalization. */
/* Draft for preferred-image VA 0x008b2be0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae2364 CM_GLOBAL(volatile int, 0x00ae2364)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_tr_00a8ccd0 ((u32)0x00a8ccd0)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((u32)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern int CM_THISCALL cm_fn_008b4b20(int self,char *param_2,int *param_3);
extern char * cm_fn_0090d130(...);  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_008b2be0(int param_1,int *param_2,char *param_3)

{
  u8 bVar1;
  int *piVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  u8 *puVar8;
  u8 *puVar9;
  u32 uVar10;
  char local_20a;
  int local_204;
  u8 scratch[512];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  local_20a = '\0';
  if (param_2 == (int *)0x0) {
    cm_fn_00944cff((u32)0x00a8ccd0,0,0,local_100,local_200);
    puVar9 = local_200;
    puVar8 = local_100;
    uVar10 = 0x4b3;
  }
  else {
    piVar2 = *(int **)((int)param_2 + 0x61);
    if (piVar2 != (int *)0x0) {
      if (*param_2 < DAT_00ae2364) {
        iVar3 = *(volatile int *)(param_1 + 0x651d) + *piVar2 * 6;
        if (param_3[3] != '\0') {
          iVar7 = (int)*(volatile char *)(iVar3 + 4) >> 4;
          cVar4 = (char)((param_3[3] + iVar7) / 2) -
                  (char)(iVar7 - ((int)*(volatile char *)(iVar3 + 4) >> 0x1f) >> 1);
          if (cVar4 != '\0') {
            iVar6 = (int)cVar4;
            iVar7 = *(volatile char *)((int)piVar2 + 0x22) + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x22) = (char)iVar7;
            iVar7 = iVar6 * 6;
            iVar5 = *(volatile char *)((int)piVar2 + 0x23) + iVar7;
            if (iVar5 < -0x7d) {
              iVar5 = -0x7d;
            }
            else if (0x7d < iVar5) {
              iVar5 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x23) = (char)iVar5;
            iVar5 = *(volatile char *)((int)piVar2 + 0x26) + iVar7;
            if (iVar5 < -0x7d) {
              iVar5 = -0x7d;
            }
            else if (0x7d < iVar5) {
              iVar5 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x26) = (char)iVar5;
            iVar5 = *(volatile char *)((int)piVar2 + 0x29) + iVar6;
            if (iVar5 < 1) {
              iVar5 = 1;
            }
            else if (0x14 < iVar5) {
              iVar5 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x29) = (char)iVar5;
            iVar5 = *(volatile char *)((int)piVar2 + 0x2b) + iVar7;
            if (iVar5 < -0x7d) {
              iVar5 = -0x7d;
            }
            else if (0x7d < iVar5) {
              iVar5 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x2b) = (char)iVar5;
            iVar5 = *(volatile char *)((int)piVar2 + 0x37) + iVar7;
            if (iVar5 < -0x7d) {
              iVar5 = -0x7d;
            }
            else if (0x7d < iVar5) {
              iVar5 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x37) = (char)iVar5;
            iVar5 = *(volatile char *)((int)piVar2 + 0x3e) + iVar7;
            if (iVar5 < -0x7d) {
              iVar5 = -0x7d;
            }
            else if (0x7d < iVar5) {
              iVar5 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x3e) = (char)iVar5;
            iVar6 = (char)piVar2[0x10] + iVar6;
            if (iVar6 < 1) {
              iVar6 = 1;
            }
            else if (0x14 < iVar6) {
              iVar6 = 0x14;
            }
            iVar7 = iVar7 + *(volatile char *)((int)piVar2 + 0x41);
            *(volatile char *)(piVar2 + 0x10) = (char)iVar6;
            if (iVar7 < -0x7d) {
              iVar7 = -0x7d;
            }
            else if (0x7d < iVar7) {
              iVar7 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x41) = (char)iVar7;
          }
          *(volatile u8 *)(iVar3 + 4) =
               (*(volatile u8 *)(iVar3 + 4) & 0xf0) + param_3[3] * '\x10' ^ *(volatile u8 *)(iVar3 + 4) & 0xf;
        }
        if (*param_3 != '\0') {
          iVar7 = (int)(char)(*(volatile char *)(iVar3 + 3) << 4);
          iVar6 = iVar7 >> 4;
          cVar4 = (char)((*param_3 + iVar6) / 2) - (char)(iVar6 - (iVar7 >> 0x1f) >> 1);
          if (cVar4 != '\0') {
            iVar6 = (int)cVar4;
            iVar7 = *(volatile char *)((int)piVar2 + 0x1b) + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x1b) = (char)iVar7;
            iVar7 = *(volatile char *)((int)piVar2 + 0x1d) + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x1d) = (char)iVar7;
            iVar7 = *(volatile char *)((int)piVar2 + 0x2e) + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x2e) = (char)iVar7;
            iVar7 = (char)piVar2[0xd] + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)(piVar2 + 0xd) = (char)iVar7;
            iVar7 = *(volatile char *)((int)piVar2 + 0x36) + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x36) = (char)iVar7;
            iVar7 = (int)*(volatile char *)((int)piVar2 + 0x3a) + iVar6 * 6;
            if (iVar7 < -0x7d) {
              iVar7 = -0x7d;
            }
            else if (0x7d < iVar7) {
              iVar7 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x3a) = (char)iVar7;
            iVar7 = (char)piVar2[0xf] + iVar6;
            if (iVar7 < 1) {
              iVar7 = 1;
            }
            else if (0x14 < iVar7) {
              iVar7 = 0x14;
            }
            iVar6 = iVar6 + *(volatile char *)((int)piVar2 + 0x3d);
            *(volatile char *)(piVar2 + 0xf) = (char)iVar7;
            if (iVar6 < 1) {
              iVar6 = 1;
            }
            else if (0x14 < iVar6) {
              iVar6 = 0x14;
            }
            *(volatile char *)((int)piVar2 + 0x3d) = (char)iVar6;
          }
          bVar1 = *(volatile u8 *)(iVar3 + 3);
          *(volatile u8 *)(iVar3 + 3) = (((char)(bVar1 << 4) >> 4) + *param_3 ^ bVar1) & 0xf ^ bVar1;
        }
        if (param_3[2] != '\0') {
          iVar7 = (int)(char)(*(volatile char *)(iVar3 + 4) << 4);
          iVar6 = iVar7 >> 4;
          cVar4 = (char)((param_3[2] + iVar6) / 2) - (char)(iVar6 - (iVar7 >> 0x1f) >> 1);
          if (cVar4 != '\0') {
            iVar7 = cVar4 * 6;
            iVar6 = *(volatile char *)((int)piVar2 + 0x27) + iVar7;
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x27) = (char)iVar6;
            iVar6 = *(volatile char *)((int)piVar2 + 0x31) + iVar7;
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x31) = (char)iVar6;
            iVar7 = iVar7 + (char)piVar2[0xe];
            if (iVar7 < -0x7d) {
              iVar7 = -0x7d;
            }
            else if (0x7d < iVar7) {
              iVar7 = 0x7d;
            }
            *(volatile char *)(piVar2 + 0xe) = (char)iVar7;
          }
          bVar1 = *(volatile u8 *)(iVar3 + 4);
          *(volatile u8 *)(iVar3 + 4) = (((char)(bVar1 << 4) >> 4) + param_3[2] ^ bVar1) & 0xf ^ bVar1;
        }
        if (param_3[1] != '\0') {
          iVar7 = (int)*(volatile char *)(iVar3 + 3) >> 4;
          cVar4 = (char)((param_3[1] + iVar7) / 2) -
                  (char)(iVar7 - ((int)*(volatile char *)(iVar3 + 3) >> 0x1f) >> 1);
          if (cVar4 != '\0') {
            iVar7 = cVar4 * 6;
            iVar6 = (char)piVar2[9] + iVar7;
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)(piVar2 + 9) = (char)iVar6;
            iVar6 = *(volatile char *)((int)piVar2 + 0x32) + iVar7;
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x32) = (char)iVar6;
            iVar6 = *(volatile char *)((int)piVar2 + 0x33) + iVar7;
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x33) = (char)iVar6;
            iVar6 = (int)*(volatile char *)((int)piVar2 + 0x3f) + (int)cVar4;
            if (iVar6 < 1) {
              iVar6 = 1;
            }
            else if (0x14 < iVar6) {
              iVar6 = 0x14;
            }
            iVar7 = iVar7 + *(volatile char *)((int)piVar2 + 0x39);
            *(volatile char *)((int)piVar2 + 0x3f) = (char)iVar6;
            if (iVar7 < -0x7d) {
              iVar7 = -0x7d;
            }
            else if (0x7d < iVar7) {
              iVar7 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x39) = (char)iVar7;
          }
          *(volatile u8 *)(iVar3 + 3) =
               param_3[1] * '\x10' + (*(volatile u8 *)(iVar3 + 3) & 0xf0) ^ *(volatile u8 *)(iVar3 + 3) & 0xf;
        }
        if (param_3[4] != '\0') {
          iVar7 = (int)(char)(*(volatile char *)(iVar3 + 5) << 4);
          iVar6 = iVar7 >> 4;
          cVar4 = (char)((param_3[4] + iVar6) / 2) - (char)(iVar6 - (iVar7 >> 0x1f) >> 1);
          if (cVar4 != '\0') {
            iVar6 = cVar4 * 6;
            iVar7 = *(volatile char *)((int)piVar2 + 0x2a) + iVar6;
            if (iVar7 < -0x7d) {
              iVar7 = -0x7d;
            }
            else if (0x7d < iVar7) {
              iVar7 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x2a) = (char)iVar7;
            iVar6 = iVar6 + *(volatile char *)((int)piVar2 + 0x35);
            if (iVar6 < -0x7d) {
              iVar6 = -0x7d;
            }
            else if (0x7d < iVar6) {
              iVar6 = 0x7d;
            }
            *(volatile char *)((int)piVar2 + 0x35) = (char)iVar6;
          }
          bVar1 = *(volatile u8 *)(iVar3 + 5);
          *(volatile u8 *)(iVar3 + 5) = (((char)(bVar1 << 4) >> 4) + param_3[4] ^ bVar1) & 0xf ^ bVar1;
        }
        iVar3 = cm_fn_008b4b20(param_1,param_3,&local_204);
        if (iVar3 < 1) {
          cVar4 = '\0';
        }
        else {
          cVar4 = '\x0e' < *(volatile char *)((int)piVar2 + 0xf);
          if ('\x0e' < (char)piVar2[4]) {
            cVar4 = cVar4 + '\x01';
          }
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x11)) {
            cVar4 = cVar4 + '\x01';
          }
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x12)) {
            cVar4 = cVar4 + '\x01';
          }
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x13)) {
            cVar4 = cVar4 + '\x01';
          }
          if ('\x0e' < (char)piVar2[5]) {
            cVar4 = cVar4 + '\x01';
          }
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x15)) {
            cVar4 = cVar4 + '\x01';
          }
          local_20a = '\x0e' < (char)piVar2[6];
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x19)) {
            local_20a = local_20a + '\x01';
          }
          if ('\x0e' < *(volatile char *)((int)piVar2 + 0x17)) {
            local_20a = local_20a + '\x01';
          }
        }
        iVar7 = 0;
        if (0 < iVar3) {
          do {
            iVar6 = (int)*(volatile short *)(local_204 + 4 + iVar7 * 8);
            if (*(volatile char *)(local_204 + 6 + iVar7 * 8) < '\x0f') {
              if (iVar6 < 0x21) {
                if (iVar6 == 0x20) {
                  if ('\x01' < cVar4) {
                    *(volatile u8 *)(piVar2 + 5) = 1;
                    cVar4 = cVar4 + -1;
                  }
                }
                else {
                  if (1 <= iVar6 && iVar6 <= 16) {
                    u8 mapped = *(volatile u8 *)(u32)(0x008b33cc + (u32)(iVar6 - 1));
                    (void)*(volatile u32 *)(u32)(0x008b33b4 + (u32)mapped * 4);
                  }
                  switch(iVar6) {
                  case 1:
                    if ('\x01' < cVar4) {
                      *(volatile u8 *)((int)piVar2 + 0xf) = 1;
                      cVar4 = cVar4 + -1;
                    }
                    break;
                  case 2:
                    if ('\x01' < cVar4) {
                      *(volatile u8 *)(piVar2 + 4) = 1;
                      cVar4 = cVar4 + -1;
                    }
                    break;
                  case 4:
                    if ('\x01' < cVar4) {
                      *(volatile u8 *)((int)piVar2 + 0x11) = 1;
                      cVar4 = cVar4 + -1;
                    }
                    break;
                  case 8:
                    if ('\x01' < cVar4) {
                      *(volatile u8 *)((int)piVar2 + 0x12) = 1;
                      cVar4 = cVar4 + -1;
                    }
                    break;
                  case 0x10:
                    if ('\x01' < cVar4) {
                      *(volatile u8 *)((int)piVar2 + 0x13) = 1;
                      cVar4 = cVar4 + -1;
                    }
                  }
                }
              }
              else if (iVar6 < 0x201) {
                if (iVar6 == 0x200) {
                  if ('\x01' < local_20a) {
                    *(volatile u8 *)((int)piVar2 + 0x19) = 1;
                    goto LAB_008b339b;
                  }
                }
                else if (iVar6 == 0x40) {
                  if ('\x01' < cVar4) {
                    *(volatile u8 *)((int)piVar2 + 0x15) = 1;
                    cVar4 = cVar4 + -1;
                  }
                }
                else if ((iVar6 == 0x80) && ('\x01' < local_20a)) {
                  *(volatile u8 *)(piVar2 + 6) = 1;
                  goto LAB_008b339b;
                }
              }
              else if ((iVar6 == 0x800) && ('\x01' < local_20a)) {
                *(volatile u8 *)((int)piVar2 + 0x17) = 1;
LAB_008b339b:
                local_20a = local_20a + -1;
              }
            }
            else if (iVar6 < 0x21) {
              if (iVar6 == 0x20) {
                *(volatile u8 *)(piVar2 + 5) = 0x14;
              }
              else {
                if (1 <= iVar6 && iVar6 <= 16) {
                  u8 mapped = *(volatile u8 *)(u32)(0x008b33f4 + (u32)(iVar6 - 1));
                  (void)*(volatile u32 *)(u32)(0x008b33dc + (u32)mapped * 4);
                }
                switch(iVar6) {
                case 1:
                  *(volatile u8 *)((int)piVar2 + 0xf) = 0x14;
                  break;
                case 2:
                  *(volatile u8 *)(piVar2 + 4) = 0x14;
                  break;
                case 4:
                  *(volatile u8 *)((int)piVar2 + 0x11) = 0x14;
                  break;
                case 8:
                  *(volatile u8 *)((int)piVar2 + 0x12) = 0x14;
                  break;
                case 0x10:
                  *(volatile u8 *)((int)piVar2 + 0x13) = 0x14;
                }
              }
            }
            else if (iVar6 < 0x201) {
              if (iVar6 == 0x200) {
                *(volatile u8 *)((int)piVar2 + 0x19) = 0x14;
              }
              else if (iVar6 == 0x40) {
                *(volatile u8 *)((int)piVar2 + 0x15) = 0x14;
              }
              else if (iVar6 == 0x80) {
                *(volatile u8 *)(piVar2 + 6) = 0x14;
              }
            }
            else if (iVar6 == 0x800) {
              *(volatile u8 *)((int)piVar2 + 0x17) = 0x14;
            }
            iVar7 = iVar7 + 1;
          } while (iVar7 < iVar3);
        }
      }
      return;
    }
    cm_fn_00944cff((u32)0x00a8ccd0,0,0,local_200,local_100);
    puVar9 = local_100;
    puVar8 = local_200;
    uVar10 = 0x4ba;
  }
  uVar10 = cm_fn_0090d130(puVar8,puVar9,uVar10);
  cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)0x009870e8,uVar10);
  cm_fn_005e8290(s_Error_009870e0,(const char *)(u32)&DAT_00ae24d0,0);
  DAT_00b67a34 = 0;
  return;
}

