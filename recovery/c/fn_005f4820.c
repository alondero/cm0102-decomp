/* WIP for preferred-image VA 0x005f4820, image base 0x00400000. Three failed
   iterations total (one compile/build, two semantic); semantic cases 127, 164,
   240 report the same page 0x00ae2000, with inconclusive byte-level deltas.
   Resume only with new evidence; do not treat these runs as acceptance evidence. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009cf338 (*(volatile int *)0x009cf338)
#define DAT_009cf568 (*(volatile int *)0x009cf568)
#define DAT_009cf56c (*(volatile int *)0x009cf56c)
#define DAT_009cf6e8 (*(volatile int *)0x009cf6e8)
#define DAT_009cf6f0 (*(volatile int *)0x009cf6f0)
#define DAT_009cf6f8 (*(volatile int *)0x009cf6f8)
#define DAT_009cf764 (*(volatile int *)0x009cf764)
#define DAT_00ae235c (*(volatile int *)0x00ae235c)
#define DAT_00ae23a8 (*(volatile int *)0x00ae23a8)
#define DAT_00ae23b4 (*(volatile int *)0x00ae23b4)
#define DAT_00ae24d0 (*(volatile u8 *)0x00ae24d0)
#define DAT_00b67a34 (*(volatile u32 *)0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_co_009cedd4 ((char *)0x009cedd4)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_004afb40(int param_1,short param_2);
extern void cm_fn_004afbd0(int param_1,short param_2,short param_3);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern void cm_fn_00945333(u32 *destination, u32 count, u32 element_size, void *compare);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_FASTCALL cm_fn_005f4820(u32 param_1)

{
  volatile u32 *puVar1;
  int iVar2;
  u32 uVar3;
  int iVar4;
  short sVar5;
  short sVar7;
  int iVar8;
  volatile int *piVar9;
  int iVar10;
  u8 scratch[0x6b8];
  volatile int *local_6b8 = (volatile int *)scratch;
  u8 *local_6b0 = scratch + 8;
  u8 *local_5b0 = scratch + 0x108;
  volatile int *local_4b0 = (volatile int *)(scratch + 0x208);
  
  iVar10 = 0;
  *(volatile u32 *)(param_1 + 0xf2) = 0x56;
  *local_6b8 = 0;
  iVar2 = (u32)cm_fn_00945501(4,0x56);
  *(volatile int *)(param_1 + 0xee) = iVar2;
  if (iVar2 == 0) {
    cm_fn_00944cff((u32)s_E__dev_CM3_cm3_00_01_cm3_code_co_009cedd4,0,0,local_5b0,local_6b0);
    uVar3 = (u32)cm_fn_0090d130();
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar3,(u32)local_5b0,(u32)local_6b0,0x32c);
    cm_fn_005e8290(s_Error_009870e0,(const char *)&DAT_00ae24d0,0);
    *(volatile u32 *)0x00b67a34 = 0;
    return 0;
  }
  sVar7 = 0;
  u32 record_address = (u32)DAT_00ae23b4;
  s32 first_limit = DAT_00ae235c;
  s32 season_register = DAT_009cf338;
  if (0 < first_limit) {
    do {
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        u32 type_id = *(volatile u32 *)type_ptr;
        s32 required_type = DAT_009cf764;
        if (type_id == (u32)required_type) {
          s32 season = DAT_009cf338;
          s32 base = DAT_00ae23a8;
          u32 expected = (u32)base + (u32)season * 0x122u;
          u32 candidate = *(volatile u32 *)(record_address + 0x53u);
          if (candidate == expected) {
            s32 old_count = *local_6b8;
            s16 table_index = (s16)old_count;
            if (table_index < 300) {
              u32 table_address = (u32)(unsigned long)(scratch + 0x208u) +
                                  (u32)(s32)table_index * 4u;
              *(volatile u32 *)table_address = record_address;
              *local_6b8 = (u32)old_count + 1u;
            }
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    } while (sVar7 < first_limit);
  }
  if (first_limit > 0) {
    season_register = DAT_009cf338;
  }
  sVar7 = 0;
  record_address = (u32)DAT_00ae23b4;
  iVar8 = first_limit;
  if (0 < iVar8) {
    int first_iteration = 1;
    while (sVar7 < iVar8) {
      if (!first_iteration) {
        season_register = DAT_009cf338;
      }
      first_iteration = 0;
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        u32 type_id = *(volatile u32 *)type_ptr;
        s32 required_type = DAT_009cf56c;
        if (type_id == (u32)required_type) {
          s32 base = DAT_00ae23a8;
          u32 expected = (u32)base + (u32)season_register * 0x122u;
          u32 candidate = *(volatile u32 *)(record_address + 0x53u);
          if (candidate == expected) {
            s32 maximum = *(volatile int *)(param_1 + 0xf2);
            if (iVar10 < maximum) {
              u32 output_base = *(volatile u32 *)(param_1 + 0xee);
              iVar10 = iVar10 + 1;
              *(volatile u32 *)(output_base + (u32)iVar10 * 4u - 4u) = record_address;
              iVar8 = DAT_00ae235c;
            }
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    }
  }
  sVar7 = 0;
  record_address = (u32)DAT_00ae23b4;
  if (0 < iVar8) {
    do {
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        s32 required_type = DAT_009cf568;
        u32 type_id = *(volatile u32 *)type_ptr;
        if (type_id == (u32)required_type) {
          s32 maximum = *(volatile int *)(param_1 + 0xf2);
          if (iVar10 < maximum &&
              *(volatile int *)(record_address + 0x1dbu) == -1) {
            u32 output_base = *(volatile u32 *)(param_1 + 0xee);
            iVar10 = iVar10 + 1;
            *(volatile u32 *)(output_base + (u32)iVar10 * 4u - 4u) = record_address;
            iVar8 = DAT_00ae235c;
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    } while (sVar7 < iVar8);
  }
  iVar2 = 0;
  iVar8 = 0x32;
  do {
    iVar4 = iVar10;
    u32 allocation_list = *(volatile u32 *)(param_1 + 0xee);
    puVar1 = (volatile u32 *)(allocation_list + (u32)iVar2);
    iVar10 = iVar4 + 1;
    u32 copied_item = *puVar1;
    iVar2 = iVar2 + 4;
    *(volatile u32 *)(allocation_list + (u32)iVar10 * 4u - 4u) = copied_item;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  cm_fn_00945333((u32 *)(u32)*(volatile u32 *)(param_1 + 0xee),(u32)iVar10,4,(void *)0x0053fe50);
  if (0 < iVar10) {
    sVar7 = 1;
    do {
      iVar2 = (int)sVar7;
      if (iVar2 < iVar10) {
        sVar5 = sVar7;
        do {
          s16 previous_index = (s16)(sVar7 - 1);
          u32 list_address = *(volatile u32 *)(param_1 + 0xee);
          u32 previous_item = *(volatile u32 *)(list_address + (u32)(s32)previous_index * 4u);
          u32 current_address = list_address + (u32)iVar2 * 4u;
          u32 current_item = *(volatile u32 *)current_address;
          if (previous_item == current_item) {
            *(volatile u32 *)current_address = 0u;
          }
          sVar5 = sVar5 + 1;
          iVar2 = (int)sVar5;
        } while (iVar2 < iVar10);
      }
      iVar2 = (int)sVar7;
      sVar7 = sVar7 + 1;
    } while (iVar2 < iVar10);
  }
  cm_fn_004afbd0((u32)local_4b0,(short)*local_6b8,0x32);
  piVar9 = local_4b0;
  iVar2 = 0x32;
  do {
    sVar7 = 0;
    if (0 < iVar10) {
      u32 output_base = *(volatile u32 *)(param_1 + 0xee);
      iVar8 = 0;
      do {
        u32 current_item = *(volatile u32 *)(output_base + (u32)iVar8 * 4u);
        if (current_item == 0u) {
          u32 replacement = (u32)*piVar9;
          *(volatile u32 *)(output_base + (u32)(s32)sVar7 * 4u) = replacement;
          break;
        }
        sVar7 = sVar7 + 1;
        iVar8 = (int)sVar7;
      } while (iVar8 < iVar10);
    }
    piVar9 = piVar9 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee),8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x20u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x40u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x60u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x80u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0xa0u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0xc0u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0xe0u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x100u,8);
  cm_fn_004afb40(*(volatile u32 *)(param_1 + 0xee) + 0x120u,iVar4 + -0x47);
  sVar7 = 0;
  iVar8 = DAT_00ae235c;
  record_address = (u32)DAT_00ae23b4;
  if (0 < DAT_00ae235c) {
    do {
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        s32 required_type = DAT_009cf568;
        u32 type_id = *(volatile u32 *)type_ptr;
        if (type_id == (u32)required_type &&
            iVar10 < *(volatile int *)(param_1 + 0xf2)) {
          s32 ranking = *(volatile int *)(record_address + 0x1dbu);
          s32 required_ranking = DAT_009cf6f8;
          if (ranking == required_ranking) {
            u32 output_base = *(volatile u32 *)(param_1 + 0xee);
            iVar10 = iVar10 + 1;
            *(volatile u32 *)(output_base + (u32)iVar10 * 4u - 4u) = record_address;
            iVar8 = DAT_00ae235c;
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    } while (sVar7 < iVar8);
  }
  sVar7 = 0;
  record_address = (u32)DAT_00ae23b4;
  if (0 < iVar8) {
    do {
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        s32 required_type = DAT_009cf568;
        u32 type_id = *(volatile u32 *)type_ptr;
        if (type_id == (u32)required_type &&
            iVar10 < *(volatile int *)(param_1 + 0xf2)) {
          s32 ranking = *(volatile int *)(record_address + 0x1dbu);
          s32 required_ranking = DAT_009cf6f0;
          if (ranking == required_ranking) {
            u32 output_base = *(volatile u32 *)(param_1 + 0xee);
            iVar10 = iVar10 + 1;
            *(volatile u32 *)(output_base + (u32)iVar10 * 4u - 4u) = record_address;
            iVar8 = DAT_00ae235c;
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    } while (sVar7 < iVar8);
  }
  sVar7 = 0;
  record_address = (u32)DAT_00ae23b4;
  if (0 < iVar8) {
    do {
      u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
      if (type_ptr != 0u) {
        s32 required_type = DAT_009cf568;
        u32 type_id = *(volatile u32 *)type_ptr;
        if (type_id == (u32)required_type &&
            iVar10 < *(volatile int *)(param_1 + 0xf2)) {
          s32 ranking = *(volatile int *)(record_address + 0x1dbu);
          s32 required_ranking = DAT_009cf6e8;
          if (ranking == required_ranking) {
            u32 output_base = *(volatile u32 *)(param_1 + 0xee);
            iVar10 = iVar10 + 1;
            *(volatile u32 *)(output_base + (u32)iVar10 * 4u - 4u) = record_address;
            iVar8 = DAT_00ae235c;
          }
        }
      }
      sVar7 = sVar7 + 1;
      record_address += 0x245u;
    } while (sVar7 < iVar8);
  }
  iVar2 = *(volatile int *)(param_1 + 0xf2);
  if ((iVar10 < iVar2) && (sVar7 = 0, 0 < iVar2)) {
    *local_6b8 = 0;
    do {
      u32 list_address = *(volatile u32 *)(param_1 + 0xee);
      if (*(volatile u32 *)(list_address + (u32)*local_6b8 * 4u) == 0u) {
        s32 scan_limit = DAT_00ae235c;
        record_address = (u32)DAT_00ae23b4;
        iVar8 = 0;
        if (scan_limit > 0) {
        do {
          u32 type_ptr = *(volatile u32 *)(record_address + 0x57u);
          if (type_ptr != 0u) {
            s32 required_type = DAT_009cf764;
            u32 type_id = *(volatile u32 *)type_ptr;
            if (type_id == (u32)required_type) {
              s32 season = DAT_009cf338;
              s32 base = DAT_00ae23a8;
              u32 expected = (u32)base + (u32)season * 0x122u;
              u32 candidate = *(volatile u32 *)(record_address + 0x53u);
              if (candidate == expected) {
                iVar4 = 0;
                u32 compared_address = list_address;
                if (iVar2 < 1) {
                  *(volatile u32 *)(list_address + (u32)*local_6b8 * 4u) = record_address;
                  break;
                }
                while (*(volatile u32 *)compared_address != record_address) {
                  iVar4 = iVar4 + 1;
                  compared_address += 4u;
                  if (iVar2 <= iVar4) {
                    *(volatile u32 *)(list_address + (u32)*local_6b8 * 4u) = record_address;
                    goto LAB_005f4cfe_done;
                  }
                }
              }
            }
          }
          iVar8 = iVar8 + 1;
          record_address += 0x245u;
        } while (iVar8 < scan_limit);
LAB_005f4cfe_done:
        ;
        }
      }
      iVar2 = *(volatile int *)(param_1 + 0xf2);
      sVar7 = sVar7 + 1;
      *local_6b8 = (int)sVar7;
    } while (*local_6b8 < iVar2);
  }
  return 1;
}
