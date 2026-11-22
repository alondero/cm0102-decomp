#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define CONCAT22(a, b) ((u32)(((u32)(u16)(a) << 16) | (u32)(u16)(b)))
#define CONCAT31(a, b) ((u32)(((u32)(a) << 8) | (u32)(u8)(b)))
#define ExceptionList CM_FS(void *, 0)
#define DAT_00ae2364 CM_GLOBAL(int, 0x00ae2364)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define DAT_00ae2c94 CM_GLOBAL(u32, 0x00ae2c94)
#define LAB_00632040 CM_GLOBAL(u8, 0x00632040)
#define LAB_0095b168 CM_GLOBAL(u8, 0x0095b168)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1,u32 *param_2,u16 *param_3);
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1,u32 *param_2,int param_3);
extern u32 * CM_THISCALL cm_fn_0054cc30(short *param_1,u32 *param_2,u32 param_3,u32 param_4);
extern void CM_THISCALL cm_fn_0062c680(int *param_1,int *param_2);
extern void CM_THISCALL cm_fn_00630ee0(int *param_1,int *param_2,char param_3,char param_4,u32 param_5,short param_6);
extern u32 cm_fn_00946464(u32 param_1,u32 param_2,u32 param_3,int param_4,void *param_5);

#define D32(o) (*(volatile u32 *)(scratch + (o)))
#define D16(o) (*(volatile u16 *)(scratch + (o)))
#define D8(o)  (*(volatile u8  *)(scratch + (o)))

void CM_FASTCALL cm_fn_0062e010(int *param_1)
{
  volatile u8 scratch[0x38];
  struct { volatile u32 prev; volatile u32 handler; volatile u32 state; } frame;
  int iVar8;
  u32 outer;
  u32 esi;
  u32 uVar9;
  int iVar15;
  int iVar4a;
  u16 sVar12;
  int retval;

  frame.state = (u32)-1;
  frame.handler = (u32)&LAB_0095b168;
  frame.prev = (u32)CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)&frame;

  D32(0x08) = 0;
  iVar8 = DAT_00ae2364;
  for (outer = 0; outer < 0x10; outer++) {
    D32(0x08) = outer;
    {
      u32 idx = (u32)(iVar8 - (int)outer - 1);
      u32 t = idx + idx * 4;
      u32 u = idx + t * 2;
      u32 v = u + u * 4;
      int base = DAT_00ae23bc + (int)(v * 2);
      int p = *(int *)(base + 0x39);
      if (p != 0) {
        int *piVar5 = (int *)(p + 0xd7);
        D32(0x04) = 0x32;
        D32(0x00) = (u32)piVar5;
        do {
          int *piVar1 = (int *)*piVar5;
          if (piVar1 != (int *)0 && *piVar1 < iVar8) {
            u32 iVar4 = (u32)*piVar1 * 0x1f;
            int field = *(int *)((u8 *)param_1[0] + iVar4 + 0xe);
            if (field != -1) {
              u32 pv = (u32)param_1[1] + (u32)field * 0x30;
              u32 ebx_val = DAT_00ae2c90;
              u32 ecx_val = DAT_00ae2c94;
              D32(0x14) = ecx_val;
              D32(0x10) = ebx_val;
              {
                u16 dx = *(u16 *)(pv + 0x2a);
                u32 edx_val = ((iVar4 >> 16) << 16) | dx;
                u32 ps = cm_fn_0054c5b0((short *)(pv + 0x20),
                                        (u32 *)(scratch + 0x18), (int)edx_val);
                if (*(u16 *)(scratch + 0x12) == *(u16 *)(ps + 2) &&
                    *(u16 *)(scratch + 0x10) == *(u16 *)ps &&
                    (short)*(u16 *)(pv + 0x2a) > 5) {
                  u8 *addr = (u8 *)param_1[0] + ((u32)*piVar1 * 0x1f) + 0x1c;
                  if (*addr == 0) {
                    *addr = 1;
                    {
                      short diff = (short)(*(u16 *)(pv + 0x28) - *(u16 *)(pv + 0x2a));
                      u8 byt = *(u8 *)pv;
                      cm_fn_00630ee0(param_1, piVar1, 0, (char)byt, 0, diff);
                    }
                  }
                }
              }
              cm_fn_0062c680(param_1, piVar1);
              iVar8 = DAT_00ae2364;
            }
          }
          piVar5 = (int *)((u32)piVar5 + 4);
          D32(0x00) = (u32)piVar5;
          D32(0x04) = D32(0x04) - 1;
        } while (D32(0x04) != 0);
      }
    }
  }

  if (param_1[4] != 0) {
    cm_fn_0054a110((u16 *)(scratch + 0x20));
    frame.state = 0;
    cm_fn_0054a110((u16 *)(scratch + 0x28));
    frame.state = 1;
    cm_fn_00417c10((u32 *)(scratch + 0x20), (u32 *)(scratch + 0x18), (u16 *)&DAT_00ae2c90);
    cm_fn_00417c10((u32 *)(scratch + 0x28), (u32 *)(scratch + 0x18), (u16 *)&DAT_00ae2c90);
    esi = cm_fn_00946464((u32)(scratch + 0x20), (u32)param_1[4], (u32)param_1[6], 0x16, (void *)0x778e90);
    if (esi != 0) {
      if (esi > (u32)param_1[4]) {
        for (;;) {
          u32 rec = *(u32 *)(esi - 0x16);
          u16 axkey = D16(0x22);
          esi -= 0x16;
          D32(0x10) = rec;
          D32(0x14) = *(u32 *)(esi + 4);
          {
            u16 dx = D16(0x12);
            u16 cx = (u16)rec;
            if (dx > axkey) { esi += 0x16; break; }
            if (dx < axkey) goto first_check;
            if ((short)cx > (short)D16(0x20)) { esi += 0x16; break; }
          }
        first_check:
          retval = (int)cm_fn_00456740((u32)(scratch + 0x20), *(u32 *)esi, *(u32 *)(esi + 4));
          if (retval != 0) { esi += 0x16; break; }
          if (esi <= (u32)param_1[4]) break;
        }
      }
      if (esi <= (u32)(param_1[4] + (u32)param_1[6] * 0x16 - 0x16)) {
        do {
          u32 rec = *(u32 *)esi;
          D32(0x10) = rec;
          D32(0x14) = *(u32 *)(esi + 4);
          {
            u16 dx = D16(0x12);
            u16 cx = (u16)rec;
            u16 axkey = D16(0x22);
            if (dx > axkey) break;
            if (dx < axkey) goto second_check;
            if ((short)cx > (short)D16(0x20)) break;
          }
        second_check:
          retval = (int)cm_fn_00456740((u32)(scratch + 0x20), rec, *(u32 *)(esi + 4));
          if (retval != 0) break;
          {
            u32 x = *(u32 *)((u8 *)esi + 0x10);
            u32 tab = (u32)DAT_00ae23bc + x * 0x6e;
            u32 a, c, imm;
            cm_fn_0054cc30((short *)(esi + 8), (u32 *)(scratch + 8),
                           *(u32 *)esi, *(u32 *)(esi + 4));
            a = D32(0x0a);
            c = D32(0x08);
            *(volatile u8 *)&frame.state = 2;
            imm = c + a * 0x16d;
            cm_fn_00630ee0(param_1, (int *)tab, 0,
                           (char)*(u8 *)((u8 *)esi + 0x15),
                           (u32)(u8)*(u8 *)((u8 *)esi + 0x14), (short)imm);
          }
          esi += 0x16;
          *(volatile u8 *)&frame.state = 1;
        } while (esi <= (u32)(param_1[4] + (u32)param_1[6] * 0x16 - 0x16));
      }
    }

    uVar9 = cm_fn_00946464((u32)(scratch + 0x20), (u32)param_1[4], (u32)param_1[6], 0x16, (void *)&LAB_00632040);
    if (uVar9 != 0) {
      if (uVar9 > (u32)param_1[4]) {
        for (;;) {
          u32 rec = *(u32 *)(uVar9 - 0x16 + 8);
          D32(0x10) = rec;
          D32(0x14) = *(u32 *)(uVar9 - 0x16 + 12);
          {
            u16 dx = D16(0x12);
            u16 axkey = D16(0x2a);
            u16 cx = (u16)rec;
            u16 bx = D16(0x28);
            if (dx > axkey || (dx == axkey && (short)cx > (short)bx)) break;
          }
          D32(0x08) = *(u32 *)(uVar9 - 0x16 + 8);
          D32(0x0c) = *(u32 *)(uVar9 - 0x16 + 12);
          {
            u16 dx2 = D16(0x0a);
            u16 axkey = D16(0x2a);
            u16 cx2 = D16(0x08);
            u16 bx = D16(0x28);
            if (dx2 < axkey || (dx2 == axkey && (short)cx2 < (short)bx)) break;
            uVar9 -= 0x16;
            if (uVar9 <= (u32)param_1[4]) break;
          }
        }
      }
      if (uVar9 <= (u32)(param_1[4] + (u32)param_1[6] * 0x16 - 0x16)) {
        for (;;) {
          u32 rec = *(u32 *)(uVar9 + 8);
          D32(0x10) = rec;
          D32(0x14) = *(u32 *)(uVar9 + 12);
          {
            u16 dx = D16(0x12);
            u16 axkey = D16(0x2a);
            u16 cx = (u16)rec;
            u16 bx = D16(0x28);
            if (dx > axkey || (dx == axkey && (short)cx > (short)bx)) break;
          }
          {
            u32 v = *(u32 *)(uVar9 + 8);
            D32(0x08) = v;
            D32(0x0c) = *(u32 *)(uVar9 + 12);
            {
              u16 dx2 = D16(0x0a);
              u16 axkey = D16(0x2a);
              u16 cx2 = D16(0x08);
              u16 bx = D16(0x28);
              if (dx2 < axkey || (dx2 == axkey && (short)cx2 < (short)bx)) break;
            }
            {
              u32 tab = (u32)DAT_00ae23bc + *(u32 *)(uVar9 + 16) * 0x6e;
              cm_fn_00630ee0(param_1, (int *)tab, 1,
                             (char)*(u8 *)((u8 *)uVar9 + 0x15),
                             (u32)(u8)*(u8 *)((u8 *)uVar9 + 0x14), (short)0);
            }
          }
          uVar9 += 0x16;
          if ((u32)(param_1[4] + (u32)param_1[6] * 0x16 - 0x16) < uVar9) break;
        }
      }

      iVar4a = param_1[6];
      sVar12 = 0;
      iVar15 = 0;
      if (0 < iVar4a) {
        D16(0x20) = (u16)DAT_00ae2c90;
        D16(0x22) = (u16)(DAT_00ae2c90 >> 16);
        {
          u16 *pu = (u16 *)((u8 *)param_1[4] + 10);
          for (;;) {
            u16 hi = D16(0x22);
            if (hi <= *pu && (hi < *pu || (short)D16(0x20) <= (short)pu[-1])) break;
            sVar12 = sVar12 + 1;
            iVar15 = iVar15 + 1;
            pu = pu + 0xb;
            if (iVar15 >= iVar4a) break;
          }
          if (sVar12 != 0 && (int)(short)sVar12 != iVar4a) {
            u32 *psrc = (u32 *)((u32)param_1[4] + (u32)(int)(short)sVar12 * 0x16);
            u32 *pdst = (u32 *)param_1[4];
            u32 bytes = ((u32)iVar4a - (u32)(int)(short)sVar12) * 0x16;
            u32 cnt = bytes >> 2;
            u32 k;
            for (k = 0; k < cnt; k++) pdst[k] = psrc[k];
            for (k = 0; k < (bytes & 3); k++) ((u8 *)pdst)[cnt * 4 + k] = ((u8 *)psrc)[cnt * 4 + k];
          }
        }
      }
      param_1[6] = (int)((u32)param_1[6] - (u32)(int)(short)sVar12);
    }
  }

  CM_FS(void *, 0) = (void *)frame.prev;
  return;
}
