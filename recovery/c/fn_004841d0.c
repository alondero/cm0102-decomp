/* WIP: five failed iterations, all compile failures; current helper prototype/type mismatches remain at 0053d980/0053c190. No semantic run completed.
   Draft for preferred-image VA 0x004841d0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ad9db0 CM_GLOBAL(int * volatile, 0x00ad9db0)
#define DAT_00ae1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define DAT_00de1d08 CM_GLOBAL(volatile u32, 0x00de1d08)
#define DAT_00de2f70 CM_GLOBAL(volatile u32, 0x00de2f70)
#define DAT_00ad9db4 CM_GLOBAL(volatile u8, 0x00ad9db4)
#define DAT_00ad9db5 CM_GLOBAL(volatile u8, 0x00ad9db5)
#define DAT_00adab33 CM_GLOBAL(volatile u8, 0x00adab33)
#define DAT_00adab34 CM_GLOBAL(volatile int, 0x00adab34)
#define DAT_00adab38 CM_GLOBAL(int * volatile, 0x00adab38)
#define DAT_00adab3c CM_GLOBAL(int * volatile, 0x00adab3c)
#define DAT_00adab44 CM_GLOBAL(volatile u32, 0x00adab44)
#define DAT_00adab4a CM_GLOBAL(volatile u8, 0x00adab4a)
#define DAT_00adadf0 CM_GLOBAL(volatile u32, 0x00adadf0)
#define DAT_00ae2364 CM_GLOBAL(volatile int, 0x00ae2364)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00ae2c90 CM_GLOBAL(volatile u32, 0x00ae2c90)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define DAT_00b74c26 CM_GLOBAL(volatile u32, 0x00b74c26)
#define DAT_00b77c7a CM_GLOBAL(volatile short, 0x00b77c7a)
#define s_E__dev_CM3_cm3_00_01_cm3_code_cl_0098e598 ((char *)0x0098e598)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern u8 cm_fn_0045cbe0(int param_1,int param_2,int *param_3,int param_4);
extern u8 CM_THISCALL cm_fn_004cc130(int *self,int *param_2,int *param_3);
extern int CM_THISCALL cm_fn_004cf5a0(int *self,int *param_2);
extern u8 cm_fn_0053b900(int *param_1);
extern u32 cm_fn_0053c190(int *param_1,int param_2,int param_3,int param_4,int param_5);
extern u32 cm_fn_0053d980(int param_1,int *param_2,int param_3);
extern int cm_fn_00540a50(u32 *param_1,u32 *param_2,int param_3);
extern u16 cm_fn_005b7d20(int param_1);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 cm_fn_00600c90(int param_1,int param_2,char param_3);
extern void CM_FASTCALL cm_fn_0068d080(u16 *param_1);
extern u32 CM_THISCALL cm_fn_0068d0d0(short *param_1,u32 param_2,u32 param_3);
extern u32 CM_CDECL cm_fn_00775790(int *param_1);
extern u32 CM_THISCALL cm_fn_008972c0(int param_1,int param_2,int *param_3,u32 param_4);
extern char * CM_CDECL cm_fn_0090d130(void);
extern void CM_THISCALL cm_fn_0090d4f0(int param_1,int param_2);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_00945333(u32 *param_1,u32 param_2,u32 param_3,void *param_4);

u32
cm_fn_004841d0(int *param_1,int param_2,u32 param_3,u8 param_4,u8 param_5,
            u8 param_6,u8 param_7,u32 param_8,int *param_9,int *param_10,
            u32 param_11,int param_12,int *param_13,int param_14)

{
  u8 bVar1;
  u8 bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  u32 uVar6;
  int iVar7;
  u32 uVar8;
  int *piVar9;
  u8 *puVar10;
  char local_2d9;
  int *local_2d8;
  int local_2d4;
  int local_2cc;
  int local_2c8 [50];
  u8 local_200 [256];
  u8 local_100 [256];
  
  if ((param_1 == (int *)0x0) || (param_9 == (int *)0x0)) {
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_cl_0098e598,0,0,local_200,local_100);
    uVar8 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,
                   uVar8,(u32)local_200,(u32)local_100,0x503f);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return 0;
  }
  iVar4 = cm_fn_00540a50((u32 *)param_1,0,1);
  cm_fn_0068d080((u16 *)param_9);
  {
    u32 receiver = DAT_00de2f70;
    cm_fn_0090d4f0((int *)receiver,param_1);
  }
  local_2d9 = '\0';
  if (param_13 == (int *)0x0) {
    param_13 = (int *)((int)param_1 + 0xd7);
  }
  local_2d4 = 0x32;
  local_2d8 = param_13;
  do {
    piVar9 = (int *)*local_2d8;
    if ((piVar9 != (int *)0x0) && (iVar7 = *(int *)((int)piVar9 + 0x61), iVar7 != 0)) {
      iVar5 = cm_fn_0053b900(param_1);
      if ((iVar5 != 0) &&
         (iVar5 = cm_fn_0053d980(piVar9,*(u32 *)((int)param_1 + 0x53),1), iVar5 != 0)) {
        cm_fn_0053c190(param_1,piVar9,0,1,0);
      }
      iVar5 = cm_fn_0053b900(param_1);
      if (((((iVar5 != 0) || (iVar5 = cm_fn_00600c90((int)(u32)param_1,0,1), iVar5 != 0)) ||
           (*piVar9 < DAT_00ae2364)) || ((*(u8 *)((int)piVar9 + 0x5e) & 2) == 0)) &&
         ((((param_8 & 1) != 0 && ((param_8 & 0x20) != 0)) ||
          (((((uVar6 = cm_fn_005b7d20(iVar7), (param_8 & 2) == 0 || ((uVar6 & 1) != 0)) &&
             (((param_8 & 4) == 0 || ((uVar6 & 6) != 0)))) &&
            ((((param_8 & 8) == 0 || ((uVar6 & 0x38) != 0)) &&
             (((((param_8 & 0x10) == 0 || ((uVar6 & 0x40) != 0)) &&
               (((param_8 & 0x40) == 0 || ((uVar6 & 0x80) != 0)))) &&
              (((param_8 & 0x80) == 0 || ((uVar6 & 0x200) != 0)))))))) &&
           (((param_8 & 0x100) == 0 || ((uVar6 & 0x800) != 0)))))))) {
        if ((param_8 & 0x400) == 0) {
          iVar7 = cm_fn_00600c90((int)(u32)param_1,0,1);
          if (iVar7 == 0) {
            bVar1 = false;
            if (param_10 != (int *)0x0) {
              cVar3 = '\0';
              do {
                if (bVar1) break;
                if (*(int *)((int)param_10 +
                            cVar3 * 0x49 + 400 + (u32)(*param_1 != *param_10) * 0x625) == *piVar9)
                {
                  bVar1 = true;
                }
                cVar3 = cVar3 + '\x01';
              } while (cVar3 < '\x14');
            }
          }
          else {
            bVar2 = cm_fn_008972c0((int)DAT_00de1d08,(int)param_1,(int *)param_9,(u32)param_2);
            bVar1 = bVar2 < 0x80;
          }
          iVar7 = cm_fn_0053b900(param_1);
          if (iVar7 != 0) {
            if (param_2 == 0) {
              bVar2 = *(u8 *)((int)piVar9 + 0x5e) & 8;
            }
            else {
              bVar2 = *(u8 *)((int)piVar9 + 0x5e) & 4;
            }
            if ((bVar2 != 0) && (!bVar1)) goto LAB_0048455a;
          }
          if (((param_8 & 0x200) != 0) &&
             ((iVar7 = cm_fn_0053b900(param_1), iVar7 == 0 &&
              (iVar7 = cm_fn_00600c90((int)(u32)param_1,0,1), iVar7 != 0)))) {
            if (param_2 == 0) {
              cVar3 = cm_fn_004cc130((int *)DAT_00ae1050,piVar9,param_1);
              if (cVar3 == '\x02') {
LAB_00484453:
                if (!bVar1) goto LAB_0048455a;
              }
            }
            else {
              cVar3 = cm_fn_004cc130((int *)DAT_00ae1050,piVar9,param_1);
              if (cVar3 == '\x01') goto LAB_00484453;
            }
          }
        }
        if (((((param_8 & 0x800) != 0) ||
             (((iVar7 = cm_fn_0045cbe0((int)piVar9,param_11,param_1,(int)(u32)&DAT_00ae2c90), (param_8 & 0x1000) == 0
               || (iVar7 != 0)) && (((param_8 & 0x2000) == 0 || (iVar7 == 0)))))) &&
            ((((((param_8 & 0x4000) != 0 || ((param_8 & 0x8000) == 0)) ||
               (*(char *)((int)piVar9 + 0x6d) == '\x02')) || ((char)piVar9[6] < '\x14')) ||
             ((*(int *)((int)param_1 + 0x1db) == -1 &&
              ((iVar4 == 0 || (*(int *)(iVar4 + 0x1db) == -1)))))))) &&
           (((((param_8 & 0x10000) != 0 || (((param_8 & 0x20000) == 0 || (*piVar9 < DAT_00ae2364))))
             && (((param_8 & 0x40000) != 0 ||
                 (((param_8 & 0x80000) == 0 || (*(char *)((int)piVar9 + 0x6d) != '\b')))))) &&
            ((((param_8 & 0x100000) != 0 || ((param_8 & 0x200000) == 0)) ||
             ((((*(volatile int **)((int)piVar9 + 0x39) == param_1 ||
                (iVar7 = cm_fn_00540a50((u32 *)param_1,0,1), *(volatile int *)((int)piVar9 + 0x39) == iVar7)) &&
               (iVar7 = cm_fn_004cf5a0((int *)DAT_00ae1050,piVar9), iVar7 != 0)) &&
              (iVar7 = cm_fn_004cf5a0((int *)DAT_00ae1050,piVar9), (*(u8 *)(iVar7 + 0x35) & 0x3f) != 5)))))))) {
          iVar7 = (int)local_2d9;
          local_2d9 = local_2d9 + '\x01';
          local_2c8[iVar7] = (int)piVar9;
        }
      }
    }
LAB_0048455a:
    local_2d8 = local_2d8 + 1;
    local_2d4 = local_2d4 + -1;
  } while (local_2d4 != 0);
  if (local_2d9 == '\0') {
    return 1;
  }
  DAT_00ad9db0 = param_1;
  DAT_00adab3c = (int *)cm_fn_00540a50((u32 *)param_1,(u32 *)&local_2cc,1);
  if ((local_2cc != 0) || (DAT_00adab3c == (int *)0x0)) {
    DAT_00adab3c = DAT_00ad9db0;
  }
  DAT_00adab4a = param_5;
  DAT_00adab33 = param_4;
  DAT_00adab38 = param_10;
  DAT_00ad9db4 = param_7;
  DAT_00adab34 = param_2;
  DAT_00ad9db5 = param_6;
  DAT_00adadf0 = (&DAT_00b74c26)[DAT_00b77c7a * 0xc0];
  DAT_00adab44 = param_11;
  if ((param_3 & 1) == 0) {
    if ((param_3 & 2) == 0) {
      if ((param_3 & 4) == 0) {
        if ((param_3 & 8) == 0) {
          if (((param_3 & 0x10) == 0) ||
              ((CM_GLOBAL(volatile u32,0x00b74318),
                iVar4 = cm_fn_00775790(param_1)), iVar4 != 0)) {
            if ((param_3 & 0x10) == 0) {
              if ((param_3 & 0x20) == 0) {
                if ((param_3 & 0x40) == 0) {
                  if ((param_3 & 0x80) == 0) {
                    if ((param_3 & 0x100) == 0) {
                      if ((param_3 & 0x200) == 0) {
                        if ((param_3 & 0x8000000) == 0) {
                          if ((param_3 & 0x4000000) == 0) {
                            if ((param_3 & 0x400) == 0) {
                              if ((param_3 & 0x800) == 0) {
                                if ((param_3 & 0x1000) == 0) {
                                  if (((param_3 & 0x2000) == 0) ||
                                     ((CM_GLOBAL(volatile u32,0x00b74318),
                                       iVar4 = cm_fn_00775790(param_1)), iVar4 != 0)) {
                                    if ((param_3 & 0x2000) == 0) {
                                      if ((param_3 & 0x4000) == 0) {
                                        if ((param_3 & 0x8000) == 0) {
                                          if ((param_3 & 0x10000) == 0) {
                                            if ((param_3 & 0x20000) == 0) {
                                              if ((param_3 & 0x40000) == 0) {
                                                if ((param_3 & 0x20000000) == 0) {
                                                  if ((param_3 & 0x80000) == 0) {
                                                    if ((param_3 & 0x10000000) == 0) {
                                                      if ((param_3 & 0x100000) == 0) {
                                                        if ((param_3 & 0x200000) == 0) {
                                                          if ((param_3 & 0x400000) == 0) {
                                                            if ((param_3 & 0x800000) == 0) {
                                                              if ((param_3 & 0x2000000) == 0)
                                                              goto LAB_0048486d;
                                                              puVar10 = (u8 *)0x00485d20;
                                                            }
                                                            else {
                                                              puVar10 = (u8 *)0x00487470;
                                                            }
                                                          }
                                                          else {
                                                            puVar10 = (u8 *)0x00487360;
                                                          }
                                                        }
                                                        else {
                                                          puVar10 = (u8 *)0x00487250;
                                                        }
                                                      }
                                                      else {
                                                        puVar10 = (u8 *)0x00487140;
                                                      }
                                                    }
                                                    else {
                                                      puVar10 = (u8 *)0x00487030;
                                                    }
                                                  }
                                                  else {
                                                    puVar10 = (u8 *)0x00486f80;
                                                  }
                                                }
                                                else {
                                                  puVar10 = (u8 *)0x00486e60;
                                                }
                                              }
                                              else {
                                                puVar10 = (u8 *)0x00486c70;
                                              }
                                            }
                                            else {
                                              puVar10 = (u8 *)0x00486af0;
                                            }
                                          }
                                          else {
                                            puVar10 = (u8 *)0x00486a30;
                                          }
                                        }
                                        else {
                                          puVar10 = (u8 *)0x00486970;
                                        }
                                      }
                                      else {
                                        puVar10 = (u8 *)0x004868c0;
                                      }
                                    }
                                    else {
                                      puVar10 = (u8 *)0x00485960;
                                    }
                                  }
                                  else {
                                    puVar10 = (u8 *)0x00486760;
                                  }
                                }
                                else {
                                  puVar10 = (u8 *)0x00486760;
                                }
                              }
                              else {
                                puVar10 = (u8 *)0x00486620;
                              }
                            }
                            else {
                              puVar10 = (u8 *)0x004864e0;
                            }
                          }
                          else {
                            puVar10 = (u8 *)0x004862c0;
                          }
                        }
                        else {
                          puVar10 = (u8 *)0x004861e0;
                        }
                      }
                      else {
                        puVar10 = (u8 *)0x004863c0;
                      }
                    }
                    else {
                      puVar10 = (u8 *)0x004860e0;
                    }
                  }
                  else {
                    puVar10 = (u8 *)0x00485fd0;
                  }
                }
                else {
                  puVar10 = (u8 *)0x00485ed0;
                }
              }
              else {
                puVar10 = (u8 *)0x00485ba0;
              }
            }
            else {
              puVar10 = (u8 *)0x00485960;
            }
          }
          else {
            puVar10 = (u8 *)0x00485a60;
          }
        }
        else {
          puVar10 = (u8 *)0x00485830;
        }
      }
      else {
        puVar10 = (u8 *)0x004857a0;
      }
    }
    else {
      puVar10 = (u8 *)0x004856a0;
    }
  }
  else if (param_14 == 0) {
    puVar10 = (u8 *)0x00485470;
  }
  else {
    puVar10 = (u8 *)0x00485630;
  }
  cm_fn_00945333((u32 *)local_2c8,(u32)(s32)local_2d9,4,puVar10);
LAB_0048486d:
  if (param_12 != 0) {
    iVar4 = (int)local_2d9;
    if (param_12 <= local_2d9) {
      iVar4 = param_12;
    }
    cm_fn_00945333((u32 *)local_2c8,(u32)iVar4,4,(u8 *)0x004857a0);
  }
  if ((param_3 & 0x1000000) == 0) {
    if ('\0' < local_2d9) {
      piVar9 = local_2c8;
      iVar4 = (int)local_2d9;
      do {
        cm_fn_0068d0d0((short *)param_9,(u32)piVar9,4);
        piVar9 = piVar9 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  }
  else {
    local_2d9 = local_2d9 + -1;
    if (-1 < local_2d9) {
      piVar9 = local_2c8 + local_2d9;
      iVar4 = local_2d9 + 1;
      do {
        cm_fn_0068d0d0((short *)param_9,(u32)piVar9,4);
        piVar9 = piVar9 + -1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      return 1;
    }
  }
  return 1;
}
