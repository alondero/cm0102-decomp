#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT22(a, b) ((u32)(((u32)(u16)(a) << 16) | (u32)(u16)(b)))
#define DAT_009cf79c CM_GLOBAL(s32, 0x009cf79c)
#define DAT_009cf7a4 CM_GLOBAL(s32, 0x009cf7a4)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define DAT_00ae2a58 CM_GLOBAL(u32, 0x00ae2a58)
#define DAT_00dd7ef4 CM_GLOBAL(u32, 0x00dd7ef4)
#define s_E__dev_CM3_cm3_00_01_cm3_code_co_009f15f4 ((char *)0x009f15f4)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)

extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1,u32 *param_2,u16 *param_3);
extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern void cm_fn_005e8290(const char *param_1,const char *param_2,int param_3);
extern u32 CM_STDCALL cm_fn_006844b0(u32 rows, s32 n);
extern void CM_THISCALL cm_fn_00685640(int param_1,short *param_2);
extern void CM_THISCALL cm_fn_00686a70(u32 receiver, volatile s32 stack_count);
extern int cm_fn_0068a030(int param_1, char *param_2, int param_3, s16 param_4, s16 param_5, s16 param_6, u8 param_7);
extern u32 CM_THISCALL cm_fn_0085c260(u32 self, volatile u32 flags_arg, volatile u32 object_arg, volatile u32 explicit_arg);
extern char * cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void CM_STDCALL cm_fn_009462bb(u32 param_1,u32 param_2,int param_3,void *param_4);
extern u8 CM_THISCALL cm_fn_005aa680(int param_1,int param_2,int param_3);

typedef struct { void *prev; void *handler; volatile u32 state; } cmre_seh;
typedef void (CM_THISCALL *cmre_virt)(void *self, u32 *arg);

u32 CM_FASTCALL cm_fn_006835c0(int *param_1)
{
  u32 *puVar1;
  u32 uVar2;
  void *pvVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  u16 *puVar7;
  u16 uVar9;
  u32 *puVar8;
  int iVar10;
  int *piVar11;
  int iVar12;
  u32 uVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  u8 bVar17;
  cmre_seh frame;
  u8 local_289;
  int local_288;
  int *piStack_284;
  int iStack_280;
  int local_27c;
  u32 *puStack_278;
  int aiStack_224 [2];
  u16 local_21c;
  volatile u16 uStack_21a;
  int aiStack_214 [2];
  u8 local_20c [256];
  u8 local_10c [256];
  union { u8 b[0x50]; u32 a; } st;

#define local_274 ((u32 *)(void *)st.b)
#define local_26c (*(u32 *)(void *)(st.b + 0x08))
#define local_268 (*(u32 *)(void *)(st.b + 0x0c))
#define local_264 (*(u32 *)(void *)(st.b + 0x10))
#define local_260 (*(u32 *)(void *)(st.b + 0x14))
#define puStack_258 (*(u32 *)(void *)(st.b + 0x1c))
#define puStack_254 (*(u32 *)(void *)(st.b + 0x20))
#define uStack_250 (*(u16 *)(void *)(st.b + 0x24))
#define uStack_24e (*(u16 *)(void *)(st.b + 0x26))
#define sStack_24c (*(short *)(void *)(st.b + 0x28))
#define uStack_24a (*(u16 *)(void *)(st.b + 0x2a))
#define local_248 (*(u16 *)(void *)(st.b + 0x2c))
#define local_244 (*(u16 *)(void *)(st.b + 0x30))
#define local_242 (*(u16 *)(void *)(st.b + 0x32))
#define sStack_240 (*(short *)(void *)(st.b + 0x34))
#define local_23e (*(u16 *)(void *)(st.b + 0x36))
#define local_23c (*(u16 *)(void *)(st.b + 0x38))
#define local_23a (*(u8 *)(void *)(st.b + 0x3a))
#define local_239 (*(u8 *)(void *)(st.b + 0x3b))
#define local_237 (*(u8 *)(void *)(st.b + 0x3d))
#define local_236 (*(u8 *)(void *)(st.b + 0x3e))
#define uStack_235 (*(u8 *)(void *)(st.b + 0x3f))
#define bStack_234 (*(u8 *)(void *)(st.b + 0x40))
#define bStack_233 (*(u8 *)(void *)(st.b + 0x41))
#define local_232 (*(u8 *)(void *)(st.b + 0x42))
#define local_231 (*(u8 *)(void *)(st.b + 0x43))
#define local_230 (*(u8 *)(void *)(st.b + 0x44))
#define local_22f (*(u8 *)(void *)(st.b + 0x45))
#define local_22e (*(u8 *)(void *)(st.b + 0x46))
#define local_22d (*(u8 *)(void *)(st.b + 0x47))
#define local_22c (*(u8 *)(void *)(st.b + 0x48))
#define local_22b (*(u8 *)(void *)(st.b + 0x49))
#define local_22a (*(u8 *)(void *)(st.b + 0x4a))
#define local_229 (*(u8 *)(void *)(st.b + 0x4b))
#define local_228 (*(u8 *)(void *)(st.b + 0x4c))
#define local_227 (*(u16 *)(void *)(st.b + 0x4d))

  frame.state = 0xffffffff;
  frame.handler = (void *)0x0095c981;
  frame.prev = CM_FS(void *, 0);
  CM_FS(void *, 0) = &frame;
  local_288 = -1;
  uVar13 = (int)*(short *)((int)param_1 + 0x3e) & 0x80000001;
  if ((int)uVar13 < 0) {
    uVar13 = ((uVar13 - 1) | 0xfffffffe) + 1;
  }
  iVar14 = uVar13 + (int)*(short *)((int)param_1 + 0x3e);
  local_27c = iVar14;
  local_289 = 0;
  cm_fn_0054a110(&local_21c);
  frame.state = 0;
  *(u32 *)((int)param_1 + 0xdd) = 0;
  puVar1 = (u32 *)(u32)(cm_fn_00944cf1(iVar14 * 4 + 4));
  if (puVar1 == (u32 *)0x0) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_co_009f15f4,0,0,local_10c,local_20c);
    uVar2 = (u32)(cm_fn_0090d130());
    cm_fn_00944c9f((u32)(&DAT_00ae24d0),(u32)s_v_s__s__s__d_009870e8,uVar2,
                   (u32)local_10c,(u32)local_20c,0x3fc);
    cm_fn_005e8290(s_Error_009870e0,(const char *)(&DAT_00ae24d0),0);
    DAT_00b67a34 = 0;
    uVar2 = 0;
  }
  else {
    iVar16 = 1;
    puVar8 = puVar1;
    if (0 < iVar14) {
      do {
        pvVar3 = (void *)(u32)(cm_fn_00944cf1(iVar14 * 4));
        if (pvVar3 == (void *)0x0) {
          cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_co_009f15f4,0,0,local_20c,local_10c);
          uVar2 = (u32)(cm_fn_0090d130());
          cm_fn_00944c9f((u32)(&DAT_00ae24d0),(u32)s_v_s__s__s__d_009870e8,uVar2,
                         (u32)local_20c,(u32)local_10c,0x405);
          cm_fn_005e8290(s_Error_009870e0,(const char *)(&DAT_00ae24d0),0);
          DAT_00b67a34 = 0;
          cm_fn_00944c94((void *)puVar1);
          CM_FS(void *, 0) = frame.prev;
          return 0;
        }
        puVar8[1] = (u32)(pvVar3);
        iVar16 = iVar16 + 1;
        puVar8 = puVar8 + 1;
      } while (iVar16 <= iVar14);
    }
    cm_fn_006844b0((u32)(puVar1),iVar14);
    if (((int *)param_1[1] == (int *)0x0) ||
       ((iVar16 = *(int *)param_1[1], iVar16 != DAT_009cf79c && (iVar16 != DAT_009cf7a4)))) {
      cm_fn_00686a70((u32)param_1,iVar14);
    }
    local_260 = (u32)param_1[1];
    local_242 = *(u16 *)((int)param_1 + 0x3a);
    local_274[0] = *(u32 *)(u32)local_260;
    local_248 = (u16)param_1[0x10];
    local_23c = *(u16 *)((int)param_1 + 0xab);
    local_227 = local_227 & 0xfbff;
    local_23a = (u8)param_1[0x31];
    local_23e = *(u16 *)((int)param_1 + 0xdb);
    local_232 = (u8)param_1[0x11];
    local_244 = 0;
    local_231 = 0xff;
    local_230 = 0xff;
    local_22f = 0xff;
    local_22e = 0xff;
    local_22d = 0xff;
    local_22c = 0xff;
    local_22b = 0xff;
    local_22a = 0xff;
    local_229 = 0xff;
    local_228 = 0xff;
    local_26c = 0xffffffff;
    local_237 = 0;
    local_236 = 0;
    local_239 = 0xff;
    ((cmre_virt)(*((u32 **)(*param_1 + 0x7c))))((void *)param_1, local_274);
    iVar16 = *(short *)((int)param_1 + 0xa9) * 0x41;
    iVar4 = *(int *)((int)param_1 + 0xba) + iVar16;
    uVar2 = (u32)cm_fn_00549ef0((void *)aiStack_224,
        CONCAT22((short)((u32)iVar16 >> 0x10),*(u16 *)(iVar4 + -0x41)),
        (u32)(*(short *)(iVar4 + -0x3f) + (short)param_1[0x10]));
    *(u8 *)&frame.state = 1;
    cm_fn_00417c10((u32 *)&local_21c,(u32 *)aiStack_214,(u16 *)(u32)uVar2);
    iVar16 = *(volatile int *)((u32)param_1 + 0xea);
    *(u8 *)&frame.state = 0;
    uStack_24e = local_21c;
    uStack_250 = uStack_21a;
    if ((iVar16 == 0) ||
       ((int)(short)param_1[0x10] != (int)(u32)*(u16 *)(iVar16 + 2))) {
      uVar13 = (u32)*(u16 *)(param_1 + 0xf);
      iVar14 = iVar14 + -1;
      iVar16 = (short)*(u16 *)(param_1 + 0xf) * iVar14;
      iStack_280 = 1;
      if (0 < iVar16) {
        do {
          iVar4 = iStack_280;
          local_288 = cm_fn_0068a030(local_288,(char *)&local_289,*(u32 *)param_1[1],
                                   *(u16 *)((int)param_1 + 0x3e),uVar13,
                                   CONCAT22((short)((u32)iVar16 >> 0x10),
                                            *(u16 *)((int)param_1 + 0xa9)),
                                   *(u16 *)((int)param_1 + 0xd9));
          uVar9 = (u16)((u32)(iVar4 / iVar14) >> 0x10);
          iVar4 = iVar4 % iVar14;
          if (iVar4 == 0) {
            iVar4 = iVar14;
          }
          sStack_240 = (short)local_288;
          if (0 < *(short *)((int)param_1 + 0x3e)) {
            piStack_284 = (int *)(puVar1 + 1);
            iVar14 = 0;
            do {
              iVar16 = *piStack_284;
              iVar12 = *(int *)(iVar16 + iVar4 * 4);
              if (iVar12 != 0) {
                uVar13 = (iStack_280 + -1) / (local_27c + -1) & 0x80000001;
                bVar17 = uVar13 == 0;
                if ((int)uVar13 < 0) {
                  bVar17 = ((uVar13 - 1) | 0xfffffffe) == 0xffffffff;
                }
                iVar10 = iVar14;
                if (bVar17) {
                  if (iVar12 < 1) {
                    uVar13 = ((int)(short)param_1[0x10] + (int)(char)param_1[0x11]) & 0x80000001;
                    if ((int)uVar13 < 0) {
                      uVar13 = ((uVar13 - 1) | 0xfffffffe) + 1;
                    }
                    if (uVar13 == 0) {
LAB_00683c0c:
                      iVar12 = -iVar12;
                      goto LAB_00683c13;
                    }
                    iVar16 = -iVar12 + -1;
                    iVar5 = puVar1[-iVar12];
                  }
                  else {
                    uVar13 = ((int)(short)param_1[0x10] + (int)(char)param_1[0x11]) & 0x80000001;
                    bVar17 = uVar13 == 0;
                    if ((int)uVar13 < 0) {
                      bVar17 = ((uVar13 - 1) | 0xfffffffe) == 0xffffffff;
                    }
                    if (bVar17) {
LAB_00683b90:
                      iVar5 = puVar1[iVar12];
                      iVar16 = iVar12 + -1;
                    }
                    else {
LAB_00683c13:
                      iVar5 = puVar1[iVar12];
                      iVar16 = iVar14;
                      iVar10 = iVar12 + -1;
                    }
                  }
                }
                else {
                  if (0 < iVar12) {
                    uVar13 = ((int)(short)param_1[0x10] + (int)(char)param_1[0x11]) & 0x80000001;
                    bVar17 = uVar13 == 0;
                    if ((int)uVar13 < 0) {
                      bVar17 = ((uVar13 - 1) | 0xfffffffe) == 0xffffffff;
                    }
                    if (!bVar17) goto LAB_00683b90;
                    goto LAB_00683c13;
                  }
                  uVar13 = ((int)(short)param_1[0x10] + (int)(char)param_1[0x11]) & 0x80000001;
                  if ((int)uVar13 < 0) {
                    uVar13 = ((uVar13 - 1) | 0xfffffffe) + 1;
                  }
                  if (uVar13 != 0) goto LAB_00683c0c;
                  iVar16 = -iVar12 + -1;
                  iVar5 = puVar1[-iVar12];
                }
                *(u32 *)(iVar5 + iVar4 * 4) = 0;
                if ((((-1 < iVar16) &&
                     (iVar12 = *(short *)((int)param_1 + 0x3e) + -1, iVar16 <= iVar12)) &&
                    (-1 < iVar10)) && (iVar10 <= iVar12)) {
                  iVar12 = *(int *)((int)param_1 + 0xb1);
                  if (param_1[9] == -1) {
                    puVar8 = (u32 *)(iVar16 * 0x3b + iVar12);
                    puStack_258 = *puVar8;
                    puStack_254 = *(u32 *)(iVar12 + iVar10 + iVar10 * 0x3a);
                    bStack_234 = *(char *)(puVar8 + 0xe) == '\0';
                    bStack_233 = *(char *)(iVar12 + iVar10 + iVar10 * 0x3a + 0x38) == '\0';
                  }
                  else {
                    puVar8 = (u32 *)(iVar12 + iVar10 + iVar10 * 0x3a);
                    piVar6 = *(int **)(*(int *)(iVar12 + iVar10 + iVar10 * 0x3a) + 0x69);
                    if ((piVar6 == (int *)0x0) || (*piVar6 != param_1[9])) {
                      iVar12 = *(int *)((int)param_1 + 0xb1) + iVar16;
                      puStack_258 = *(u32 *)(iVar12 + iVar16 * 0x3a);
                      puStack_254 = *puVar8;
                      bStack_234 = *(char *)(iVar12 + iVar16 * 0x3a + 0x38) == '\0';
                      bStack_233 = *(char *)(puVar8 + 0xe) == '\0';
                    }
                    else {
                      puStack_258 = *puVar8;
                      iVar12 = *(int *)((int)param_1 + 0xb1) + iVar16;
                      puStack_254 = *(u32 *)(iVar12 + iVar16 * 0x3a);
                      bStack_234 = *(char *)(puVar8 + 0xe) == '\0';
                      bStack_233 = *(char *)(iVar12 + iVar16 * 0x3a + 0x38) == '\0';
                    }
                  }
                  if (puStack_258 == 0) {
                    local_268 = 0xffffffff;
                  }
                  else {
                    local_268 = *(u32 *)(u32)puStack_258;
                  }
                  if (puStack_254 == 0) {
                    local_264 = 0xffffffff;
                  }
                  else {
                    local_264 = *(u32 *)(u32)puStack_254;
                  }
                  iVar16 = local_288 * 0x41;
                  cm_fn_0085c260(DAT_00dd7ef4,*(u32 *)(iVar16 + 0x3d + *(int *)((int)param_1 + 0xba)),
                                 (u32)local_274,(u32)param_1[9]);
                  puVar7 = (u16 *)(iVar16 + *(int *)((int)param_1 + 0xba));
                  sStack_24c = puVar7[1] + (short)param_1[0x10];
                  uStack_24a = *puVar7;
                  uStack_235 = *(u8 *)(puVar7 + 2);
                  if ((int)sStack_240 == *(short *)((int)param_1 + 0xa9) + -1) {
                    local_227 = (local_227 & 0xfcff) | 0x800;
                  }
                  else {
                    local_227 = local_227 & 0xf4ff;
                  }
                  iVar16 = cm_fn_005aa680((int)DAT_00ae2a58,(int)(local_274),1);
                }
              }
              uVar9 = (u16)((u32)iVar16 >> 0x10);
              piStack_284 = piStack_284 + 1;
              iVar16 = iVar14 + 2;
              iVar14 = iVar14 + 1;
            } while (iVar16 <= *(short *)((int)param_1 + 0x3e));
          }
          uVar13 = CONCAT22(uVar9,(short)param_1[0xf]);
          iVar14 = local_27c + -1;
          iVar16 = (short)param_1[0xf] * iVar14;
          iStack_280 = iStack_280 + 1;
        } while (iStack_280 <= iVar16);
      }
    }
    else {
      piStack_284 = (int *)0x0;
      if (0 < *(short *)((int)param_1 + 0xa9)) {
        local_288 = 0;
        do {
          iVar16 = (int)*(short *)((int)param_1 + 0x3e);
          iStack_280 = 0;
          iVar14 = iVar16 / 2;
          if (0 < iVar14) {
            do {
              iVar12 = iStack_280;
              iVar4 = local_288;
              sStack_240 = (short)(u32)(u32)piStack_284;
              iVar5 = (iVar14 * (int)piStack_284 + iStack_280) * 0x10 +
                      *(int *)((int)param_1 + 0xea);
              iVar15 = 0;
              iVar10 = -1;
              iVar14 = -1;
              if (0 < iVar16) {
                aiStack_214[0] = *(int *)(iVar5 + 8);
                aiStack_224[0] = *(int *)(iVar5 + 0xc);
                puStack_278 = (u32 *)(u32)*(u32 *)((int)param_1 + 0xb1);
                do {
                  if (aiStack_214[0] == *(int *)*puStack_278) {
                    iVar10 = iVar15;
                  }
                  if (aiStack_224[0] == *(int *)*puStack_278) {
                    iVar14 = iVar15;
                  }
                  iVar15 = iVar15 + 1;
                  puStack_278 = (u32 *)((int)puStack_278 + 0x3b);
                } while (iVar15 < iVar16);
              }
              piVar11 = (int *)(iVar10 * 0x3b + *(int *)((int)param_1 + 0xb1));
              piVar6 = (int *)(*(int *)((int)param_1 + 0xb1) + iVar14 * 0x3b);
              puStack_258 = (u32)*piVar11;
              puStack_254 = (u32)*piVar6;
              bStack_234 = (char)piVar11[0xe] == '\0';
              bStack_233 = (char)piVar6[0xe] == '\0';
              if (puStack_258 == 0) {
                local_268 = 0xffffffff;
              }
              else {
                local_268 = *(u32 *)(u32)puStack_258;
              }
              if (puStack_254 == 0) {
                local_264 = 0xffffffff;
              }
              else {
                local_264 = *(u32 *)(u32)puStack_254;
              }
              cm_fn_0085c260(DAT_00dd7ef4,*(u32 *)(local_288 + 0x3d + *(int *)((int)param_1 + 0xba)),
                             (u32)local_274,(u32)param_1[9]);
              puVar7 = (u16 *)(*(int *)((int)param_1 + 0xba) + iVar4);
              sStack_24c = puVar7[1] + (short)param_1[0x10];
              uStack_24a = *puVar7;
              uStack_235 = *(u8 *)(puVar7 + 2);
              if (((int)sStack_240 == *(short *)((int)param_1 + 0xa9) + -1) ||
                 (*(char *)((int)puVar7 + 0xb) == '\x04')) {
                local_227 = (local_227 & 0xfcff) | 0x800;
              }
              else {
                local_227 = local_227 & 0xf4ff;
              }
              cm_fn_005aa680((int)DAT_00ae2a58,(int)(local_274),1);
              iVar16 = (int)*(short *)((int)param_1 + 0x3e);
              iStack_280 = iVar12 + 1;
              iVar14 = iVar16 / 2;
            } while (iStack_280 < iVar14);
          }
          piStack_284 = (int *)((int)piStack_284 + 1);
          local_288 = local_288 + 0x41;
        } while ((int)piStack_284 < (int)*(short *)((int)param_1 + 0xa9));
      }
      iVar14 = *(int *)((int)param_1 + 0xea);
      if (iVar14 != 0) {
        cm_fn_009462bb(iVar14,0x10,*(u32 *)(iVar14 + -4),(void *)0x005a53c0);
        cm_fn_00944c94((void *)(iVar14 + -4));
      }
      *(u32 *)((int)param_1 + 0xea) = 0;
    }
    iVar14 = local_27c;
    puVar8 = puVar1;
    if (0 < local_27c) {
      do {
        cm_fn_00944c94((void *)(puVar8[1]));
        iVar14 = iVar14 + -1;
        puVar8 = puVar8 + 1;
      } while (iVar14 != 0);
    }
    cm_fn_00944c94((void *)puVar1);
    iVar14 = 0;
    if (0 < *(short *)((int)param_1 + 0xa9)) {
      iVar16 = 0;
      do {
        iVar4 = *(int *)((int)param_1 + 0xba);
        if (*(char *)(iVar16 + 0xb + iVar4) == '\x03') {
          cm_fn_00549ef0((void *)aiStack_224,
              CONCAT22((short)((u32)iVar4 >> 0x10),*(u16 *)(iVar16 + iVar4)),
              (u32)(((u16 *)(iVar16 + iVar4))[1] + (short)param_1[0x10]));
          *(u8 *)&frame.state = 2;
          cm_fn_00685640((int)param_1,(short *)aiStack_224);
          *(u8 *)&frame.state = 0;
        }
        iVar14 = iVar14 + 1;
        iVar16 = iVar16 + 0x41;
      } while (iVar14 < *(short *)((int)param_1 + 0xa9));
    }
    uVar2 = 1;
  }
  CM_FS(void *, 0) = frame.prev;
  return uVar2;
}
