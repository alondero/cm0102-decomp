/* Draft for preferred-image VA 0x0083c090, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"

void CM_THISCALL cm_fn_0083c090(u32 self,u32 param_1)

{
  u32 iVar1;
  u32 iVar2;
  char cVar3;
  (void)self;
  int iVar4;
  
  cVar3 = '\0';
  iVar2 = 0;
  iVar4 = -0x19;
  do {
    iVar1 = *(volatile u32 *)param_1;
    if (cVar3 < '\x19') {
      *(volatile u8 *)(iVar2 + iVar1) = 0xb;
    }
    else {
      u32 flag_address = (u32)iVar2 + 0x24u + (u32)iVar1;
      u8 old_flags = *(volatile u8 *)flag_address;
      *(volatile u8 *)flag_address = (u8)((old_flags & 0xe3u) | 2u);
      if ((u32)iVar4 <= 0xeu) {
        volatile u32 jump_target = *(volatile u32 *)(0x0083cad4u + (u32)iVar4 * 4u);
        (void)jump_target;
      }
      switch(iVar4) {
      case 0:
        *(volatile u8 *)(iVar2 + *(volatile u32 *)param_1) = 6;
        break;
      case 1:
        *(volatile u8 *)(iVar2 + *(volatile u32 *)param_1) = 7;
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
        *(volatile u8 *)(iVar2 + *(volatile u32 *)param_1) = 8;
        break;
      case 7:
        *(volatile u8 *)(iVar2 + *(volatile u32 *)param_1) = 10;
        break;
      case 8:
      case 9:
      case 10:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
        *(volatile u8 *)(iVar2 + *(volatile u32 *)param_1) = 9;
      }
    }
    iVar2 = iVar2 + 0x25u;
    cVar3 = cVar3 + '\x01';
    iVar4 = iVar4 + 1;
  } while (cVar3 < '(');
  u8 raw_selector = *(volatile u8 *)(param_1 + 7u);
  s32 dispatch_index = (s32)(s8)raw_selector - 1;
  if ((u32)dispatch_index <= 8u) {
    volatile u32 jump_target = *(volatile u32 *)(0x0083cb10u + (u32)dispatch_index * 4u);
    (void)jump_target;
  }
  switch(raw_selector) {
  case 1:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x808;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x88;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 2;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 2;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x32f) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x354) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x379) = 1;
    return;
  case 2:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x808;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x88;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x32f) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x354) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x379) = 1;
    return;
  case 3:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x32f) = 8;
    goto LAB_0083cab8;
  case 4:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x32f) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x354) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x379) = 1;
    return;
  case 5:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x820;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0xa0;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x20;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x32f) = 0x20;
    goto LAB_0083cab8;
  case 6:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x208;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
    iVar2 = *(volatile u32 *)param_1;
    goto LAB_0083cab1;
  case 7:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x208;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    break;
  case 8:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    break;
  case 9:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x210;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x220;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x240;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 0x204;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
    break;
  default:
    *(volatile u16 *)(*(volatile u32 *)param_1 + 1) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x26) = 0x804;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x4b) = 0x84;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x70) = 0x404;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x95) = 0x104;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xba) = 0x810;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0xdf) = 0x90;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x104) = 0x410;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x129) = 0x110;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x14e) = 0x440;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x173) = 0x140;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x198) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1bd) = 8;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x1e2) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x207) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x22c) = 0x20;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x251) = 0x40;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x276) = 4;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x29b) = 0x10;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2c0) = 1;
    *(volatile u16 *)(*(volatile u32 *)param_1 + 0x2e5) = 0x40;
  }
  *(volatile u16 *)(*(volatile u32 *)param_1 + 0x30a) = 4;
  iVar2 = *(volatile u32 *)param_1;
LAB_0083cab1:
  *(volatile u16 *)(iVar2 + 0x32f) = 0x10;
LAB_0083cab8:
  *(volatile u16 *)(*(volatile u32 *)param_1 + 0x354) = 0x10;
  *(volatile u16 *)(*(volatile u32 *)param_1 + 0x379) = 1;
  return;
}
