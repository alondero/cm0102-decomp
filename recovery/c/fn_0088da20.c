/* Preferred-image VA 0x0088da20, image base 0x00400000, CM0102 3.9.68.
   Binary SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Checked against listing; cmre test decides acceptance. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define DAT_009cf4a8 CM_GLOBAL(int, 0x009cf4a8)
#define DAT_009cf5ec CM_GLOBAL(int, 0x009cf5ec)
#define DAT_009cf5f0 CM_GLOBAL(int, 0x009cf5f0)
#define DAT_009cf6ac CM_GLOBAL(int, 0x009cf6ac)
#define DAT_009cf7c8 CM_GLOBAL(int, 0x009cf7c8)
#define DAT_00ae235c CM_GLOBAL(int, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define LAB_0053fd40 CM_GLOBAL(u8, 0x0053fd40)
#define s_path_00a8b4a0 ((char *)0x00a8b4a0)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_fmt_009870e8 ((char *)0x009870e8)

extern u32 CM_CDECL cm_fn_004afb40(u32 array, u32 raw_count);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 bound);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern unsigned long long CM_CDECL cm_fn_00945333(volatile u32 base_arg, volatile u32 count_arg, volatile u32 width_arg, volatile u32 compare_arg);

u32 CM_FASTCALL cm_fn_0088da20(int param_1)
{
  int iVar1;
  void *pvVar2;
  int iVar3;
  char cVar4;
  short sVar5;
  int iVar6;
  u32 uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  u8 bVar11;
  u8 *puVar12;
  u8 *puVar13;
  u32 uVar14;
  u32 uVarResult;
  char local_219;
  u32 local_214;
  int local_20c;
  u8 scratch[0x200];   /* L+0x1c .. L+0x21b; buffers are 0x100 apart as in listing */
#define local_200 scratch
#define local_100 (scratch + 0x100)

  *(u16 *)(param_1 + 0xb6) = 0xf4;
  iVar9 = 0;
  sVar5 = 0;
  pvVar2 = (void *)(u32)(cm_fn_00944e46(0x3d0));
  *(void **)(param_1 + 0xb2) = pvVar2;
  if (pvVar2 == (void *)0x0) {
    cm_fn_00944cff((u32)(s_path_00a8b4a0),0,0,(void *)local_100,(void *)local_200);
    puVar13 = local_200;
    puVar12 = local_100;
    uVar14 = 0x29e;
  }
  else {
    iVar8 = 0;
    iVar3 = DAT_00ae23b4;
    iVar6 = DAT_00ae235c;
    if (0 < DAT_00ae235c) {
      do {
        if ((*(int **)(iVar3 + 0x57) != (int *)0x0) && (**(int **)(iVar3 + 0x57) == DAT_009cf7c8)) {
          iVar9 = iVar9 + 1;
          *(int *)(*(int *)(param_1 + 0xb2) + -4 + iVar9 * 4) = iVar3;
          iVar6 = DAT_00ae235c;
        }
        iVar8 = iVar8 + 1;
        iVar3 = iVar3 + 0x245;
      } while (iVar8 < iVar6);
    }
    iVar8 = 0;
    iVar3 = DAT_00ae23b4;
    if (0 < iVar6) {
      do {
        if ((*(int **)(iVar3 + 0x53) != (int *)0x0) &&
            (**(int **)(iVar3 + 0x53) == DAT_009cf4a8) &&
            (*(int **)(iVar3 + 0x57) != (int *)0x0) &&
            (**(int **)(iVar3 + 0x57) == DAT_009cf6ac) &&
            (iVar9 < *(short *)(param_1 + 0xb6) + -0x1e)) {
          iVar9 = iVar9 + 1;
          *(int *)(*(int *)(param_1 + 0xb2) + -4 + iVar9 * 4) = iVar3;
          iVar6 = DAT_00ae235c;
        }
        iVar8 = iVar8 + 1;
        iVar3 = iVar3 + 0x245;
      } while (iVar8 < iVar6);
    }
    iVar8 = 0;
    iVar3 = DAT_00ae23b4;
    if (0 < iVar6) {
      do {
        if (((*(int **)(iVar3 + 0x57) != (int *)0x0) && (**(int **)(iVar3 + 0x57) == DAT_009cf5f0)) &&
           (iVar9 < *(short *)(param_1 + 0xb6))) {
          iVar9 = iVar9 + 1;
          *(int *)(*(int *)(param_1 + 0xb2) + -4 + iVar9 * 4) = iVar3;
          iVar6 = DAT_00ae235c;
        }
        iVar8 = iVar8 + 1;
        iVar3 = iVar3 + 0x245;
      } while (iVar8 < iVar6);
    }
    cm_fn_004afb40(*(int *)(param_1 + 0xb2) + -0x40 + iVar9 * 4,0x10);
    iVar6 = 0;
    iVar3 = DAT_00ae23b4;
    if (0 < DAT_00ae235c) {
      do {
        if (((*(int **)(iVar3 + 0x57) != (int *)0x0) && (**(int **)(iVar3 + 0x57) == DAT_009cf5ec)) &&
           (iVar9 < *(short *)(param_1 + 0xb6))) {
          iVar9 = iVar9 + 1;
          *(int *)(*(int *)(param_1 + 0xb2) + -4 + iVar9 * 4) = iVar3;
        }
        iVar6 = iVar6 + 1;
        iVar3 = iVar3 + 0x245;
      } while (iVar6 < DAT_00ae235c);
    }
    cm_fn_00945333(*(u32 *)(param_1 + 0xb2),0xe8,4,(u32)(&LAB_0053fd40));
    pvVar2 = (void *)(u32)(cm_fn_00944e46((int)*(short *)(param_1 + 0xb6) << 2));
    if (pvVar2 != (void *)0x0) {
      local_214 = 0;
      local_20c = 8;
      do {
        uVar7 = local_214 & 0x80000001;
        bVar11 = uVar7 == 0;
        if ((int)uVar7 < 0) {
          bVar11 = ((uVar7 - 1) | 0xfffffffe) == 0xffffffff;
        }
        if (bVar11) {
          local_219 = '\x1e';
          cVar4 = '\x02';
        }
        else {
          local_219 = '\x1f';
          cVar4 = '\x01';
        }
        iVar9 = (int)local_219;
        iVar3 = (int)cVar4;
        iVar8 = 0;
        iVar6 = 0;
        if (0 < iVar9 - iVar3) {
          do {
            if (*(short *)(param_1 + 0xb6) <= iVar6) break;
            iVar1 = *(int *)(*(int *)(param_1 + 0xb2) + iVar6 * 4);
            if (iVar1 != 0) {
              iVar10 = sVar5 + iVar8;
              iVar8 = iVar8 + 1;
              *(int *)((int)pvVar2 + iVar10 * 4) = iVar1;
              *(u32 *)(*(int *)(param_1 + 0xb2) + iVar6 * 4) = 0;
            }
            iVar6 = iVar6 + 1;
          } while (iVar8 < iVar9 - iVar3);
        }
        for (; iVar3 != 0; iVar3 = iVar3 + -1) {
          iVar8 = (int)(u32)(cm_fn_0090cfc0(0xc));
          iVar8 = iVar8 + 0xe8;
          iVar6 = *(int *)(*(int *)(param_1 + 0xb2) + iVar8 * 4);
          while (iVar6 == 0) {
            iVar8 = (int)(u32)(cm_fn_0090cfc0(0xc));
            iVar8 = iVar8 + 0xe8;
            iVar6 = *(int *)(*(int *)(param_1 + 0xb2) + iVar8 * 4);
          }
          iVar6 = sVar5 + iVar9;
          iVar9 = iVar9 + -1;
          *(u32 *)((int)pvVar2 + iVar6 * 4 + -4) = *(u32 *)(*(int *)(param_1 + 0xb2) + iVar8 * 4);
          *(u32 *)(*(int *)(param_1 + 0xb2) + iVar8 * 4) = 0;
        }
        sVar5 = sVar5 + local_219;
        local_214 = local_214 + 1;
        local_20c = local_20c + -1;
        if (local_20c == 0) {
          cm_fn_009452ca(*(u32 *)(param_1 + 0xb2));
          *(void **)(param_1 + 0xb2) = pvVar2;
          return 1;
        }
      } while( true );
    }
    cm_fn_00944cff((u32)(s_path_00a8b4a0),0,0,(void *)local_200,(void *)local_100);
    puVar13 = local_100;
    puVar12 = local_200;
    uVar14 = 0x2d9;
  }
  uVarResult = (u32)(cm_fn_0090d130());
  cm_fn_00944c9f((u32)(&DAT_00ae24d0),(u32)(s_fmt_009870e8),uVarResult,(u32)puVar12,(u32)puVar13,uVar14);
  cm_fn_005e8290(s_Error_009870e0,(const char *)(&DAT_00ae24d0),0);
  DAT_00b67a34 = 0;
  return 0;
}
