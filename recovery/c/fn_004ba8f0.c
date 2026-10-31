#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_00ae3138 CM_GLOBAL(volatile u32, 0x00ae3138)
#define DAT_00de1f64 CM_GLOBAL(volatile u32, 0x00de1f64)
#define s_Cannot_find_index__d_009a51b0 ((char *)0x009a51b0)
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

static u32 cm_fn_004ba8f0_map_dispatch(u32 index, u32 byte_map, u32 targets)
{
  u8 selector = *(volatile u8 *)(byte_map + index);
  return *(volatile u32 *)(targets + (u32)selector * 4u);
}

static u32 cm_fn_004ba8f0_direct_dispatch(u32 index, u32 targets)
{
  return *(volatile u32 *)(targets + index * 4u);
}

u32 cm_fn_004ba8f0(short param_1,char param_2)

{
  u32 uVar2;
  
  u32 mode_base = *(volatile u32 *)0x00ae3138u;
  u32 mode = *(volatile u32 *)(mode_base + 8u);
  u32 index = (u32)(s32)param_1;
  if (mode == 3 && param_2 != 3) {
    if (index > 0x483u) {
      (void)cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
    } else {
      u32 target = *(volatile u32 *)(0x004bb390u + index * 4u);
      switch (target) {
      case 0x004ba91eu:
      case 0x004ba953u:
        (void)cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
        break;
      case 0x004bb378u:
        uVar2 = cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
        return uVar2 & 0xffffff00u;
      case 0x004bb38bu: return index & 0xffffff00u;
      case 0x004ba9c9u: return (index & 0xffffff00u) | 2u;
      case 0x004bab33u: return (index & 0xffffff00u) | 3u;
      case 0x004bac77u: return (index & 0xffffff00u) | 1u;
      case 0x004bad97u: return (index & 0xffffff00u) | 4u;
      case 0x004bb1acu: return (index & 0xffffff00u) | 5u;
      default: return index & 0xffffff00u;
      }
    }
  }
  mode_base = *(volatile u32 *)0x00ae3138u;
  mode = *(volatile u32 *)(mode_base + 8u);
  if (mode == 3 && param_2 == 3) {
    if (index > 0x483u) {
      (void)cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
    } else {
      u32 target = *(volatile u32 *)(0x004bc5a0u + index * 4u);
      switch (target) {
      case 0x004ba953u:
        (void)cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
        break;
      case 0x004bb378u:
        uVar2 = cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
        return uVar2 & 0xffffff00u;
      case 0x004bb38bu: return index & 0xffffff00u;
      case 0x004ba9c9u: return (index & 0xffffff00u) | 2u;
      case 0x004bab33u: return (index & 0xffffff00u) | 3u;
      case 0x004bac77u: return (index & 0xffffff00u) | 1u;
      case 0x004bad97u: return (index & 0xffffff00u) | 4u;
      case 0x004bb1acu: return (index & 0xffffff00u) | 5u;
      default: return index & 0xffffff00u;
      }
    }
  }
  mode_base = *(volatile u32 *)0x00ae3138u;
  uVar2 = *(volatile u32 *)(mode_base + 8u);
  if (uVar2 == 2) {
    if (param_2 == '\x03') {
      if (param_1 < 0x32) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 1u;
        if (dispatch_index <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bdbccu,0x004bdbc0u);
        switch(uVar2) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
          goto switchD_004ba9ec_caseD_78;
        case 10:
        case 0x14:
        case 0x1e:
        case 0x28:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else if (param_1 < 100) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0x32u;
        if (dispatch_index <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bdc04u,0x004bdbf8u);
        switch(uVar2) {
        case 0x32:
        case 0x3c:
        case 0x46:
        case 0x50:
        case 0x5a:
          goto switchD_004ba99e_caseD_1;
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else if (param_1 < 0xa0) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0x64u;
        if (dispatch_index <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bdc40u,0x004bdc34u);
        switch(uVar2) {
        case 100:
        case 0x6e:
          goto switchD_004ba99e_caseD_1;
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x78:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else if (param_1 < 1000) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0xa0u;
        if (dispatch_index <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bdc88u,0x004bdc78u);
        switch(uVar2) {
        case 0xa0:
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xbe:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xdd:
        case 0xde:
        case 0xdf:
        case 0xe0:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0xfd:
        case 0xfe:
        case 0x107:
        case 0x108:
        case 0x111:
        case 0x112:
        case 0x118:
        case 0x11b:
        case 0x11c:
        case 0x122:
        case 0x125:
        case 0x126:
        case 300:
        case 0x140:
        case 0x143:
        case 0x144:
        case 0x14a:
        case 0x14d:
        case 0x14e:
        case 0x154:
        case 0x157:
        case 0x158:
        case 0x15e:
        case 0x161:
        case 0x162:
        case 0x168:
        case 0x16b:
        case 0x16c:
          goto switchD_004ba9ec_caseD_78;
        case 200:
        case 0xd2:
        case 0xdc:
        case 0xe6:
        case 0xf0:
        case 0xf3:
        case 0xf4:
        case 0x136:
        case 0x137:
        case 0x138:
        case 0x139:
        case 0x13a:
        case 0x172:
        case 0x175:
        case 0x176:
          goto switchD_004ba99e_caseD_1;
        case 0xfa:
        case 0x104:
        case 0x10e:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x3fc) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0x3e8u;
        if (dispatch_index <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bdd60u);
        switch(uVar2) {
        case 1000:
        case 0x3e9:
        case 0x3ea:
        case 0x3eb:
        case 0x3ec:
        case 0x3ed:
        case 0x3ee:
        case 0x3ef:
        case 0x3f0:
        case 0x3f1:
        case 0x3f2:
        case 0x3f3:
        case 0x3f4:
        case 0x3f5:
        case 0x3f6:
        case 0x3f7:
        case 0x3f8:
        case 0x3f9:
        case 0x3fa:
        case 0x3fb:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else if (param_1 < 0x40c) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0x3fcu;
        if (dispatch_index <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bddb0u);
        switch(uVar2) {
        case 0x3fc:
        case 0x3fd:
        case 0x3fe:
        case 0x3ff:
        case 0x400:
        case 0x401:
        case 0x402:
        case 0x403:
        case 0x404:
        case 0x405:
        case 0x406:
        case 0x407:
        case 0x408:
        case 0x409:
        case 0x40a:
        case 0x40b:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else if (param_1 < 0x418) {
        uVar2 = (u32)param_1;
        u32 dispatch_index = (u32)(s32)param_1 - 0x40cu;
        if (dispatch_index <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bddf0u);
        switch(uVar2) {
        case 0x40c:
        case 0x40d:
        case 0x40e:
          goto switchD_004ba99e_caseD_1;
        case 0x40f:
        case 0x410:
        case 0x413:
        case 0x414:
        case 0x417:
          goto switchD_004ba9ec_caseD_82;
        case 0x411:
        case 0x412:
          goto switchD_004baa77_caseD_411;
        case 0x415:
        case 0x416:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else {
        uVar2 = (u32)param_1;
        if (0x41e < param_1) {
          uVar2 = uVar2 - 0x41f;
          u32 dispatch_index = (u32)(s32)param_1 - 0x41fu;
          if (dispatch_index <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bde3cu);
          switch(uVar2) {
          case 0:
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 9:
          case 10:
          case 0xb:
          case 0xc:
          case 0xd:
          case 0xe:
          case 0xf:
          case 0x10:
          case 0x44:
          case 0x45:
          case 0x46:
          case 0x47:
          case 0x48:
          case 0x49:
          case 0x4a:
          case 0x4b:
          case 0x4d:
          case 0x53:
          case 0x54:
          case 0x55:
          case 0x56:
          case 0x57:
          case 0x58:
          case 0x59:
          case 0x5a:
          case 0x5b:
          case 0x5f:
          case 0x60:
          case 0x61:
          case 0x62:
          case 99:
          case 100:
            goto switchD_004ba99e_caseD_1;
          default:
            goto switchD_004baa77_caseD_411;
          case 0x13:
          case 0x14:
          case 0x15:
          case 0x16:
          case 0x17:
          case 0x18:
          case 0x1a:
          case 0x1b:
          case 0x1c:
          case 0x1d:
          case 0x1e:
          case 0x1f:
          case 0x20:
          case 0x21:
          case 0x22:
          case 0x23:
          case 0x24:
          case 0x25:
          case 0x26:
          case 0x27:
          case 0x28:
          case 0x29:
          case 0x2a:
          case 0x2b:
          case 0x2c:
          case 0x2d:
          case 0x2e:
          case 0x2f:
          case 0x30:
          case 0x31:
          case 0x32:
          case 0x33:
          case 0x34:
          case 0x35:
          case 0x36:
          case 0x37:
          case 0x38:
          case 0x39:
          case 0x3a:
          case 0x3b:
          case 0x3c:
          case 0x3d:
          case 0x3e:
          case 0x3f:
          case 0x40:
          case 0x41:
          case 0x42:
          case 0x43:
          case 0x4c:
          case 0x4e:
          case 0x4f:
          case 0x50:
          case 0x51:
          case 0x52:
          case 0x5c:
          case 0x5d:
          case 0x5e:
            goto switchD_004ba9ec_caseD_82;
          }
        }
        u32 dispatch_index = (u32)(s32)param_1 - 0x418u;
        if (dispatch_index <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bde20u);
        switch(uVar2) {
        case 0x418:
        case 0x419:
          goto switchD_004ba9ec_caseD_78;
        case 0x41a:
        case 0x41b:
        case 0x41d:
        case 0x41e:
switchD_004ba9ec_caseD_82:
          return ((uVar2 & 0xffffff00u) | 1u);
        }
      }
    }
    else if (param_1 < 0x32) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 1u;
      if (dispatch_index <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bd7bcu,0x004bd7b0u);
      switch(uVar2) {
      case 1:
      case 2:
      case 4:
      case 5:
      case 10:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1e:
      case 0x1f:
      case 0x20:
      case 0x21:
      case 0x22:
      case 0x28:
      case 0x29:
      case 0x2a:
      case 0x2b:
      case 0x2c:
        goto switchD_004ba99e_caseD_1;
      case 3:
        goto switchD_004ba99e_caseD_3;
      }
    }
    else if (param_1 < 100) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0x32u;
      if (dispatch_index <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bd7f0u,0x004bd7e8u);
      switch(uVar2) {
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x3c:
      case 0x3d:
      case 0x3e:
      case 0x3f:
      case 0x40:
      case 0x46:
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4a:
      case 0x50:
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x5a:
      case 0x5b:
      case 0x5c:
      case 0x5d:
      case 0x5e:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 0xa0) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0x64u;
      if (dispatch_index <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bd830u,0x004bd820u);
      switch(uVar2) {
      case 100:
      case 0x65:
      case 0x66:
      case 0x67:
      case 0x68:
      case 0x6e:
      case 0x6f:
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x79:
      case 0x7a:
      case 0x7b:
      case 0x7c:
      case 0x7d:
      case 0x87:
      case 0x8c:
      case 0x8d:
      case 0x8e:
      case 0x8f:
      case 0x90:
      case 0x9b:
        goto switchD_004ba99e_caseD_1;
      case 0x78:
        goto switchD_004ba9ec_caseD_78;
      case 0x82:
      case 0x83:
      case 0x84:
      case 0x85:
      case 0x86:
      case 0x96:
      case 0x97:
      case 0x98:
      case 0x99:
      case 0x9a:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 1000) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0xa0u;
      if (dispatch_index <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_index,0x004bd878u,0x004bd868u);
      switch(uVar2) {
      case 0xa0:
      case 0xa3:
      case 0xa4:
      case 0xbe:
      case 0xc1:
      case 0xc2:
      case 0x118:
      case 0x11b:
      case 0x11c:
      case 0x122:
      case 0x125:
      case 0x126:
      case 300:
        goto switchD_004ba9ec_caseD_78;
      case 0xa1:
      case 0xa2:
      case 0xbf:
      case 0xc0:
      case 200:
      case 0xc9:
      case 0xca:
      case 0xcb:
      case 0xcc:
      case 0xd2:
      case 0xd3:
      case 0xd4:
      case 0xd5:
      case 0xd6:
      case 0xdc:
      case 0xdd:
      case 0xde:
      case 0xdf:
      case 0xe0:
      case 0xe6:
      case 0xe7:
      case 0xe8:
      case 0xe9:
      case 0xea:
      case 0xf0:
      case 0xf3:
      case 0xf4:
      case 0x136:
      case 0x137:
      case 0x138:
      case 0x139:
      case 0x13a:
      case 0x172:
      case 0x175:
      case 0x176:
        goto switchD_004ba99e_caseD_1;
      case 0xaa:
      case 0xab:
      case 0xac:
      case 0xad:
      case 0xae:
      case 0xb4:
      case 0xb5:
      case 0xb6:
      case 0xb7:
      case 0xb8:
      case 0xfa:
      case 0xfd:
      case 0xfe:
      case 0x104:
      case 0x107:
      case 0x108:
      case 0x10e:
      case 0x111:
      case 0x112:
      case 0x140:
      case 0x143:
      case 0x144:
      case 0x14a:
      case 0x14d:
      case 0x14e:
      case 0x154:
      case 0x157:
      case 0x158:
      case 0x15e:
      case 0x161:
      case 0x162:
      case 0x168:
      case 0x16b:
      case 0x16c:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x3fc) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0x3e8u;
      if (dispatch_index <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bd950u);
      switch(uVar2) {
      case 1000:
      case 0x3e9:
      case 0x3ea:
      case 0x3eb:
      case 0x3ec:
      case 0x3ed:
      case 0x3ee:
      case 0x3ef:
      case 0x3f0:
      case 0x3f1:
      case 0x3f2:
      case 0x3f3:
      case 0x3f4:
      case 0x3f5:
      case 0x3f6:
      case 0x3f7:
      case 0x3f8:
      case 0x3f9:
      case 0x3fa:
      case 0x3fb:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 0x40c) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0x3fcu;
      if (dispatch_index <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bd9a0u);
      switch(uVar2) {
      case 0x3fc:
        goto switchD_004ba9ec_caseD_78;
      case 0x3fd:
      case 0x3fe:
      case 0x3ff:
      case 0x400:
      case 0x401:
      case 0x402:
      case 0x403:
      case 0x404:
      case 0x405:
      case 0x406:
      case 0x407:
      case 0x408:
      case 0x409:
      case 0x40a:
      case 0x40b:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 0x418) {
      uVar2 = (u32)param_1;
      u32 dispatch_index = (u32)(s32)param_1 - 0x40cu;
      if (dispatch_index <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index,0x004bd9e0u);
      switch(uVar2) {
      case 0x40c:
      case 0x40d:
      case 0x40e:
      case 0x40f:
      case 0x410:
      case 0x413:
      case 0x414:
      case 0x417:
        goto switchD_004ba9ec_caseD_82;
      case 0x411:
      case 0x412:
        goto switchD_004baa77_caseD_411;
      case 0x415:
      case 0x416:
        goto switchD_004ba9ec_caseD_78;
      }
    }
    else {
      uVar2 = (u32)param_1;
      if (param_1 < 0x41f) {
        u32 dispatch_index_418 = (u32)(s32)param_1 - 0x418u;
        if (dispatch_index_418 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index_418,0x004bda10u);
        switch(uVar2) {
        case 0x418:
        case 0x419:
switchD_004ba99e_caseD_1:
          return ((uVar2 & 0xffffff00u) | 2u);
        case 0x41a:
        case 0x41b:
        case 0x41d:
        case 0x41e:
          goto switchD_004ba9ec_caseD_82;
        case 0x41c:
switchD_004ba9ec_caseD_78:
          return ((index & 0xffffff00u) | 3u);
        }
      }
      else {
        u32 dispatch_index_41f = (u32)(s32)param_1 - 0x41fu;
        if (dispatch_index_41f <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_index_41f,0x004bda2cu);
        switch(uVar2) {
        case 0x41f:
        case 0x420:
        case 0x421:
        case 0x422:
        case 0x423:
        case 0x424:
        case 0x425:
        case 0x426:
        case 0x427:
        case 0x428:
        case 0x429:
        case 0x42a:
        case 0x42b:
        case 0x42c:
        case 0x42d:
        case 0x42e:
        case 0x42f:
        case 0x44c:
        case 0x44d:
        case 0x44e:
        case 0x44f:
        case 0x450:
        case 0x453:
        case 0x454:
        case 0x455:
        case 0x456:
        case 0x457:
        case 0x45d:
        case 0x45e:
        case 0x45f:
        case 0x460:
        case 0x461:
        case 0x462:
        case 0x463:
        case 0x464:
        case 0x465:
        case 0x466:
        case 0x467:
        case 0x468:
        case 0x469:
        case 0x46a:
        case 0x46c:
        case 0x46d:
        case 0x46e:
        case 0x46f:
        case 0x470:
        case 0x471:
        case 0x472:
        case 0x473:
        case 0x474:
        case 0x475:
        case 0x476:
        case 0x477:
        case 0x478:
        case 0x479:
        case 0x47a:
        case 0x47e:
        case 0x47f:
        case 0x480:
        case 0x481:
        case 0x482:
        case 0x483:
          goto switchD_004ba99e_caseD_1;
        case 0x430:
        case 0x431:
        case 0x434:
        case 0x435:
        case 0x438:
          goto switchD_004baa77_caseD_411;
        case 0x432:
        case 0x433:
        case 0x436:
        case 0x437:
        case 0x439:
        case 0x43a:
        case 0x43b:
        case 0x43c:
        case 0x43d:
        case 0x43e:
        case 0x43f:
        case 0x440:
        case 0x441:
        case 0x442:
        case 0x443:
        case 0x444:
        case 0x445:
        case 0x446:
        case 0x447:
        case 0x448:
        case 0x449:
        case 0x44a:
        case 0x451:
        case 0x459:
        case 0x45a:
        case 0x45b:
        case 0x45c:
        case 0x46b:
        case 0x47b:
        case 0x47c:
        case 0x47d:
          goto switchD_004ba9ec_caseD_82;
        case 1099:
        case 0x452:
        case 0x458:
          goto switchD_004ba9ec_caseD_78;
        }
      }
    }
  }
  else if (uVar2 == 5) {
    if (param_2 == '\x03') {
      if (param_1 < 0x32) {
        uVar2 = (u32)param_1;
        u32 dispatch_18 = (u32)(s32)param_1 - 1u;
        if (dispatch_18 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_18,0x004be3e8u,0x004be3d8u);
        switch(uVar2) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x2a:
        case 0x2b:
        case 0x2c:
switchD_004bad6c_caseD_1:
          return ((index & 0xffffff00u) | 4u);
        case 5:
          goto switchD_004ba9ec_caseD_78;
        case 10:
        case 0x14:
        case 0x1e:
        case 0x28:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else if (param_1 < 100) {
        uVar2 = (u32)param_1;
        u32 dispatch_19 = (u32)(s32)param_1 - 0x32u;
        if (dispatch_19 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_19,0x004be420u,0x004be414u);
        switch(uVar2) {
        case 0x32:
        case 0x3c:
        case 0x46:
        case 0x50:
        case 0x5a:
          goto switchD_004ba99e_caseD_1;
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
          goto switchD_004bad6c_caseD_1;
        }
      }
      else if (param_1 < 0xa0) {
        uVar2 = (u32)param_1;
        u32 dispatch_20 = (u32)(s32)param_1 - 0x64u;
        if (dispatch_20 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_20,0x004be460u,0x004be450u);
        switch(uVar2) {
        case 100:
        case 0x6e:
          goto switchD_004ba99e_caseD_1;
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
          goto switchD_004bad6c_caseD_1;
        case 0x78:
        case 0x8c:
        case 0x9b:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else if (param_1 < 1000) {
        uVar2 = (u32)param_1;
        u32 dispatch_21 = (u32)(s32)param_1 - 0xa0u;
        if (dispatch_21 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_21,0x004be4acu,0x004be498u);
        switch(uVar2) {
        case 0xa0:
        case 0xbe:
        case 0xfd:
        case 0xfe:
        case 0x107:
        case 0x108:
        case 0x111:
        case 0x112:
        case 0x11b:
        case 0x11c:
        case 0x125:
        case 0x126:
          goto switchD_004ba9ec_caseD_78;
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xdd:
        case 0xde:
        case 0xdf:
        case 0xe0:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0xf3:
        case 0xf4:
        case 0x140:
        case 0x143:
        case 0x144:
        case 0x14a:
        case 0x14d:
        case 0x14e:
        case 0x154:
        case 0x157:
        case 0x158:
        case 0x15e:
        case 0x161:
        case 0x162:
        case 0x168:
        case 0x16b:
        case 0x16c:
          goto switchD_004bad6c_caseD_1;
        case 200:
        case 0xd2:
        case 0xdc:
        case 0xe6:
        case 0xf0:
        case 0xfa:
        case 0x104:
        case 0x10e:
        case 0x137:
        case 0x138:
        case 0x139:
        case 0x13a:
        case 0x175:
        case 0x176:
          goto switchD_004ba99e_caseD_1;
        case 0x118:
        case 0x122:
        case 300:
        case 0x136:
        case 0x172:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x3fc) {
        uVar2 = (u32)param_1;
        u32 dispatch_22 = (u32)(s32)param_1 - 0x3e8u;
        if (dispatch_22 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_22,0x004be584u);
        switch(uVar2) {
        case 1000:
        case 0x3e9:
        case 0x3ea:
        case 0x3eb:
        case 0x3ec:
        case 0x3ed:
        case 0x3ee:
        case 0x3ef:
        case 0x3f0:
        case 0x3f1:
        case 0x3f2:
        case 0x3f3:
        case 0x3f4:
        case 0x3f5:
        case 0x3f6:
        case 0x3f7:
        case 0x3f8:
        case 0x3f9:
        case 0x3fa:
        case 0x3fb:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x40c) {
        uVar2 = (u32)param_1;
        u32 dispatch_23 = (u32)(s32)param_1 - 0x3fcu;
        if (dispatch_23 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_23,0x004be5d4u);
        switch(uVar2) {
        case 0x3fc:
          goto switchD_004ba9ec_caseD_78;
        case 0x3fd:
        case 0x3fe:
        case 0x3ff:
        case 0x400:
        case 0x401:
        case 0x402:
        case 0x403:
        case 0x404:
        case 0x405:
        case 0x406:
        case 0x407:
        case 0x408:
        case 0x409:
        case 0x40a:
        case 0x40b:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x418) {
        uVar2 = (u32)param_1;
        u32 dispatch_24 = (u32)(s32)param_1 - 0x40cu;
        if (dispatch_24 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_24,0x004be614u);
        switch(uVar2) {
        case 0x40c:
        case 0x40d:
        case 0x40e:
        case 0x415:
        case 0x416:
          goto switchD_004ba9ec_caseD_82;
        case 0x40f:
        case 0x410:
        case 0x411:
        case 0x412:
        case 0x413:
        case 0x414:
        case 0x417:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else {
        uVar2 = (u32)param_1;
        if (param_1 < 0x41f) {
          u32 dispatch_25 = (u32)(s32)param_1 - 0x418u;
          if (dispatch_25 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_25,0x004be644u);
          switch(uVar2) {
          case 0x418:
          case 0x419:
          case 0x41a:
          case 0x41b:
            goto switchD_004ba9ec_caseD_82;
          case 0x41c:
          case 0x41d:
          case 0x41e:
            goto switchD_004ba99e_caseD_1;
          }
        }
        else {
          u32 dispatch_26 = (u32)(s32)param_1 - 0x41fu;
          if (dispatch_26 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_26,0x004be660u);
          switch(uVar2) {
          case 0x41f:
          case 0x420:
          case 0x421:
          case 0x422:
          case 0x423:
          case 0x424:
          case 0x425:
          case 0x426:
          case 0x427:
          case 0x428:
          case 0x429:
          case 0x42a:
          case 0x42b:
          case 0x42c:
          case 0x42d:
          case 0x42e:
          case 0x42f:
          case 0x434:
          case 0x435:
          case 0x439:
          case 0x43a:
          case 0x43b:
          case 0x43c:
          case 1099:
          case 0x44c:
          case 0x44d:
          case 0x44e:
          case 0x44f:
          case 0x450:
          case 0x452:
          case 0x453:
          case 0x454:
          case 0x455:
          case 0x456:
          case 0x458:
          case 0x45d:
          case 0x45e:
          case 0x45f:
          case 0x460:
          case 0x461:
          case 0x462:
          case 0x463:
          case 0x464:
          case 0x465:
          case 0x466:
          case 0x467:
          case 0x468:
          case 0x469:
          case 0x46a:
          case 0x46b:
          case 0x46c:
          case 0x46d:
          case 0x46e:
          case 0x46f:
          case 0x470:
          case 0x471:
          case 0x472:
          case 0x473:
          case 0x474:
          case 0x475:
          case 0x476:
          case 0x477:
          case 0x478:
          case 0x479:
          case 0x47a:
          case 0x47e:
          case 0x47f:
          case 0x480:
          case 0x481:
          case 0x482:
          case 0x483:
            goto switchD_004ba9ec_caseD_82;
          case 0x430:
          case 0x431:
          case 0x432:
          case 0x433:
          case 0x436:
          case 0x437:
          case 0x438:
          case 0x43d:
          case 0x43e:
          case 0x43f:
          case 0x440:
          case 0x441:
          case 0x442:
          case 0x443:
          case 0x444:
          case 0x445:
          case 0x446:
          case 0x447:
          case 0x448:
          case 0x449:
          case 0x44a:
          case 0x451:
          case 0x457:
          case 0x459:
          case 0x45a:
          case 0x45b:
          case 0x45c:
          case 0x47b:
          case 0x47c:
          case 0x47d:
            goto switchD_004ba99e_caseD_1;
          }
        }
      }
    }
    else if (param_1 < 0x32) {
      uVar2 = (u32)param_1;
      u32 dispatch_27 = (u32)(s32)param_1 - 1u;
      if (dispatch_27 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_27,0x004bdfdcu,0x004bdfd0u);
      switch(uVar2) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 10:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1e:
      case 0x1f:
      case 0x20:
      case 0x21:
      case 0x22:
      case 0x28:
      case 0x2a:
      case 0x2b:
      case 0x2c:
        goto switchD_004ba99e_caseD_1;
      case 5:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 100) {
      uVar2 = (u32)param_1;
      u32 dispatch_28 = (u32)(s32)param_1 - 0x32u;
      if (dispatch_28 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_28,0x004be010u,0x004be008u);
      switch(uVar2) {
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x3c:
      case 0x3d:
      case 0x3e:
      case 0x3f:
      case 0x40:
      case 0x46:
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4a:
      case 0x50:
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x5a:
      case 0x5b:
      case 0x5c:
      case 0x5d:
      case 0x5e:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 0xa0) {
      uVar2 = (u32)param_1;
      u32 dispatch_29 = (u32)(s32)param_1 - 0x64u;
      if (dispatch_29 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_29,0x004be04cu,0x004be040u);
      switch(uVar2) {
      case 100:
      case 0x65:
      case 0x66:
      case 0x67:
      case 0x68:
      case 0x6e:
      case 0x6f:
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x79:
      case 0x7a:
      case 0x7b:
      case 0x7c:
      case 0x7d:
      case 0x82:
      case 0x83:
      case 0x84:
      case 0x85:
      case 0x86:
      case 0x87:
      case 0x8d:
      case 0x8e:
      case 0x8f:
      case 0x90:
      case 0x96:
      case 0x97:
      case 0x98:
      case 0x99:
      case 0x9a:
        goto switchD_004ba99e_caseD_1;
      case 0x78:
      case 0x8c:
      case 0x9b:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 1000) {
      uVar2 = (u32)param_1;
      u32 dispatch_30 = (u32)(s32)param_1 - 0xa0u;
      if (dispatch_30 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_30,0x004be090u,0x004be084u);
      switch(uVar2) {
      case 0xa0:
      case 0xbe:
      case 0x118:
      case 0x122:
      case 300:
      case 0x136:
      case 0x172:
        goto switchD_004ba9ec_caseD_82;
      case 0xa1:
      case 0xa2:
      case 0xa3:
      case 0xa4:
      case 0xaa:
      case 0xab:
      case 0xac:
      case 0xad:
      case 0xae:
      case 0xb4:
      case 0xb5:
      case 0xb6:
      case 0xb7:
      case 0xb8:
      case 0xbf:
      case 0xc0:
      case 0xc1:
      case 0xc2:
      case 200:
      case 0xc9:
      case 0xca:
      case 0xcb:
      case 0xcc:
      case 0xd2:
      case 0xd3:
      case 0xd4:
      case 0xd5:
      case 0xd6:
      case 0xdc:
      case 0xdd:
      case 0xde:
      case 0xdf:
      case 0xe0:
      case 0xe6:
      case 0xe7:
      case 0xe8:
      case 0xe9:
      case 0xea:
      case 0xf0:
      case 0xf3:
      case 0xf4:
      case 0xfa:
      case 0xfd:
      case 0xfe:
      case 0x104:
      case 0x107:
      case 0x108:
      case 0x10e:
      case 0x111:
      case 0x112:
      case 0x11b:
      case 0x11c:
      case 0x125:
      case 0x126:
      case 0x137:
      case 0x138:
      case 0x139:
      case 0x13a:
      case 0x140:
      case 0x143:
      case 0x144:
      case 0x14a:
      case 0x14d:
      case 0x14e:
      case 0x154:
      case 0x157:
      case 0x158:
      case 0x15e:
      case 0x161:
      case 0x162:
      case 0x168:
      case 0x16b:
      case 0x16c:
      case 0x175:
      case 0x176:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 0x3fc) {
      uVar2 = (u32)param_1;
      u32 dispatch_31 = (u32)(s32)param_1 - 0x3e8u;
      if (dispatch_31 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_31,0x004be168u);
      switch(uVar2) {
      case 1000:
      case 0x3e9:
      case 0x3ea:
      case 0x3eb:
      case 0x3ec:
      case 0x3ed:
      case 0x3ee:
      case 0x3ef:
      case 0x3f0:
      case 0x3f1:
      case 0x3f2:
      case 0x3f3:
      case 0x3f4:
      case 0x3f5:
      case 0x3f6:
      case 0x3f7:
      case 0x3f8:
      case 0x3f9:
      case 0x3fa:
      case 0x3fb:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x40c) {
      uVar2 = (u32)param_1;
      u32 dispatch_32 = (u32)(s32)param_1 - 0x3fcu;
      if (dispatch_32 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_32,0x004be1b8u);
      switch(uVar2) {
      case 0x3fc:
        goto switchD_004ba9ec_caseD_78;
      case 0x3fd:
      case 0x3fe:
      case 0x3ff:
      case 0x400:
      case 0x401:
      case 0x402:
      case 0x403:
      case 0x404:
      case 0x405:
      case 0x406:
      case 0x407:
      case 0x408:
      case 0x409:
      case 0x40a:
      case 0x40b:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x418) {
      uVar2 = (u32)param_1;
      u32 dispatch_33 = (u32)(s32)param_1 - 0x40cu;
      if (dispatch_33 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_33,0x004be1f8u);
      switch(uVar2) {
      case 0x40c:
      case 0x40d:
      case 0x40e:
      case 0x415:
      case 0x416:
        goto switchD_004ba9ec_caseD_82;
      case 0x40f:
      case 0x410:
      case 0x411:
      case 0x412:
      case 0x413:
      case 0x414:
      case 0x417:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else {
      uVar2 = (u32)param_1;
      if (param_1 < 0x41f) {
        u32 dispatch_34 = (u32)(s32)param_1 - 0x418u;
        if (dispatch_34 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_34,0x004be228u);
        switch(uVar2) {
        case 0x418:
        case 0x419:
        case 0x41a:
        case 0x41b:
          goto switchD_004ba9ec_caseD_82;
        case 0x41c:
        case 0x41d:
        case 0x41e:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else {
        u32 dispatch_35 = (u32)(s32)param_1 - 0x41fu;
        if (dispatch_35 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_35,0x004be244u);
        switch(uVar2) {
        case 0x41f:
        case 0x420:
        case 0x421:
        case 0x422:
        case 0x423:
        case 0x424:
        case 0x425:
        case 0x426:
        case 0x427:
        case 0x428:
        case 0x429:
        case 0x42a:
        case 0x42b:
        case 0x42c:
        case 0x42d:
        case 0x42e:
        case 0x42f:
        case 0x434:
        case 0x435:
        case 0x439:
        case 0x43a:
        case 0x43b:
        case 0x43c:
        case 1099:
        case 0x44c:
        case 0x44d:
        case 0x44e:
        case 0x44f:
        case 0x450:
        case 0x452:
        case 0x453:
        case 0x454:
        case 0x455:
        case 0x456:
        case 0x458:
        case 0x45d:
        case 0x45e:
        case 0x45f:
        case 0x460:
        case 0x461:
        case 0x462:
        case 0x463:
        case 0x464:
        case 0x465:
        case 0x466:
        case 0x467:
        case 0x468:
        case 0x469:
        case 0x46a:
        case 0x46b:
        case 0x46d:
        case 0x46e:
        case 0x46f:
        case 0x470:
        case 0x471:
        case 0x472:
        case 0x473:
        case 0x474:
        case 0x475:
        case 0x476:
        case 0x477:
        case 0x478:
        case 0x479:
        case 0x47a:
        case 0x47e:
        case 0x47f:
        case 0x480:
        case 0x481:
        case 0x482:
        case 0x483:
          goto switchD_004ba9ec_caseD_82;
        case 0x430:
        case 0x431:
        case 0x432:
        case 0x433:
        case 0x436:
        case 0x437:
        case 0x438:
        case 0x43d:
        case 0x43e:
        case 0x43f:
        case 0x440:
        case 0x441:
        case 0x442:
        case 0x443:
        case 0x444:
        case 0x445:
        case 0x446:
        case 0x447:
        case 0x448:
        case 0x449:
        case 0x44a:
        case 0x451:
        case 0x457:
        case 0x459:
        case 0x45a:
        case 0x45b:
        case 0x45c:
        case 0x46c:
        case 0x47b:
        case 0x47c:
        case 0x47d:
          goto switchD_004ba99e_caseD_1;
        }
      }
    }
  }
  else if (uVar2 == 9) {
    if (param_2 == '\x03') {
      if (param_1 < 0x32) {
        uVar2 = (u32)param_1;
        u32 dispatch_36 = (u32)(s32)param_1 - 1u;
        if (dispatch_36 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_36,0x004bec0cu,0x004bec00u);
        switch(uVar2) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1e:
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x28:
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
          goto switchD_004ba9ec_caseD_78;
        case 5:
          goto switchD_004baa77_caseD_411;
        }
      }
      else if (param_1 < 100) {
        u32 dispatch_37 = (u32)(s32)param_1 - 0x32u;
        if (dispatch_37 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_37,0x004bec40u,0x004bec38u);
        switch(param_1) {
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x3c:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else if (param_1 < 0xa0) {
        uVar2 = (u32)param_1;
        u32 dispatch_38 = (u32)(s32)param_1 - 0x64u;
        if (dispatch_38 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_38,0x004bec7cu,0x004bec70u);
        switch(uVar2) {
        case 100:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x6e:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x8c:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
          goto switchD_004ba9ec_caseD_78;
        case 0x78:
        case 0x87:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else if (param_1 < 1000) {
        uVar2 = (u32)param_1;
        u32 dispatch_39 = (u32)(s32)param_1 - 0xa0u;
        if (dispatch_39 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_39,0x004becc8u,0x004becb4u);
        switch(uVar2) {
        case 0xa0:
        case 0xbe:
          goto switchD_004ba99e_caseD_1;
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 200:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xdc:
        case 0xdd:
        case 0xde:
        case 0xdf:
        case 0xe0:
        case 0xe6:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0xf0:
        case 0xf3:
        case 0xf4:
        case 0x136:
        case 0x137:
        case 0x138:
        case 0x139:
        case 0x13a:
        case 0x140:
        case 0x143:
        case 0x144:
        case 0x14a:
        case 0x14d:
        case 0x14e:
        case 0x154:
        case 0x157:
        case 0x158:
        case 0x15e:
        case 0x161:
        case 0x162:
        case 0x168:
        case 0x16b:
        case 0x16c:
        case 0x172:
        case 0x175:
        case 0x176:
          goto switchD_004ba9ec_caseD_78;
        case 0xfa:
        case 0xfd:
        case 0xfe:
        case 0x104:
        case 0x107:
        case 0x108:
        case 0x10e:
        case 0x111:
        case 0x112:
        case 300:
          goto switchD_004ba9ec_caseD_82;
        case 0x118:
        case 0x11b:
        case 0x11c:
        case 0x122:
        case 0x125:
        case 0x126:
          goto switchD_004baa77_caseD_411;
        }
      }
      else if (param_1 < 0x3fc) {
        uVar2 = (u32)param_1;
        u32 dispatch_40 = (u32)(s32)param_1 - 0x3e8u;
        if (dispatch_40 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_40,0x004beda0u);
        switch(uVar2) {
        case 1000:
        case 0x3e9:
        case 0x3ea:
        case 0x3eb:
        case 0x3ec:
        case 0x3ed:
        case 0x3ee:
        case 0x3ef:
        case 0x3f0:
        case 0x3f1:
        case 0x3f2:
        case 0x3f3:
        case 0x3f4:
        case 0x3f5:
        case 0x3f6:
        case 0x3f7:
        case 0x3f8:
        case 0x3f9:
        case 0x3fa:
        case 0x3fb:
          goto switchD_004baa77_caseD_411;
        }
      }
      else if (param_1 < 0x40c) {
        uVar2 = (u32)param_1;
        u32 dispatch_41 = (u32)(s32)param_1 - 0x3fcu;
        if (dispatch_41 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_41,0x004bedf0u);
        switch(uVar2) {
        case 0x3fc:
          goto switchD_004ba9ec_caseD_78;
        case 0x3fd:
        case 0x3fe:
        case 0x3ff:
        case 0x400:
        case 0x401:
        case 0x402:
        case 0x403:
        case 0x404:
        case 0x405:
        case 0x406:
        case 0x407:
        case 0x408:
        case 0x409:
        case 0x40a:
        case 0x40b:
          goto switchD_004baa77_caseD_411;
        }
      }
      else if (param_1 < 0x418) {
        uVar2 = (u32)param_1;
        u32 dispatch_42 = (u32)(s32)param_1 - 0x40cu;
        if (dispatch_42 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_42,0x004bee30u);
        switch(uVar2) {
        case 0x40c:
        case 0x40d:
        case 0x40e:
          goto switchD_004baa77_caseD_411;
        case 0x40f:
        case 0x410:
        case 0x413:
        case 0x414:
        case 0x417:
          goto switchD_004ba9ec_caseD_82;
        case 0x411:
        case 0x412:
        case 0x415:
        case 0x416:
          goto switchD_004ba99e_caseD_1;
        }
      }
      else {
        uVar2 = (u32)param_1;
        if (param_1 < 0x41f) {
          u32 dispatch_43 = (u32)(s32)param_1 - 0x418u;
          if (dispatch_43 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_43,0x004bee60u);
          switch(uVar2) {
          case 0x418:
          case 0x419:
            goto switchD_004baa77_caseD_411;
          case 0x41a:
          case 0x41b:
          case 0x41d:
          case 0x41e:
            goto switchD_004ba9ec_caseD_82;
          }
        }
        else {
          u32 dispatch_44 = (u32)(s32)param_1 - 0x41fu;
          if (dispatch_44 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_44,0x004bee7cu);
          switch(uVar2) {
          case 0x41f:
          case 0x420:
          case 0x421:
          case 0x422:
          case 0x423:
          case 0x424:
          case 0x425:
          case 0x426:
          case 0x427:
          case 0x428:
          case 0x429:
          case 0x42a:
          case 0x42b:
          case 0x42c:
          case 0x42d:
          case 0x42e:
          case 0x42f:
          case 0x430:
          case 0x431:
          case 0x438:
          case 0x439:
          case 0x43a:
          case 0x43b:
          case 0x43c:
          case 0x463:
          case 0x464:
          case 0x465:
          case 0x466:
          case 0x467:
          case 0x468:
          case 0x469:
          case 0x46a:
          case 0x472:
          case 0x473:
          case 0x474:
          case 0x475:
          case 0x476:
          case 0x477:
          case 0x478:
          case 0x479:
          case 0x47a:
          case 0x47e:
          case 0x47f:
          case 0x480:
          case 0x481:
          case 0x482:
          case 0x483:
            goto switchD_004baa77_caseD_411;
          case 0x432:
          case 0x433:
          case 0x434:
          case 0x435:
          case 0x436:
          case 0x437:
          case 0x43d:
          case 0x43e:
          case 0x43f:
          case 0x440:
          case 0x441:
          case 0x442:
          case 0x443:
          case 0x444:
          case 0x445:
          case 0x446:
          case 0x447:
          case 0x448:
          case 0x449:
          case 0x44a:
          case 1099:
          case 0x44c:
          case 0x44d:
          case 0x44e:
          case 0x44f:
          case 0x450:
          case 0x451:
          case 0x452:
          case 0x453:
          case 0x454:
          case 0x455:
          case 0x456:
          case 0x457:
          case 0x458:
          case 0x459:
          case 0x45a:
          case 0x45b:
          case 0x45c:
          case 0x45d:
          case 0x45e:
          case 0x45f:
          case 0x460:
          case 0x461:
          case 0x462:
          case 0x46b:
          case 0x46c:
          case 0x46d:
          case 0x46e:
          case 0x46f:
          case 0x470:
          case 0x471:
          case 0x47b:
          case 0x47c:
          case 0x47d:
            goto switchD_004ba9ec_caseD_82;
          }
        }
      }
    }
    else if (param_1 < 0x32) {
      uVar2 = (u32)param_1;
      u32 dispatch_45 = (u32)(s32)param_1 - 1u;
      if (dispatch_45 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_45,0x004be800u,0x004be7f4u);
      switch(uVar2) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 10:
      case 0xb:
      case 0xc:
      case 0xd:
      case 0xe:
      case 0x14:
      case 0x15:
      case 0x16:
      case 0x17:
      case 0x18:
      case 0x19:
      case 0x1e:
      case 0x1f:
      case 0x20:
      case 0x21:
      case 0x22:
      case 0x28:
      case 0x29:
      case 0x2a:
      case 0x2b:
      case 0x2c:
        goto switchD_004ba9ec_caseD_82;
      case 5:
        goto switchD_004baa77_caseD_411;
      }
    }
    else if (param_1 < 100) {
      uVar2 = (u32)param_1;
      u32 dispatch_46 = (u32)(s32)param_1 - 0x32u;
      if (dispatch_46 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_46,0x004be834u,0x004be82cu);
      switch(uVar2) {
      case 0x32:
      case 0x33:
      case 0x34:
      case 0x35:
      case 0x36:
      case 0x3c:
      case 0x3d:
      case 0x3e:
      case 0x3f:
      case 0x40:
      case 0x46:
      case 0x47:
      case 0x48:
      case 0x49:
      case 0x4a:
      case 0x50:
      case 0x51:
      case 0x52:
      case 0x53:
      case 0x54:
      case 0x5a:
      case 0x5b:
      case 0x5c:
      case 0x5d:
      case 0x5e:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0xa0) {
      uVar2 = (u32)param_1;
      u32 dispatch_47 = (u32)(s32)param_1 - 0x64u;
      if (dispatch_47 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_47,0x004be874u,0x004be864u);
      switch(uVar2) {
      case 100:
      case 0x65:
      case 0x66:
      case 0x67:
      case 0x68:
      case 0x6e:
      case 0x6f:
      case 0x70:
      case 0x71:
      case 0x72:
      case 0x79:
      case 0x7a:
      case 0x7b:
      case 0x7c:
      case 0x7d:
      case 0x82:
      case 0x83:
      case 0x84:
      case 0x85:
      case 0x86:
      case 0x8d:
      case 0x8e:
      case 0x8f:
      case 0x90:
      case 0x96:
      case 0x97:
      case 0x98:
      case 0x99:
      case 0x9a:
      case 0x9b:
        goto switchD_004ba9ec_caseD_82;
      case 0x78:
        goto switchD_004ba99e_caseD_1;
      case 0x87:
      case 0x8c:
        goto switchD_004baa77_caseD_411;
      }
    }
    else if (param_1 < 1000) {
      uVar2 = (u32)param_1;
      u32 dispatch_48 = (u32)(s32)param_1 - 0xa0u;
      if (dispatch_48 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_48,0x004be8b8u,0x004be8acu);
      switch(uVar2) {
      case 0xa0:
      case 0xaa:
      case 0xbe:
      case 0x118:
      case 0x11b:
      case 0x11c:
      case 0x122:
      case 0x125:
      case 0x126:
      case 0x136:
      case 0x172:
        goto switchD_004baa77_caseD_411;
      case 0xa1:
      case 0xa2:
      case 0xa3:
      case 0xa4:
      case 0xab:
      case 0xac:
      case 0xad:
      case 0xae:
      case 0xb4:
      case 0xb5:
      case 0xb6:
      case 0xb7:
      case 0xb8:
      case 0xbf:
      case 0xc0:
      case 0xc1:
      case 0xc2:
      case 200:
      case 0xc9:
      case 0xca:
      case 0xcb:
      case 0xcc:
      case 0xd2:
      case 0xd3:
      case 0xd4:
      case 0xd5:
      case 0xd6:
      case 0xdc:
      case 0xdd:
      case 0xde:
      case 0xdf:
      case 0xe0:
      case 0xe6:
      case 0xe7:
      case 0xe8:
      case 0xe9:
      case 0xea:
      case 0xf0:
      case 0xf3:
      case 0xf4:
      case 0xfa:
      case 0xfd:
      case 0xfe:
      case 0x104:
      case 0x107:
      case 0x108:
      case 0x10e:
      case 0x111:
      case 0x112:
      case 300:
      case 0x137:
      case 0x138:
      case 0x139:
      case 0x13a:
      case 0x140:
      case 0x143:
      case 0x144:
      case 0x14a:
      case 0x14d:
      case 0x14e:
      case 0x154:
      case 0x157:
      case 0x158:
      case 0x15e:
      case 0x161:
      case 0x162:
      case 0x168:
      case 0x16b:
      case 0x16c:
      case 0x175:
      case 0x176:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x3fc) {
      uVar2 = (u32)param_1;
      u32 dispatch_49 = (u32)(s32)param_1 - 0x3e8u;
      if (dispatch_49 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_49,0x004be990u);
      switch(uVar2) {
      case 1000:
      case 0x3e9:
      case 0x3ea:
      case 0x3eb:
      case 0x3ec:
      case 0x3ed:
      case 0x3ee:
      case 0x3ef:
      case 0x3f0:
      case 0x3f1:
      case 0x3f2:
      case 0x3f3:
      case 0x3f4:
      case 0x3f5:
      case 0x3f6:
      case 0x3f7:
      case 0x3f8:
      case 0x3f9:
      case 0x3fa:
      case 0x3fb:
        goto switchD_004baa77_caseD_411;
      }
    }
    else if (param_1 < 0x40c) {
      uVar2 = (u32)param_1;
      u32 dispatch_50 = (u32)(s32)param_1 - 0x3fcu;
      if (dispatch_50 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_50,0x004be9e0u);
      switch(uVar2) {
      case 0x3fc:
        goto switchD_004ba9ec_caseD_78;
      case 0x3fd:
      case 0x3fe:
      case 0x3ff:
      case 0x400:
      case 0x401:
      case 0x402:
      case 0x403:
      case 0x404:
      case 0x405:
      case 0x406:
      case 0x407:
      case 0x408:
      case 0x409:
      case 0x40a:
      case 0x40b:
        goto switchD_004baa77_caseD_411;
      }
    }
    else if (param_1 < 0x418) {
      uVar2 = (u32)param_1;
      u32 dispatch_51 = (u32)(s32)param_1 - 0x40cu;
      if (dispatch_51 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_51,0x004bea20u);
      switch(uVar2) {
      case 0x40c:
      case 0x40d:
      case 0x40e:
      case 0x40f:
      case 0x410:
      case 0x413:
      case 0x414:
      case 0x417:
        goto switchD_004ba9ec_caseD_82;
      case 0x411:
      case 0x412:
      case 0x415:
      case 0x416:
        goto switchD_004baa77_caseD_411;
      }
    }
    else {
      uVar2 = (u32)param_1;
      if (param_1 < 0x41f) {
        u32 dispatch_52 = (u32)(s32)param_1 - 0x418u;
        if (dispatch_52 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_52,0x004bea50u);
        switch(uVar2) {
        case 0x418:
        case 0x419:
          goto switchD_004baa77_caseD_411;
        case 0x41a:
        case 0x41b:
        case 0x41c:
        case 0x41d:
        case 0x41e:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else {
        u32 dispatch_53 = (u32)(s32)param_1 - 0x41fu;
        if (dispatch_53 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_53,0x004bea6cu);
        switch(uVar2) {
        case 0x41f:
        case 0x420:
        case 0x421:
        case 0x422:
        case 0x423:
        case 0x424:
        case 0x425:
        case 0x426:
        case 0x427:
        case 0x428:
        case 0x429:
        case 0x42a:
        case 0x42b:
        case 0x42c:
        case 0x42d:
        case 0x42e:
        case 0x42f:
        case 0x430:
        case 0x431:
        case 0x439:
        case 0x43a:
        case 0x43b:
        case 0x43c:
        case 1099:
        case 0x44c:
        case 0x452:
        case 0x453:
        case 0x454:
        case 0x455:
        case 0x456:
        case 0x457:
        case 0x458:
        case 0x45d:
        case 0x45e:
        case 0x45f:
        case 0x460:
        case 0x461:
        case 0x462:
        case 0x463:
        case 0x464:
        case 0x465:
        case 0x466:
        case 0x467:
        case 0x468:
        case 0x469:
        case 0x46a:
        case 0x46c:
        case 0x46d:
        case 0x46e:
        case 0x46f:
        case 0x470:
        case 0x471:
        case 0x472:
        case 0x473:
        case 0x474:
        case 0x475:
        case 0x476:
        case 0x477:
        case 0x478:
        case 0x479:
        case 0x47a:
        case 0x47e:
        case 0x47f:
        case 0x480:
        case 0x481:
        case 0x482:
        case 0x483:
          goto switchD_004baa77_caseD_411;
        case 0x432:
        case 0x433:
        case 0x434:
        case 0x435:
        case 0x436:
        case 0x437:
        case 0x438:
        case 0x43d:
        case 0x43e:
        case 0x43f:
        case 0x440:
        case 0x441:
        case 0x442:
        case 0x443:
        case 0x444:
        case 0x445:
        case 0x446:
        case 0x447:
        case 0x448:
        case 0x449:
        case 0x44a:
        case 0x44d:
        case 0x44e:
        case 0x44f:
        case 0x450:
        case 0x451:
        case 0x459:
        case 0x45a:
        case 0x45b:
        case 0x45c:
        case 0x46b:
        case 0x47b:
        case 0x47c:
        case 0x47d:
          goto switchD_004ba9ec_caseD_82;
        }
      }
    }
  }
  else {
    if (uVar2 != 4) goto switchD_004baa77_caseD_411;
    if (param_2 == '\x03') {
      if (param_1 < 0x32) {
        u32 dispatch_54 = (u32)(s32)param_1 - 1u;
        if (dispatch_54 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_54,0x004bf444u,0x004bf438u);
        switch(param_1) {
        case 1:
        case 2:
        case 5:
          goto switchD_004ba9ec_caseD_78;
        case 3:
        case 4:
        case 10:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1e:
        case 0x1f:
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x28:
        case 0x29:
        case 0x2a:
        case 0x2b:
        case 0x2c:
switchD_004bb1ea_caseD_3fc:
          return ((index & 0xffffff00u) | 7u);
        }
      }
      else if (param_1 < 100) {
        u32 dispatch_55 = (u32)(s32)param_1 - 0x32u;
        if (dispatch_55 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_55,0x004bf478u,0x004bf470u);
        switch(param_1) {
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x3c:
        case 0x3d:
        case 0x3e:
        case 0x3f:
        case 0x40:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4a:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x5a:
        case 0x5b:
        case 0x5c:
        case 0x5d:
        case 0x5e:
          goto switchD_004bb1ea_caseD_3fc;
        }
      }
      else if (param_1 < 0xa0) {
        u32 dispatch_56 = (u32)(s32)param_1 - 0x64u;
        if (dispatch_56 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_56,0x004bf4b8u,0x004bf4a8u);
        switch(param_1) {
        case 100:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x6e:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x79:
        case 0x7a:
        case 0x7b:
        case 0x7c:
        case 0x7d:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x8d:
        case 0x8e:
        case 0x8f:
        case 0x90:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9a:
        case 0x9b:
          goto switchD_004bb1ea_caseD_3fc;
        case 0x78:
          goto switchD_004ba99e_caseD_3;
        case 0x8c:
          goto switchD_004ba9ec_caseD_78;
        }
      }
      else if (param_1 < 1000) {
        uVar2 = (u32)param_1;
        u32 dispatch_57 = (u32)(s32)param_1 - 0xa0u;
        if (dispatch_57 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_57,0x004bf504u,0x004bf4f0u);
        switch(uVar2) {
        case 0xa0:
        case 0xbe:
          goto switchD_004ba99e_caseD_3;
        case 0xa1:
        case 0xa2:
        case 0xa3:
        case 0xa4:
        case 0xaa:
        case 0xab:
        case 0xac:
        case 0xad:
        case 0xae:
        case 0xb4:
        case 0xb5:
        case 0xb6:
        case 0xb7:
        case 0xb8:
        case 0xbf:
        case 0xc0:
        case 0xc1:
        case 0xc2:
        case 200:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xd2:
        case 0xd3:
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xdc:
        case 0xdd:
        case 0xde:
        case 0xdf:
        case 0xe0:
        case 0xe6:
        case 0xe7:
        case 0xe8:
        case 0xe9:
        case 0xea:
        case 0x137:
        case 0x138:
        case 0x139:
        case 0x13a:
        case 0x140:
        case 0x143:
        case 0x144:
        case 0x14a:
        case 0x14d:
        case 0x14e:
        case 0x154:
        case 0x157:
        case 0x158:
        case 0x15e:
        case 0x161:
        case 0x162:
        case 0x168:
        case 0x16b:
        case 0x16c:
        case 0x175:
        case 0x176:
          goto switchD_004bb1ea_caseD_3fc;
        case 0xf0:
        case 0xf3:
        case 0xf4:
        case 0x136:
        case 0x172:
          goto switchD_004ba9ec_caseD_78;
        case 0xfa:
        case 0xfd:
        case 0xfe:
        case 0x104:
        case 0x107:
        case 0x108:
        case 0x10e:
        case 0x111:
        case 0x112:
        case 0x118:
        case 0x11b:
        case 0x11c:
        case 0x122:
        case 0x125:
        case 0x126:
        case 300:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x3fc) {
        uVar2 = (u32)param_1;
        u32 dispatch_58 = (u32)(s32)param_1 - 0x3e8u;
        if (dispatch_58 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_58,0x004bf5dcu);
        switch(uVar2) {
        case 1000:
        case 0x3e9:
        case 0x3ea:
        case 0x3eb:
        case 0x3ec:
        case 0x3ed:
        case 0x3ee:
        case 0x3ef:
        case 0x3f0:
        case 0x3f1:
        case 0x3f2:
        case 0x3f3:
        case 0x3f4:
        case 0x3f5:
        case 0x3f6:
        case 0x3f7:
        case 0x3f8:
        case 0x3f9:
        case 0x3fa:
        case 0x3fb:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x40c) {
        uVar2 = (u32)param_1;
        u32 dispatch_59 = (u32)(s32)param_1 - 0x3fcu;
        if (dispatch_59 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_59,0x004bf62cu);
        switch(uVar2) {
        case 0x3fc:
          goto switchD_004bb1ea_caseD_3fc;
        case 0x3fd:
        case 0x3fe:
        case 0x3ff:
        case 0x400:
        case 0x401:
        case 0x402:
        case 0x403:
        case 0x404:
        case 0x405:
        case 0x406:
        case 0x407:
        case 0x408:
        case 0x409:
        case 0x40a:
        case 0x40b:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else if (param_1 < 0x418) {
        uVar2 = (u32)param_1;
        u32 dispatch_60 = (u32)(s32)param_1 - 0x40cu;
        if (dispatch_60 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_60,0x004bf66cu);
        switch(uVar2) {
        case 0x40c:
        case 0x40d:
        case 0x40e:
        case 0x40f:
        case 0x410:
        case 0x413:
        case 0x414:
        case 0x417:
          goto switchD_004ba9ec_caseD_78;
        case 0x411:
        case 0x412:
          goto switchD_004ba9ec_caseD_82;
        case 0x415:
        case 0x416:
          goto switchD_004ba99e_caseD_3;
        }
      }
      else {
        uVar2 = (u32)param_1;
        if (param_1 < 0x41f) {
          u32 dispatch_61 = (u32)(s32)param_1 - 0x418u;
          if (dispatch_61 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_61,0x004bf69cu);
          switch(uVar2) {
          case 0x418:
          case 0x419:
            goto switchD_004ba99e_caseD_3;
          case 0x41a:
          case 0x41b:
          case 0x41c:
            goto switchD_004ba9ec_caseD_78;
          case 0x41d:
          case 0x41e:
            goto switchD_004ba9ec_caseD_82;
          }
        }
        else {
          u32 dispatch_62 = (u32)(s32)param_1 - 0x41fu;
          if (dispatch_62 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_62,0x004bf6b8u);
          switch(uVar2) {
          case 0x41f:
          case 0x420:
          case 0x421:
          case 0x422:
          case 0x423:
          case 0x424:
          case 0x425:
          case 0x426:
          case 0x427:
          case 0x428:
          case 0x429:
          case 0x42a:
          case 0x42b:
          case 0x42c:
          case 0x42d:
          case 0x42e:
          case 0x434:
          case 0x435:
          case 0x439:
          case 0x43a:
          case 0x43b:
          case 0x43c:
          case 0x43d:
          case 0x43e:
          case 0x43f:
          case 0x440:
          case 0x441:
          case 0x442:
          case 0x443:
          case 0x444:
          case 0x445:
          case 0x446:
          case 0x447:
          case 0x448:
          case 0x449:
          case 0x44a:
          case 1099:
          case 0x44c:
          case 0x44d:
          case 0x44e:
          case 0x44f:
          case 0x450:
          case 0x451:
          case 0x452:
          case 0x453:
          case 0x454:
          case 0x455:
          case 0x456:
          case 0x457:
          case 0x458:
          case 0x459:
          case 0x45a:
          case 0x45b:
          case 0x45c:
          case 0x45d:
          case 0x45e:
          case 0x45f:
          case 0x460:
          case 0x461:
          case 0x462:
          case 0x463:
          case 0x464:
          case 0x465:
          case 0x466:
          case 0x467:
          case 0x468:
          case 0x469:
          case 0x46a:
          case 0x46b:
          case 0x46c:
          case 0x46d:
          case 0x46e:
          case 0x46f:
          case 0x470:
          case 0x471:
          case 0x472:
          case 0x473:
          case 0x474:
          case 0x475:
          case 0x476:
          case 0x477:
          case 0x478:
          case 0x479:
          case 0x47a:
          case 0x47c:
          case 0x47d:
          case 0x47e:
          case 0x47f:
          case 0x480:
          case 0x481:
          case 0x482:
          case 0x483:
            goto switchD_004ba9ec_caseD_82;
          case 0x42f:
            goto switchD_004ba99e_caseD_3;
          case 0x430:
          case 0x431:
          case 0x432:
          case 0x433:
          case 0x436:
          case 0x437:
          case 0x438:
          case 0x47b:
            goto switchD_004ba9ec_caseD_78;
          }
        }
      }
    }
    else if (param_1 < 0x32) {
      uVar2 = (u32)param_1;
      u32 dispatch_63 = (u32)(s32)param_1 - 1u;
      if (dispatch_63 <= 0x2bu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_63,0x004bf020u,0x004bf010u);
      switch(uVar2) {
      case 1:
      case 2:
      case 5:
      case 0xb:
      case 0xc:
      case 0x15:
      case 0x16:
      case 0x1f:
      case 0x20:
      case 0x29:
      case 0x2a:
        goto switchD_004ba9ec_caseD_78;
      case 3:
      case 0xd:
      case 0x17:
      case 0x21:
      case 0x2b:
        goto switchD_004bad6c_caseD_1;
      case 4:
      case 10:
      case 0xe:
      case 0x14:
      case 0x18:
      case 0x19:
      case 0x1e:
      case 0x22:
      case 0x28:
      case 0x2c:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 100) {
      uVar2 = (u32)param_1;
      u32 dispatch_64 = (u32)(s32)param_1 - 0x32u;
      if (dispatch_64 <= 0x2cu) (void)cm_fn_004ba8f0_map_dispatch(dispatch_64,0x004bf05cu,0x004bf04cu);
      switch(uVar2) {
      case 0x32:
      case 0x36:
      case 0x3c:
      case 0x40:
      case 0x46:
      case 0x4a:
      case 0x50:
      case 0x54:
      case 0x5a:
      case 0x5e:
        goto switchD_004ba9ec_caseD_82;
      case 0x33:
      case 0x34:
      case 0x3d:
      case 0x3e:
      case 0x47:
      case 0x48:
      case 0x51:
      case 0x52:
      case 0x5b:
      case 0x5c:
        goto switchD_004ba9ec_caseD_78;
      case 0x35:
      case 0x3f:
      case 0x49:
      case 0x53:
      case 0x5d:
        goto switchD_004bad6c_caseD_1;
      }
    }
    else if (param_1 < 0xa0) {
      uVar2 = (u32)param_1;
      u32 dispatch_65 = (u32)(s32)param_1 - 0x64u;
      if (dispatch_65 <= 0x37u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_65,0x004bf0a4u,0x004bf08cu);
      switch(uVar2) {
      case 100:
      case 0x68:
      case 0x6e:
      case 0x72:
      case 0x7c:
      case 0x90:
        goto switchD_004ba9ec_caseD_82;
      case 0x65:
      case 0x66:
      case 0x6f:
      case 0x70:
      case 0x79:
      case 0x7a:
      case 0x82:
      case 0x83:
      case 0x84:
      case 0x85:
      case 0x86:
      case 0x8c:
      case 0x8d:
      case 0x8e:
      case 0x96:
      case 0x97:
      case 0x98:
      case 0x99:
      case 0x9a:
        goto switchD_004ba9ec_caseD_78;
      case 0x67:
      case 0x71:
      case 0x7b:
      case 0x8f:
        goto switchD_004bad6c_caseD_1;
      case 0x78:
        goto switchD_004ba99e_caseD_3;
      case 0x7d:
      case 0x87:
      case 0x9b:
        goto switchD_004ba99e_caseD_1;
      }
    }
    else if (param_1 < 1000) {
      uVar2 = (u32)param_1;
      u32 dispatch_66 = (u32)(s32)param_1 - 0xa0u;
      if (dispatch_66 <= 0xd6u) (void)cm_fn_004ba8f0_map_dispatch(dispatch_66,0x004bf0f0u,0x004bf0dcu);
      switch(uVar2) {
      case 0xa0:
      case 0xa3:
      case 0xa4:
      case 0xbe:
      case 0xc1:
      case 0xc2:
        goto switchD_004ba99e_caseD_3;
      case 0xa1:
      case 0xcb:
      case 0xd5:
      case 0xdf:
      case 0xe9:
      case 0x139:
      case 0x175:
        goto switchD_004bad6c_caseD_1;
      case 0xa2:
      case 0xaa:
      case 0xab:
      case 0xac:
      case 0xad:
      case 0xae:
      case 0xb4:
      case 0xb5:
      case 0xb6:
      case 0xb7:
      case 0xb8:
      case 0xbf:
      case 0xc0:
      case 0xc9:
      case 0xca:
      case 0xd3:
      case 0xd4:
      case 0xdd:
      case 0xde:
      case 0xe7:
      case 0xe8:
      case 0xf0:
      case 0xf3:
      case 0xf4:
      case 0x136:
      case 0x137:
      case 0x138:
      case 0x140:
      case 0x143:
      case 0x144:
      case 0x14a:
      case 0x14d:
      case 0x14e:
      case 0x154:
      case 0x157:
      case 0x158:
      case 0x15e:
      case 0x161:
      case 0x162:
      case 0x168:
      case 0x16b:
      case 0x16c:
      case 0x172:
        goto switchD_004ba9ec_caseD_78;
      case 200:
      case 0xcc:
      case 0xd2:
      case 0xd6:
      case 0xdc:
      case 0xe0:
      case 0xe6:
      case 0xea:
      case 0xfa:
      case 0xfd:
      case 0xfe:
      case 0x104:
      case 0x107:
      case 0x108:
      case 0x10e:
      case 0x111:
      case 0x112:
      case 0x118:
      case 0x11b:
      case 0x11c:
      case 0x122:
      case 0x125:
      case 0x126:
      case 300:
      case 0x13a:
      case 0x176:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x3fc) {
      uVar2 = (u32)param_1;
      u32 dispatch_67 = (u32)(s32)param_1 - 0x3e8u;
      if (dispatch_67 <= 0x13u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_67,0x004bf1c8u);
      switch(uVar2) {
      case 1000:
      case 0x3e9:
      case 0x3ea:
      case 0x3eb:
      case 0x3ec:
      case 0x3ed:
      case 0x3ee:
      case 0x3ef:
      case 0x3f0:
      case 0x3f1:
      case 0x3f2:
      case 0x3f3:
      case 0x3f4:
      case 0x3f5:
      case 0x3f6:
      case 0x3f7:
      case 0x3f8:
      case 0x3f9:
      case 0x3fa:
      case 0x3fb:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x40c) {
      uVar2 = (u32)param_1;
      u32 dispatch_68 = (u32)(s32)param_1 - 0x3fcu;
      if (dispatch_68 <= 0xfu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_68,0x004bf218u);
      switch(uVar2) {
      case 0x3fc:
        goto switchD_004bb1ea_caseD_3fc;
      case 0x3fd:
      case 0x3fe:
      case 0x3ff:
      case 0x400:
      case 0x401:
      case 0x402:
      case 0x403:
      case 0x404:
      case 0x405:
      case 0x406:
      case 0x407:
      case 0x408:
      case 0x409:
      case 0x40a:
      case 0x40b:
        goto switchD_004ba9ec_caseD_82;
      }
    }
    else if (param_1 < 0x418) {
      uVar2 = (u32)param_1;
      u32 dispatch_69 = (u32)(s32)param_1 - 0x40cu;
      if (dispatch_69 <= 0xbu) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_69,0x004bf258u);
      switch(uVar2) {
      case 0x40c:
      case 0x40d:
      case 0x40e:
      case 0x40f:
      case 0x410:
      case 0x413:
      case 0x414:
      case 0x417:
        goto switchD_004ba9ec_caseD_78;
      case 0x411:
      case 0x412:
        goto switchD_004ba9ec_caseD_82;
      case 0x415:
      case 0x416:
        goto switchD_004ba99e_caseD_3;
      }
    }
    else {
      uVar2 = (u32)param_1;
      if (param_1 < 0x41f) {
        u32 dispatch_70 = (u32)(s32)param_1 - 0x418u;
        if (dispatch_70 <= 6u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_70,0x004bf288u);
        switch(uVar2) {
        case 0x418:
        case 0x419:
switchD_004ba99e_caseD_3:
          return ((index & 0xffffff00u) | 5u);
        case 0x41a:
        case 0x41b:
        case 0x41c:
          goto switchD_004ba9ec_caseD_78;
        case 0x41d:
        case 0x41e:
          goto switchD_004ba9ec_caseD_82;
        }
      }
      else {
        u32 dispatch_71 = (u32)(s32)param_1 - 0x41fu;
        if (dispatch_71 <= 0x64u) (void)cm_fn_004ba8f0_direct_dispatch(dispatch_71,0x004bf2a4u);
        switch(uVar2) {
        case 0x41f:
        case 0x420:
        case 0x421:
        case 0x422:
        case 0x423:
        case 0x424:
        case 0x425:
        case 0x426:
        case 0x427:
        case 0x428:
        case 0x429:
        case 0x42a:
        case 0x42b:
        case 0x42c:
        case 0x42d:
        case 0x42e:
        case 0x434:
        case 0x435:
        case 0x439:
        case 0x43a:
        case 0x43b:
        case 0x43c:
        case 0x43d:
        case 0x43e:
        case 0x43f:
        case 0x440:
        case 0x441:
        case 0x442:
        case 0x443:
        case 0x444:
        case 0x445:
        case 0x446:
        case 0x447:
        case 0x448:
        case 0x449:
        case 0x44a:
        case 1099:
        case 0x44c:
        case 0x44d:
        case 0x44e:
        case 0x44f:
        case 0x450:
        case 0x451:
        case 0x452:
        case 0x453:
        case 0x454:
        case 0x455:
        case 0x456:
        case 0x457:
        case 0x458:
        case 0x459:
        case 0x45a:
        case 0x45b:
        case 0x45c:
        case 0x45d:
        case 0x45e:
        case 0x45f:
        case 0x460:
        case 0x461:
        case 0x462:
        case 0x463:
        case 0x464:
        case 0x465:
        case 0x466:
        case 0x467:
        case 0x468:
        case 0x469:
        case 0x46a:
        case 0x46b:
        case 0x46c:
        case 0x46d:
        case 0x46e:
        case 0x46f:
        case 0x470:
        case 0x471:
        case 0x472:
        case 0x473:
        case 0x474:
        case 0x475:
        case 0x476:
        case 0x477:
        case 0x478:
        case 0x479:
        case 0x47a:
        case 0x47c:
        case 0x47d:
        case 0x47e:
        case 0x47f:
        case 0x480:
        case 0x481:
        case 0x482:
        case 0x483:
          goto switchD_004ba9ec_caseD_82;
        case 0x42f:
          goto switchD_004ba99e_caseD_3;
        case 0x430:
        case 0x431:
        case 0x432:
        case 0x433:
        case 0x436:
        case 0x437:
        case 0x438:
        case 0x47b:
          goto switchD_004ba9ec_caseD_78;
        }
      }
    }
  }
  uVar2 = cm_fn_00944c9f(0x00de1f64u,0x009a51b0u,index);
switchD_004baa77_caseD_411:
  return uVar2 & 0xffffff00;
}




