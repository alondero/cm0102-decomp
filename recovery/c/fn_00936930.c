/* Draft for preferred-image VA 0x00936930, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
extern int cm_fn_009446e0(u32 param_1,u32 param_2);
extern int cm_fn_00944840(u32 param_1,u32 param_2);

static s32 cm_fn_00936930_sdiv32(u32 numerator, s32 divisor)
{
  return (s32)numerator / divisor;
}

u32 cm_fn_00936930(u32 param_1,int param_2,volatile u8 *param_3)

{
  short sVar1;
  short sVar2;
  volatile u16 *puVar3;
  int iVar4;
  u32 uVar5;
  int iVar6;
  u32 uVar7;
  volatile u32 *puVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  volatile int *piVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  volatile short *local_c;
  int local_8;
  
  iVar10 = 0x60;
  u32 multiplier_29e = *(volatile u32 *)(param_1 + 0x29e) << 6;
  u32 scale_bits = *(volatile u32 *)(param_1 + 0x2a2) << 6;
  iVar14 = (s32)scale_bits;
  sVar1 = *(volatile short *)(param_1 + 0x29c);
  *(volatile u8 *)(param_1 + 0x442) = 1;
  puVar8 = (volatile u32 *)(param_1 + 0x5c);
  puVar3 = (volatile u16 *)(param_1 + 0x1dc);
  do {
    *puVar8 = 0;
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
    puVar8 = puVar8 + 1;
    iVar10 = iVar10 + -1;
  } while (iVar10 != 0);
  s16 dimension_360 = *(volatile short *)(param_1 + 0x360);
  *(volatile short *)(param_1 + 0x1e8) = dimension_360;
  iVar15 = (int)sVar1;
  iVar9 = iVar15 >> 1;
  iVar4 = cm_fn_00936930_sdiv32((u32)(s32)dimension_360 * scale_bits + (u32)iVar9, iVar15);
  iVar10 = iVar4;
  if (*(volatile int *)(param_1 + 0x2a6) == 1) {
    iVar10 = (s32)((u32)iVar4 + 9u);
  }
  uVar5 = (iVar10 + 0x20U) & 0xffffffc0;
  *(volatile u32 *)(param_1 + 0x74) = uVar5;
  iVar10 = 9;
  do {
    iVar11 = (int)*(volatile short *)(param_1 + (iVar10 * 3 + 0x1a7) * 2);
    if ((iVar10 == 2) && (*(volatile int *)(param_1 + 0x2a6) == 1)) {
      s16 left = *(volatile short *)(param_1 + 0x354);
      s16 right = *(volatile short *)(param_1 + 0x35a);
      u32 difference = (u32)(s32)left - (u32)(s32)right;
      if ((s32)difference < 0) difference = 0u - difference;
      if (cm_fn_00936930_sdiv32(difference * scale_bits + (u32)iVar9, iVar15) < 0x21) {
        iVar11 = (int)*(volatile short *)(param_1 + 0x354);
      }
    }
    *(volatile short *)(param_1 + 0x1dc + iVar10 * 4) = (short)iVar11;
    if (param_2 != 0) {
      iVar11 = cm_fn_00936930_sdiv32((u32)iVar11 * scale_bits + (u32)iVar9, iVar15);
      if (iVar10 == 3) {
        if (*(volatile int *)(param_1 + 0x2a6) == 1) {
          iVar11 = (s32)((u32)iVar11 + 9u);
        }
      }
      else if ((iVar10 < 3) || (iVar10 == 8)) {
        iVar11 = (s32)((u32)iVar11 + (uVar5 - (u32)iVar4));
      }
      *(volatile u32 *)(param_1 + 0x5c + iVar10 * 8) = (iVar11 + 0x20U) & 0xffffffc0;
    }
    sVar1 = *(volatile short *)(param_1 + 0x352 + iVar10 * 6);
    iVar11 = (int)sVar1;
    *(volatile short *)(param_1 + 0x1de + iVar10 * 4) = sVar1;
    if (param_2 != 0) {
      iVar6 = 1;
      u32 magnitude = (u32)iVar11;
      if (iVar11 < 0) {
        magnitude = 0u - magnitude;
        iVar6 = -1;
      }
      u32 scaled_offset = (u32)cm_fn_00936930_sdiv32(magnitude * scale_bits + (u32)iVar9, iVar15);
      u32 origin = *(volatile u32 *)(param_1 + 0x5c + iVar10 * 8);
      *(volatile u32 *)(param_1 + 0x60 + iVar10 * 8) =
           (((scaled_offset + 0x10U) & 0xffffffc0U) * (u32)iVar6) + origin;
    }
    iVar10 = iVar10 + -1;
  } while (-1 < iVar10);
  piVar12 = (volatile int *)(param_1 + 0x2e2);
  s16 dimension_1e0 = *(volatile short *)(param_1 + 0x1e0);
  *(volatile short *)(param_1 + 0x2da) = dimension_1e0;
  local_c = (short *)0x5;
  iVar10 = cm_fn_00936930_sdiv32((u32)(s32)dimension_1e0 * scale_bits, iVar15);
  *(volatile int *)(param_1 + 0x2aa) = iVar10;
  u32 endpoint_64 = *(volatile u32 *)(param_1 + 0x64);
  *(volatile u32 *)(param_1 + 0x2ae) = endpoint_64;
  *(volatile u32 *)(param_1 + 0x2b2) = endpoint_64 - (u32)iVar10;
  s16 dimension_1e8 = *(volatile short *)(param_1 + 0x1e8);
  *(volatile short *)(param_1 + 0x2dc) = dimension_1e8;
  iVar10 = cm_fn_00936930_sdiv32((u32)(s32)dimension_1e8 * scale_bits, iVar15);
  *(volatile int *)(param_1 + 0x2b6) = iVar10;
  u32 endpoint_74 = *(volatile u32 *)(param_1 + 0x74);
  *(volatile u32 *)(param_1 + 0x2ba) = endpoint_74;
  *(volatile u32 *)(param_1 + 0x2be) = endpoint_74 - (u32)iVar10;
  s16 dimension_1f0 = *(volatile short *)(param_1 + 0x1f0);
  *(volatile short *)(param_1 + 0x2de) = dimension_1f0;
  iVar10 = cm_fn_00936930_sdiv32((u32)(s32)dimension_1f0 * scale_bits, iVar15);
  *(volatile int *)(param_1 + 0x2c2) = iVar10;
  u32 endpoint_84 = *(volatile u32 *)(param_1 + 0x84);
  *(volatile u32 *)(param_1 + 0x2c6) = endpoint_84;
  *(volatile u32 *)(param_1 + 0x2ca) = endpoint_84 - (u32)iVar10;
  s16 dimension_1f8 = *(volatile short *)(param_1 + 0x1f8);
  *(volatile short *)(param_1 + 0x2e0) = dimension_1f8;
  iVar10 = cm_fn_00936930_sdiv32((u32)(s32)dimension_1f8 * scale_bits, iVar15);
  *(volatile int *)(param_1 + 0x2ce) = iVar10;
  u32 endpoint_94 = *(volatile u32 *)(param_1 + 0x94);
  *(volatile u32 *)(param_1 + 0x2d2) = endpoint_94;
  u32 cached_2ae = *(volatile u32 *)(param_1 + 0x2ae);
  iVar13 = (s32)(endpoint_94 - (u32)iVar10);
  *(volatile int *)(param_1 + 0x2f6) = iVar15;
  *(volatile int *)(param_1 + 0x2d6) = iVar13;
  *(volatile int *)(param_1 + 0x2e2) = iVar14;
  u32 cached_2b2 = *(volatile u32 *)(param_1 + 0x2b2);
  u32 cached_2d6 = *(volatile u32 *)(param_1 + 0x2d6);
  *(volatile u32 *)(param_1 + 0x30a) = cached_2b2;
  u32 cached_2ba = *(volatile u32 *)(param_1 + 0x2ba);
  *(volatile u32 *)(param_1 + 0x31a) = cached_2d6;
  s16 cached_2dc = *(volatile short *)(param_1 + 0x2dc);
  u32 edge_2ae_minus_2ba = cached_2ae - cached_2ba;
  *(volatile u32 *)(param_1 + 0x2e6) = edge_2ae_minus_2ba;
  *(volatile int *)(param_1 + 0x2f2) = iVar14;
  s16 cached_2da = *(volatile short *)(param_1 + 0x2da);
  *(volatile int *)(param_1 + 0x306) = iVar15;
  *(volatile int *)(param_1 + 0x2fa) = (int)cached_2da - (int)cached_2dc;
  u32 cached_2c6 = *(volatile u32 *)(param_1 + 0x2c6);
  *(volatile u32 *)(param_1 + 0x2ea) = cached_2ba - cached_2c6;
  s16 cached_2de = *(volatile short *)(param_1 + 0x2de);
  *(volatile int *)(param_1 + 0x2fe) = (int)cached_2dc - (int)cached_2de;
  u32 cached_2d2 = *(volatile u32 *)(param_1 + 0x2d2);
  u32 cached_2e0base_minus_2d2 = cached_2c6 - cached_2d2;
  s16 cached_2e0 = *(volatile short *)(param_1 + 0x2e0);
  *(volatile u32 *)(param_1 + 0x2ee) = cached_2e0base_minus_2d2;
  *(volatile int *)(param_1 + 0x302) = (int)cached_2de - (int)cached_2e0;
  do {
    {
      u32 second_arg = (u32)piVar12[5];
      u32 first_arg = (u32)piVar12[0];
      iVar10 = cm_fn_00944840(first_arg,second_arg);
    }
    piVar12[0xf] = iVar10;
    piVar12 = piVar12 + 1;
    local_c = (short *)((int)local_c + -1);
  } while (local_c != (short *)0x0);
  {
    s16 arg_a = *(volatile short *)(param_1 + 0x2dc);
    s16 arg_b = *(volatile short *)(param_1 + 0x2da);
    u32 helper_id = *(volatile u32 *)(param_1 + 0x322);
    iVar10 = cm_fn_009446e0(helper_id,(u32)((s32)arg_a + (s32)arg_b));
  }
  {
    u32 left = *(volatile u32 *)(param_1 + 0x2ba);
    u32 right = *(volatile u32 *)(param_1 + 0x2ae);
    s16 next_a = *(volatile short *)(param_1 + 0x2de);
    s16 next_b = *(volatile short *)(param_1 + 0x2dc);
    *(volatile int *)(param_1 + 0x30e) = (s32)((left - (u32)iVar10 + 1U + right)) >> 1;
    u32 helper_id = *(volatile u32 *)(param_1 + 0x326);
    iVar10 = cm_fn_009446e0(helper_id,(u32)((s32)next_a + (s32)next_b));
  }
  {
    u32 left = *(volatile u32 *)(param_1 + 0x2c6);
    u32 right = *(volatile u32 *)(param_1 + 0x2ba);
    s16 next_a = *(volatile short *)(param_1 + 0x2de);
    s16 next_b = *(volatile short *)(param_1 + 0x2e0);
    *(volatile int *)(param_1 + 0x312) = (s32)((left - (u32)iVar10 + 1U + right)) >> 1;
    u32 helper_id = *(volatile u32 *)(param_1 + 0x32a);
    iVar10 = cm_fn_009446e0(helper_id,(u32)((s32)next_a + (s32)next_b));
  }
  {
    u32 left = *(volatile u32 *)(param_1 + 0x2c6);
    u32 right = *(volatile u32 *)(param_1 + 0x2d2);
    sVar1 = *(volatile short *)(param_1 + 0x2dc);
    sVar2 = *(volatile short *)(param_1 + 0x2da);
    *(volatile int *)(param_1 + 0x316) = (s32)((left - (u32)iVar10 + 1U + right)) >> 1;
    *(volatile int *)(param_1 + 0x336) = (int)sVar2;
  }
  *(volatile int *)(param_1 + 0x33a) = (int)sVar1;
  sVar2 = *(volatile short *)(param_1 + 0x2de);
  *(volatile int *)(param_1 + 0x33e) = (int)sVar2;
  {
    s16 final_edge = *(volatile short *)(param_1 + 0x2e0);
    *(volatile int *)(param_1 + 0x342) = (int)final_edge;
    if (((sVar2 < final_edge) || (sVar1 < sVar2)) ||
     (*(volatile short *)(param_1 + 0x2da) < sVar1)) {
    uVar7 = 1;
    }
    else {
      uVar7 = 0;
    }
  }
  local_c = (short *)(param_1 + 0x38a);
  *(volatile u32 *)(param_1 + 0x346) = uVar7;
  iVar10 = 0;
  do {
    iVar4 = (int)(short)((short)iVar10 + 0x48);
    sVar1 = *local_c;
    *(volatile short *)(param_1 + 0x1dc + iVar4 * 2) = sVar1;
    if ((param_2 != 0) && (sVar1 != 0)) {
      iVar11 = cm_fn_00936930_sdiv32((u32)(s32)sVar1 * multiplier_29e + (u32)iVar9, iVar15);
      if (iVar11 < 0x21) {
        iVar11 = 0x21;
      }
      *(volatile int *)(param_1 + 0x5c + iVar4 * 4) = iVar11;
    }
    iVar10 = iVar10 + 1;
    local_c = local_c + 1;
  } while (iVar10 < 0xc);
  iVar13 = *(volatile int *)(param_1 + 0x17c);
  local_8 = 1;
  do {
    iVar4 = (int)(short)((short)local_8 + 0x48);
    iVar10 = *(volatile int *)(param_1 + 0x5c + iVar4 * 4);
    if (iVar10 != 0) {
      u32 difference = (u32)iVar13 - (u32)iVar10;
      if ((s32)difference < 0) difference = 0u - difference;
      if ((s32)difference < 0x21) {
        iVar10 = iVar13;
      }
      *(volatile int *)(param_1 + 0x5c + iVar4 * 4) = iVar10;
    }
    local_8 = local_8 + 1;
  } while (local_8 < 0xc);
  local_c = (short *)(param_1 + 0x3a2);
  iVar13 = 0;
  do {
    iVar10 = (int)(short)((short)iVar13 + 0x54);
    sVar1 = *local_c;
    *(volatile short *)(param_1 + 0x1dc + iVar10 * 2) = sVar1;
    if ((param_2 != 0) && (sVar1 != 0)) {
      iVar4 = cm_fn_00936930_sdiv32((u32)(s32)sVar1 * scale_bits + (u32)iVar9, iVar15);
      if (iVar4 < 0x21) {
        iVar4 = 0x21;
      }
      *(volatile int *)(param_1 + 0x5c + iVar10 * 4) = iVar4;
    }
    iVar13 = iVar13 + 1;
    local_c = local_c + 1;
  } while (iVar13 < 0xc);
  iVar13 = *(volatile int *)(param_1 + 0x1ac);
  iVar10 = 1;
  do {
    iVar4 = (int)(short)((short)iVar10 + 0x54);
    iVar14 = *(volatile int *)(param_1 + 0x5c + iVar4 * 4);
    if (iVar14 != 0) {
      u32 difference = (u32)iVar13 - (u32)iVar14;
      if ((s32)difference < 0) difference = 0u - difference;
      if ((s32)difference < 0x21) {
        iVar14 = iVar13;
      }
      *(volatile int *)(param_1 + 0x5c + iVar4 * 4) = iVar14;
    }
    iVar10 = iVar10 + 1;
  } while (iVar10 < 0xc);
  *param_3 = *(volatile int *)(param_1 + 0x17c) < 0x60;
  if (*(volatile int *)(param_1 + 0x448) != 0) {
    *param_3 = 0;
  }
  return 0;
}
