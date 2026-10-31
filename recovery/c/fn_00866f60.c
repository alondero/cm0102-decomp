#include "cmre.h"

u32 CM_THISCALL cm_fn_00866f60(volatile u32 *param_1)

{
  volatile u8 *piVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  short sVar5;
  int iVar6;
  u32 piVar7;
  u32 uVar8;
  char cVar9;
  u8 bVar10;
  char local_b;
  
  u32 table_base = CM_GLOBAL(volatile u32, 0x00ae23b4);
  s32 first_index = *(volatile s32 *)param_1;
  u32 row_base = CM_GLOBAL(volatile u32, 0x00ae23bc);
  piVar1 = (volatile u8 *)(row_base + (u32)first_index * 0x6eu);
  iVar6 = *(volatile s32 *)((volatile u8 *)param_1 + 4);
  u32 list_base = table_base + (u32)iVar6 + (u32)iVar6 * 0x244u;
  piVar7 = *(volatile u32 *)(list_base + 0xcfu);
  if (piVar7 != 0 && piVar1 == (volatile u8 *)piVar7) return 0;
  piVar7 = *(volatile u32 *)(list_base + 0xd3u);
  if (piVar7 != 0 && piVar1 == (volatile u8 *)piVar7) return 0;
  uVar8 = *(volatile u32 *)((volatile u8 *)param_1 + 0x45);
  if (uVar8 != 0) {
    cVar2 = *(volatile s8 *)(piVar1 + 0x5b);
    local_b = *(volatile s8 *)(piVar1 + 0x5a);
    s32 row_value = *(volatile s32 *)piVar1;
    s32 row_remainder = row_value % 7;
    cVar3 = *(volatile s8 *)(piVar1 + 0x59);
    cVar9 = (char)row_remainder + cVar3 + local_b + cVar2;
    if (local_b == '\x01') {
      cVar9 = cVar9 + -2;
    }
    if (cVar3 == '\x01') {
      cVar9 = cVar9 + -3;
    }
    else if (cVar3 < '\x06') {
      cVar9 = cVar9 + -1;
    }
    if (cVar2 == '\x01') {
      cVar9 = cVar9 + -3;
    }
    else if (cVar2 < '\x06') {
      cVar9 = cVar9 + -1;
    }
    char row5d = *(volatile s8 *)(piVar1 + 0x5d);
    if (row5d == '\x01') {
      cVar9 = cVar9 + -2;
    }
    cVar4 = *(volatile s8 *)((volatile u8 *)param_1 + 0x44);
    if (cVar4 < 'L') {
      if (cVar4 < '3') {
        if ('\0' < cVar4) {
          cVar9 = cVar9 + '\x02';
        }
      }
      else {
        cVar9 = cVar9 + '\x05';
      }
    }
    else {
      cVar9 = cVar9 + '\n';
    }
    u32 nested = *(volatile u32 *)(piVar1 + 0x61);
    if (nested != 0) {
      sVar5 = *(volatile s16 *)(nested + 0xb);
      if (sVar5 < 0x1c53) {
        if (0x1676 < sVar5) {
          cVar9 = cVar9 + -3;
        }
      }
      else {
        cVar9 = cVar9 + -5;
      }
    }
    bVar10 = (uVar8 & 0x20000) != 0;
    if ((uVar8 & 0x40000) != 0) {
      bVar10 = bVar10 + 1;
    }
    if ((uVar8 & 0x1000002) != 0) {
      bVar10 = bVar10 + 1;
    }
    if ((uVar8 & 0x200600) != 0) {
      bVar10 = bVar10 + 1;
    }
    if ((uVar8 & 0x2840) != 0) {
      bVar10 = bVar10 + 1;
    }
    if ((uVar8 & 0x14) != 0) {
      bVar10 = bVar10 + 1;
    }
    u32 self40 = *(volatile u32 *)((volatile u8 *)param_1 + 0x40) & 8u;
    if ((self40 != 0) || ((uVar8 & 0x4582208) != 0)) {
      bVar10 = bVar10 + 1;
    }
    if (1 < bVar10) {
      cVar9 = cVar9 + -3;
      if (cVar2 < '\x06') {
        cVar9 = cVar9 - bVar10;
      }
      if (row5d < '\x06') {
        cVar9 = cVar9 - bVar10;
      }
      if (cVar3 == '\x01') {
        cVar9 = cVar9 + bVar10 * -2;
      }
      else if (cVar3 < '\x06') {
        cVar9 = cVar9 - bVar10;
      }
      local_b = *(volatile s8 *)(piVar1 + 0x5a);
      if (local_b < '\x06') {
        cVar9 = cVar9 - bVar10;
      }
    }
    cVar3 = *(volatile s8 *)(piVar1 + 0x59);
    if ((cVar3 < '\x0f') && ((*(volatile u8 *)((volatile u8 *)param_1 + 0x4e) & 1) != 0)) {
      return 1;
    }
    if ((((uVar8 & 0x1000002) != 0) && (cVar3 < '\n')) && (cVar9 < '\x19')) {
      return 1;
    }
    if (((uVar8 & 0x20020) != 0) && (cVar9 < '\x19')) {
      return 1;
    }
    if (((uVar8 & 0x200) != 0) && (cVar9 < '\x12')) {
      return 1;
    }
    if ((((uVar8 & 4) != 0) && ('\x0f' < *(volatile s8 *)(piVar1 + 0x57))) && (cVar2 < '\n')) {
      return 1;
    }
    if ((((uVar8 & 0x2808) != 0) && (cVar3 < '\n')) && ((local_b < '\x06' && (cVar9 < '\x1e')))) {
      return 1;
    }
    if ((((uVar8 & 0x82200) != 0) || (self40 != 0)) &&
       ((cVar2 < '\n' && (cVar9 < '\x14')))) {
      return 1;
    }
    if ((uVar8 & 1) != 0) {
      return 1;
    }
    if ((uVar8 & 0x10) != 0) {
      return 1;
    }
    if ((uVar8 & 0x100) != 0) {
      return 1;
    }
    if ((int)cVar4 < -0x46 - cVar3) {
      return 1;
    }
  }
  return 0;
}
