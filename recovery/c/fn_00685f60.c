#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define CONCAT31(a, b) ((u32)(((u32)(u32)(a) << 8) | (u32)(u8)(b)))
#define CM_ADD_WORD_BYTE(destination, source) do { \
  s8 cm_source_byte = *(volatile s8 *)(source); \
  s16 cm_destination_word = *(volatile s16 *)(destination); \
  *(volatile s16 *)(destination) = (s16)(cm_destination_word + cm_source_byte); \
} while (0)
#define CM_SUM_U8(destination, first, second) do { \
  u8 cm_first_byte = *(volatile u8 *)(first); \
  u8 cm_second_byte = *(volatile u8 *)(second); \
  *(volatile u8 *)(destination) = (u8)(cm_first_byte + cm_second_byte); \
} while (0)
#define CM_SUM_U16(destination, first, second) do { \
  u16 cm_first_word = *(volatile u16 *)(first); \
  u16 cm_second_word = *(volatile u16 *)(second); \
  *(volatile u16 *)(destination) = (u16)(cm_first_word + cm_second_word); \
} while (0)
extern u32 CM_THISCALL cm_fn_006869a0(u8 *object, u8 *table, s32 arg3, s32 arg4, s8 arg5, s8 arg6);


static s8 cm_fn_00685f60_slot90(int *owner, u32 record, int *source)
{
  volatile u32 *volatile vtable = *(volatile u32 *volatile *)owner;
  u32 target = *(volatile u32 *)((volatile u8 *)vtable + 0x90);
  typedef s8 (CM_THISCALL *slot_type)(int *self, u32 record_id, int *source_object);
  return ((slot_type)target)(owner, record, source);
}

void CM_THISCALL
cm_fn_00685f60(int *param_1,u32 param_2,u32 param_3,short param_4,int param_5,short *param_6,
            short *param_7)

{
  volatile s16 *psVar1;
  volatile s8 *pcVar2;
  u16 uVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  char cVar8;
  u16 uVar9;
  u32 iVar10;
  int iVar11;
  int iVar12;
  short sVar13;
  int *piVar14;
  u32 uVar15;
  int iVar16;
  
  sVar6 = -1;
  sVar7 = -1;
  u32 self_field = *(volatile u32 *)((volatile u8 *)param_1 + 4);
  if (self_field == 0) {
    piVar14 = (int *)0x0;
  }
  else {
    u32 global_table = *(volatile u32 *)0x00adadfcu;
    if (global_table == 0) {
      piVar14 = (int *)0x0;
    } else {
      u32 index = *(volatile u32 *)self_field;
      piVar14 = (int *)*(volatile u32 *)(global_table + index * 4u);
      if (*(volatile u8 *)((volatile u8 *)piVar14 + 0x43) != 2) {
        piVar14 = param_1;
      }
    }
  }
  sVar13 = 0;
  if (0 < param_4) {
    u32 first_id = *(volatile u32 *)(param_2 + 0x1c);
    do {
      iVar10 = *(volatile int *)(sVar13 * 0x3b + param_3);
      sVar4 = sVar7;
      sVar5 = sVar13;
      if ((((iVar10 != first_id) &&
           (sVar4 = sVar13, sVar5 = sVar6, iVar10 != *(volatile u32 *)(param_2 + 0x20))) &&
          (sVar4 = sVar7, sVar6 != -1)) && (sVar7 != -1)) break;
      sVar13 = sVar13 + 1;
      sVar6 = sVar5;
      sVar7 = sVar4;
    } while (sVar13 < param_4);
    if (-1 < sVar6) {
      iVar10 = sVar6 * 0x3b + param_3;
      *(volatile char *)(iVar10 + 0x1f) = *(volatile char *)(iVar10 + 0x1f) + '\x01';
      cVar8 = *(volatile char *)(param_2 + 0x47);
      if (cVar8 == -1) {
        CM_ADD_WORD_BYTE(iVar10 + 0x23, param_2 + 0x43);
        cVar8 = *(volatile char *)(param_2 + 0x44);
      }
      else {
        s16 old23 = *(volatile s16 *)(iVar10 + 0x23);
        *(volatile s16 *)(iVar10 + 0x23) = (s16)(old23 + cVar8);
        cVar8 = *(volatile char *)(param_2 + 0x48);
      }
      s16 old25 = *(volatile s16 *)(iVar10 + 0x25);
      *(volatile s16 *)(iVar10 + 0x25) = (s16)(old25 + cVar8);
    }
    if (-1 < sVar7) {
      iVar10 = sVar7 * 0x3b + param_3;
      *(volatile char *)(iVar10 + 0x2d) = *(volatile char *)(iVar10 + 0x2d) + '\x01';
      cVar8 = *(volatile char *)(param_2 + 0x47);
      if (cVar8 == -1) {
        CM_ADD_WORD_BYTE(iVar10 + 0x31, param_2 + 0x44);
        CM_ADD_WORD_BYTE(iVar10 + 0x33, param_2 + 0x43);
      }
      else {
        CM_ADD_WORD_BYTE(iVar10 + 0x31, param_2 + 0x48);
        CM_ADD_WORD_BYTE(iVar10 + 0x33, param_2 + 0x47);
      }
    }
  }
  s8 score43 = *(volatile s8 *)(param_2 + 0x43);
  s8 score44 = *(volatile s8 *)(param_2 + 0x44);
  if (score44 < score43) {
    if (-1 < sVar6) {
      iVar11 = (int)sVar6;
      iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
      *(volatile char *)(iVar10 + 0x20) = *(volatile char *)(iVar11 + param_3 + 0x20 + iVar11 * 0x3a) + '\x01';
      if (piVar14 == (int *)0x0) {
        piVar14 = param_1;
      }
      cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x1c), (int *)param_2);
      psVar1 = (short *)(iVar10 + 0x27);
      *psVar1 = *psVar1 + (short)cVar8;
    }
    if (-1 < sVar7) {
      iVar10 = (int)sVar7;
      *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) =
           *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) + '\x01';
    }
  }
  else if (score43 < score44) {
    if (-1 < sVar6) {
      pcVar2 = (volatile s8 *)(sVar6 * 0x3b + param_3 + 0x22);
      *pcVar2 = *pcVar2 + '\x01';
    }
    if (-1 < sVar7) {
      iVar11 = (int)sVar7;
      iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
      *(volatile char *)(iVar10 + 0x2e) = *(volatile char *)(iVar11 + param_3 + 0x2e + iVar11 * 0x3a) + '\x01';
      if (piVar14 == (int *)0x0) {
        piVar14 = param_1;
      }
      cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x20), (int *)param_2);
      psVar1 = (short *)(iVar10 + 0x35);
      *psVar1 = *psVar1 + (short)cVar8;
    }
  }
  else {
    uVar3 = *(volatile u16 *)(param_2 + 0x38);
    if ((uVar3 & 2) != 0) {
      s8 score47a = *(volatile s8 *)(param_2 + 0x47);
      s8 score48a = *(volatile s8 *)(param_2 + 0x48);
      if (score48a < score47a) {
        if (-1 < sVar6) {
          iVar11 = (int)sVar6;
          iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
          *(volatile char *)(iVar10 + 0x20) = *(volatile char *)(iVar11 + param_3 + 0x20 + iVar11 * 0x3a) + '\x01';
          if (piVar14 == (int *)0x0) {
            piVar14 = param_1;
          }
          cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x1c), (int *)param_2);
          psVar1 = (short *)(iVar10 + 0x27);
          *psVar1 = *psVar1 + (short)cVar8;
        }
        if (-1 < sVar7) {
          iVar10 = (int)sVar7;
          *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) =
               *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) + '\x01';
        }
        if (param_5 != 0) {
          *(volatile u8 *)(param_2 + 0x3d) = 3;
        }
        goto LAB_00686452;
      }
      s8 score47b = *(volatile s8 *)(param_2 + 0x47);
      s8 score48b = *(volatile s8 *)(param_2 + 0x48);
      if (((uVar3 & 2) != 0) && (score47b < score48b)) {
        if (-1 < sVar6) {
          pcVar2 = (volatile s8 *)(sVar6 * 0x3b + param_3 + 0x22);
          *pcVar2 = *pcVar2 + '\x01';
        }
        if (-1 < sVar7) {
          iVar11 = (int)sVar7;
          iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
          *(volatile char *)(iVar10 + 0x2e) = *(volatile char *)(iVar11 + param_3 + 0x2e + iVar11 * 0x3a) + '\x01';
          if (piVar14 == (int *)0x0) {
            piVar14 = param_1;
          }
          cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x20), (int *)param_2);
          psVar1 = (short *)(iVar10 + 0x35);
          *psVar1 = *psVar1 + (short)cVar8;
        }
        if (param_5 != 0) {
          *(volatile u8 *)(param_2 + 0x3e) = 3;
        }
        goto LAB_00686452;
      }
    }
    uVar9 = uVar3 & 0x20;
    s8 tie49a = 0;
    s8 tie4aa = 0;
    int first_pair_ordered = ((uVar9 == 0) && ((uVar3 & 1) == 0));
    if (!first_pair_ordered) {
      tie49a = *(volatile s8 *)(param_2 + 0x49);
      tie4aa = *(volatile s8 *)(param_2 + 0x4a);
      first_pair_ordered = tie49a <= tie4aa;
    }
    if (first_pair_ordered) {
      s8 tie49b = 0;
      s8 tie4ab = 0;
      int second_pair_ordered = ((uVar9 == 0) && ((uVar3 & 1) == 0));
      if (!second_pair_ordered) {
        tie49b = *(volatile s8 *)(param_2 + 0x49);
        tie4ab = *(volatile s8 *)(param_2 + 0x4a);
        second_pair_ordered = tie4ab <= tie49b;
      }
      if (second_pair_ordered) {
        if ((uVar9 == 0) && ((uVar3 & 1) == 0)) {
          if (-1 < sVar6) {
            iVar10 = param_3 + sVar6 * 0x3b;
            pcVar2 = (volatile s8 *)(iVar10 + 0x21);
            *pcVar2 = *pcVar2 + '\x01';
            CM_ADD_WORD_BYTE(iVar10 + 0x27, (u32)param_1 + 0xc3u);
          }
          if (-1 < sVar7) {
            iVar10 = sVar7 * 0x3b + param_3;
            pcVar2 = (volatile s8 *)(iVar10 + 0x2f);
            *pcVar2 = *pcVar2 + '\x01';
            CM_ADD_WORD_BYTE(iVar10 + 0x35, (u32)param_1 + 0xc3u);
          }
        }
      }
      else {
        if (-1 < sVar6) {
          pcVar2 = (volatile s8 *)(sVar6 * 0x3b + param_3 + 0x22);
          *pcVar2 = *pcVar2 + '\x01';
        }
        if (-1 < sVar7) {
          iVar11 = (int)sVar7;
          iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
          *(volatile char *)(iVar10 + 0x2f) = *(volatile char *)(iVar11 + param_3 + 0x2f + iVar11 * 0x3a) + '\x01';
          if (piVar14 == (int *)0x0) {
            piVar14 = param_1;
          }
          cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x20), (int *)param_2);
          psVar1 = (short *)(iVar10 + 0x35);
          *psVar1 = *psVar1 + (short)cVar8;
        }
        if (param_5 != 0) {
          if ((*(volatile u8 *)(param_2 + 0x38) & 0x20) == 0) {
            *(volatile u8 *)(param_2 + 0x3e) = 4;
          }
          else {
            *(volatile u8 *)(param_2 + 0x3e) = 5;
          }
        }
      }
    }
    else {
      if (-1 < sVar6) {
        iVar11 = (int)sVar6;
        iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
        *(volatile char *)(iVar10 + 0x21) = *(volatile char *)(iVar11 + param_3 + 0x21 + iVar11 * 0x3a) + '\x01';
        if (piVar14 == (int *)0x0) {
          piVar14 = param_1;
        }
        cVar8 = cm_fn_00685f60_slot90(piVar14, *(volatile u32 *)(param_2 + 0x1c), (int *)param_2);
        psVar1 = (short *)(iVar10 + 0x27);
        *psVar1 = *psVar1 + (short)cVar8;
      }
      if (-1 < sVar7) {
        iVar10 = (int)sVar7;
        *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) =
             *(volatile char *)(param_3 + iVar10 + 0x30 + iVar10 * 0x3a) + '\x01';
      }
      if (param_5 != 0) {
        if ((*(volatile u8 *)(param_2 + 0x38) & 0x20) == 0) {
          *(volatile u8 *)(param_2 + 0x3d) = 4;
        }
        else {
          *(volatile u8 *)(param_2 + 0x3d) = 5;
        }
      }
    }
  }
LAB_00686452:
  if (sVar6 < 0) {
    if (param_6 != (short *)0x0) {
      *(volatile s16 *)param_6 = 0;
    }
  }
  else {
    iVar11 = (int)sVar6;
    iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
    CM_SUM_U8(iVar10 + 0x11, iVar10 + 0x2d, iVar10 + 0x1f);
    CM_SUM_U8(iVar10 + 0x12, iVar10 + 0x2e, iVar10 + 0x20);
    CM_SUM_U8(iVar10 + 0x13, iVar10 + 0x2f, iVar10 + 0x21);
    CM_SUM_U8(iVar10 + 0x14, iVar10 + 0x30, iVar10 + 0x22);
    CM_SUM_U16(iVar10 + 0x15, iVar10 + 0x31, iVar10 + 0x23);
    CM_SUM_U16(iVar10 + 0x17, iVar10 + 0x33, iVar10 + 0x25);
    u16 total19a = *(volatile u16 *)(iVar10 + 0x35);
    u16 total19b = *(volatile u16 *)(iVar10 + 0x27);
    sVar13 = (s16)(total19a + total19b);
    *(volatile short *)(iVar10 + 0x19) = sVar13;
    if (param_6 != (short *)0x0) {
      *(volatile s16 *)param_6 = sVar13;
    }
  }
  if (sVar7 < 0) {
    if (param_7 != (short *)0x0) {
      *(volatile s16 *)param_7 = 0;
    }
  }
  else {
    iVar11 = (int)sVar7;
    iVar10 = iVar11 + param_3 + iVar11 * 0x3a;
    CM_SUM_U8(iVar10 + 0x11, iVar10 + 0x2d, iVar10 + 0x1f);
    CM_SUM_U8(iVar10 + 0x12, iVar10 + 0x2e, iVar10 + 0x20);
    CM_SUM_U8(iVar10 + 0x13, iVar10 + 0x2f, iVar10 + 0x21);
    CM_SUM_U8(iVar10 + 0x14, iVar10 + 0x30, iVar10 + 0x22);
    CM_SUM_U16(iVar10 + 0x15, iVar10 + 0x31, iVar10 + 0x23);
    CM_SUM_U16(iVar10 + 0x17, iVar10 + 0x33, iVar10 + 0x25);
    u16 total19a = *(volatile u16 *)(iVar10 + 0x35);
    u16 total19b = *(volatile u16 *)(iVar10 + 0x27);
    sVar13 = (s16)(total19a + total19b);
    *(volatile short *)(iVar10 + 0x19) = sVar13;
    if (param_7 != (short *)0x0) {
      *(volatile s16 *)param_7 = sVar13;
    }
  }
  if (*(volatile int *)(param_3 + 5) != 0) {
    iVar16 = 0;
    iVar12 = param_4 + -1;
    param_5 = -1;
    param_6 = (short *)0xffffffff;
    iVar10 = iVar12;
    uVar15 = 0xffffffff;
    iVar11 = -1;
    if (0 < iVar12) {
      do {
        if (-1 < sVar6) {
          uVar15 = (u32)(short)iVar16;
          u32 row_base = *(volatile u32 *)(sVar6 * 0x3b + param_3 + 5);
          u32 opponent_id = *(volatile u32 *)(param_2 + 0x20);
          iVar10 = *(volatile int *)(row_base + uVar15 * 0xe);
          if (((iVar10 == opponent_id) && (param_6 = (short *)uVar15, -1 < param_5)) &&
             (iVar11 = param_5, uVar15 < 0x80000000)) break;
        }
        uVar15 = (u32)param_6;
        if (-1 < sVar7) {
          iVar11 = (int)(short)iVar16;
          u32 row_base = *(volatile u32 *)(sVar7 * 0x3b + param_3 + 5);
          u32 current_id = *(volatile u32 *)(param_2 + 0x1c);
          iVar10 = *(volatile int *)(row_base + iVar11 * 0xe);
          if (((iVar10 == current_id) && (param_5 = iVar11, -1 < iVar11)) &&
             ((u32)param_6 < 0x80000000u)) break;
        }
        iVar16 = iVar16 + 1;
        iVar10 = iVar12;
        iVar11 = param_5;
      } while ((short)iVar16 < iVar12);
    }
    s8 first_call44 = *(volatile s8 *)(param_2 + 0x44);
    s8 first_call43 = *(volatile s8 *)(param_2 + 0x43);
    cm_fn_006869a0((u8 *)param_1,(u8 *)param_3,(int)sVar6,uVar15,
                   first_call43, first_call44);
    s8 second_call43 = *(volatile s8 *)(param_2 + 0x43);
    s8 second_call44 = *(volatile s8 *)(param_2 + 0x44);
    cm_fn_006869a0((u8 *)param_1,(u8 *)param_3,(int)sVar7,iVar11,
                   second_call44, second_call43);
  }
  return;
}
