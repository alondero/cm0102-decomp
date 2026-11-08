/* Reconstructed preferred-image VA 0x007a9650, image base 0x00400000.
   Pinned CM0102 3.9.68 SHA-256:
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   RET 12 consumes record, selector and destination; incoming ECX is unused.
   The indexed direct caller overwrites EAX with 1 immediately after return.
   Helpers remain original-code boundaries. This does not prove composition. */
#include "cmre.h"
#define DAT_00ae23a8 CM_GLOBAL(volatile u32, 0x00ae23a8)
#define DAT_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
extern u32 CM_THISCALL cm_fn_00785830(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae660(void *,u32);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *,u8 *,u32,u32);
extern u32 CM_CDECL cm_fn_0053ac40(s8,void *);
extern u32 CM_CDECL cm_fn_0053ad00(u8,s32,s32,void *);
extern u32 CM_CDECL cm_fn_0053e140(u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053e990(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32,char *,...);
extern unsigned long long CM_CDECL __attribute__((regparm(2))) cm_fn_0090cfc0(u32,u32,u32);

void CM_STDCALL cm_fn_007a9650(u32 record, u32 selector, u32 destination)

{
  s8 cVar1;
  u32 iVar2;
  u32 uVar3;
  u32 uVar4;
  u32 iVar5;
  u32 iVar6;
  u32 iVar7;
  u32 country_base, club_base;
  char *pcVar8;
  /* L is ESP immediately after SUB ESP,0x1b0. Saved registers occupy
     L-16. All original local addresses share this uninitialized allocation. */
  union { u32 align[108]; u8 bytes[0x1b0]; } scratch;
#define local_1b0 (*(s8 *)(scratch.bytes+0))
#define local_1af (*(s8 *)(scratch.bytes+1))
#define local_1ae (*(s8 *)(scratch.bytes+2))
#define local_1ad (*(s8 *)(scratch.bytes+3))
#define local_1ac (*(u32 *)(scratch.bytes+4))
#define local_1a8 (*(u8 *)(scratch.bytes+8))
#define local_1a4 (*(u32 *)(scratch.bytes+12))
#define local_1a0 (*(u32 *)(scratch.bytes+16))
#define local_19c (*(u32 *)(scratch.bytes+20))
#define local_198 (*(u32 *)(scratch.bytes+24))
#define local_194 (*(u32 *)(scratch.bytes+28))
#define local_190 (scratch.bytes+32)

  if (selector == 0) {
    iVar2 = (u32)cm_fn_00785830((void *)record, 0);
    uVar3 = (u32)cm_fn_00785830((void *)record, 1);
    uVar4 = (u32)cm_fn_00785830((void *)record, 2);
    local_1ac = (u32)cm_fn_00785830((void *)record, 3);
    local_1ad = (u32)cm_fn_00785830((void *)record, 4);
    local_1ae = (u32)cm_fn_00785830((void *)record, 5);
    cm_fn_0053e140(uVar3,uVar4,local_1ac,(u32)(local_190),400,5,iVar2);
    if (local_1ad == '\b') {
      pcVar8 = ((char *)0x00a6e1b8);
    }
    else if (local_1ae == '\0') {
      pcVar8 = ((char *)0x00a6e1f0);
    }
    else {
      pcVar8 = ((char *)0x00a6e224);
    }
    cm_fn_0066f4e0(0x00de1f64,pcVar8,local_190);
    cm_fn_004ae660((void *)destination, 0x00de1f64);
    iVar5 = cm_fn_0053e990(iVar2 * 0x6eu + DAT_00ae23bc,uVar3,uVar4,local_1ac,0);
    if (iVar5 == 0) {
      return;
    }
    cm_fn_004ae8a0((void *)destination, local_190, 0x7d7, iVar2 * 0x6eu + DAT_00ae23bc);
    return;
  }
  local_1ac = (u32)cm_fn_00785830((void *)record, 0);
  local_1a0 = (u32)cm_fn_00785830((void *)record, 1);
  local_1a4 = (u32)cm_fn_00785830((void *)record, 2);
  local_19c = (u32)cm_fn_00785830((void *)record, 3);
  local_1ad = (u32)cm_fn_00785830((void *)record, 4);
  local_1b0 = (u32)cm_fn_00785830((void *)record, 5);
  iVar2 = (u32)cm_fn_00785830((void *)record, 6);
  iVar5 = (u32)cm_fn_00785830((void *)record, 7);
  cVar1 = (u32)cm_fn_00785830((void *)record, 8);
  local_1a8 = (u8)cVar1;
  local_1ae = (u32)cm_fn_00785830((void *)record, 9);
  local_194 = (u32)cm_fn_00785830((void *)record, 0xb);
  local_1af = (u32)cm_fn_00785830((void *)record, 0xc);
  local_198 = (u32)cm_fn_00785830((void *)record, 0xd);
  iVar6 = (u32)cm_fn_00785830((void *)record, 0xe);
  cm_fn_0053e140(local_1a0,local_1a4,local_19c,(u32)(local_190),400,1,local_1ac);
  if (local_1ad == '\b') {
    if (local_1ae == '\0') {
      if (*(volatile u32 *)(DAT_00ae23bc + 0x39 + local_1ac * 0x6eu) != 0) {
        iVar5 = DAT_00ae23b4 + iVar2 * 0x245;
        iVar2 = (int)*(volatile s8 *)(iVar5 + 0x37);
        cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6c7d4),iVar2,iVar2,iVar5 + 4
                     ,local_190);
        goto LAB_007aa2c4;
      }
    }
    else {
      if (iVar5 == 0xffffffffu) {
        country_base = DAT_00ae23a8;
        iVar2 = (s32)*(volatile s8 *)(country_base - 0xebu);
        cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6c88c),
                     iVar2,iVar2,country_base - 0x11eu,local_190);
        goto LAB_007aa2c4;
      }
      if (iVar2 != 1) {
        cVar1 = *(volatile s8 *)(DAT_00ae23bc + 0x3d + local_1ac * 0x6eu);
        if (((cVar1 == '\0') || (cVar1 == '\x10')) || (cVar1 == '\v')) {
          iVar2 = DAT_00ae23b4 + iVar2 * 0x245;
          country_base = DAT_00ae23a8;
          iVar7 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
          iVar6 = (int)*(volatile s8 *)(iVar2 + 0x37);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6c958),iVar7,iVar7,iVar6,
                       iVar6,country_base + iVar5 * 0x122u + 4,iVar2 + 4,local_190);
        }
        else {
          uVar3 = (u32)(cm_fn_0053ad00(local_1a8,0,0,(u8 *)(&local_1af)));
          iVar6 = DAT_00ae23b4 + iVar2 * 0x245;
          country_base = DAT_00ae23a8;
          iVar7 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
          iVar2 = iVar6 + 4;
          iVar6 = (int)*(volatile s8 *)(iVar6 + 0x37);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6ca34),iVar6,iVar6,iVar7,
                       iVar7,(int)local_1af,(int)local_1af,iVar6,iVar6,iVar2,
                       country_base + iVar5 * 0x122u + 4,local_190,uVar3,iVar2);
        }
        goto LAB_007aa2c4;
      }
    }
    pcVar8 = ((char *)0x00a6c744);
  }
  else {
    cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6e164),local_190);
    if (local_1b0 == '\0') {
      if (local_1ad == '\x10') {
        if (local_1af < '\x1b') {
          pcVar8 = ((char *)0x00a6d7d4);
        }
        else {
          pcVar8 = ((char *)0x00a6d734);
        }
      }
      else if ((local_1ad == ' ') &&
              (((((cVar1 == '\n' || (cVar1 == '\b')) ||
                 ((cVar1 == '\x06' || ((cVar1 == '\t' || (cVar1 == '\x01')))))) || (cVar1 == '\x02')
                ) || ((cVar1 == '\x03' || (cVar1 == '\x04')))))) {
        uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1af);
        if (iVar2 != 0xffffffffu) {
          club_base = DAT_00ae23b4;
          iVar5 = (s32)*(volatile s8 *)(club_base + iVar2 * 0x245u + 0x37);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d65c),iVar5,iVar5,
                       (int)local_1af,(int)local_1af,local_190,
                       club_base + iVar2 * 0x245u + 4,uVar3);
          goto LAB_007aa2c4;
        }
        pcVar8 = ((char *)0x00a6d5f0);
      }
      else if ((local_198 == 0) || (((cVar1 == '\0' || (cVar1 == '\x10')) || (cVar1 == '\v')))) {
        if (local_1ae == '\0') {
          pcVar8 = ((char *)0x00a6cb84);
        }
        else {
          if (iVar5 != 0xffffffffu) {
LAB_007a9e62:
            iVar2 = DAT_00ae23a8 + iVar5 * 0x122;
            iVar5 = (int)*(volatile s8 *)(iVar2 + 0x37);
            cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d550),iVar5,iVar5,
                         iVar2 + 4,local_190);
            goto LAB_007aa2c4;
          }
          pcVar8 = ((char *)0x00a6cfb0);
        }
      }
      else {
        if (local_1ae == '\0') {
          if (iVar2 == 0xffffffffu) {
            uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1af);
            cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6cbf8),(int)local_1af,
                         (int)local_1af,local_190,uVar3);
          }
          else {
            uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1af);
            club_base = DAT_00ae23b4;
            iVar5 = (s32)*(volatile s8 *)(club_base + iVar2 * 0x245u + 0x37);
            cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6ccb0),iVar5,iVar5,
                         (int)local_1af,(int)local_1af,local_190,
                         club_base + iVar2 * 0x245u + 4,uVar3);
          }
          goto LAB_007aa2c4;
        }
        if (iVar5 == 0xffffffffu) {
          cVar1 = *(volatile s8 *)(DAT_00ae23bc + 0x3d + local_1ac * 0x6eu);
          if (cVar1 == '\f') {
            pcVar8 = ((char *)0x00a6cd78);
          }
          else if (cVar1 == '\r') {
            pcVar8 = ((char *)0x00a6ce34);
          }
          else if (cVar1 == '\x0f') {
            pcVar8 = ((char *)0x00a6cef8);
          }
          else {
            pcVar8 = ((char *)0x00a6cfb0);
          }
        }
        else {
          if (iVar2 != 0xffffffffu) {
            cVar1 = *(volatile s8 *)(DAT_00ae23bc + 0x3d + local_1ac * 0x6eu);
            if (cVar1 == '\f') {
              iVar2 = DAT_00ae23b4 + iVar2 * 0x245;
              country_base = DAT_00ae23a8;
              iVar7 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
              iVar6 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d288),iVar7,iVar7,
                           iVar6,iVar6,country_base + iVar5 * 0x122u + 4,local_190,iVar2 + 4);
              goto LAB_007aa2c4;
            }
            if (cVar1 == '\r') {
              iVar2 = DAT_00ae23b4 + iVar2 * 0x245;
              country_base = DAT_00ae23a8;
              iVar7 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
              iVar6 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d374),iVar7,iVar7,
                           iVar6,iVar6,country_base + iVar5 * 0x122u + 4,local_190,iVar2 + 4);
              goto LAB_007aa2c4;
            }
            if (cVar1 == '\x0f') {
              iVar2 = DAT_00ae23b4 + iVar2 * 0x245;
              country_base = DAT_00ae23a8;
              iVar7 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
              iVar6 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d468),iVar7,iVar7,
                           iVar6,iVar6,country_base + iVar5 * 0x122u + 4,local_190,iVar2 + 4);
              goto LAB_007aa2c4;
            }
            goto LAB_007a9e62;
          }
          cVar1 = *(volatile s8 *)(DAT_00ae23bc + 0x3d + local_1ac * 0x6eu);
          if (cVar1 == '\f') {
            pcVar8 = ((char *)0x00a6d038);
          }
          else if (cVar1 == '\r') {
            pcVar8 = ((char *)0x00a6d038);
          }
          else if (cVar1 == '\x0f') {
            pcVar8 = ((char *)0x00a6d0fc);
          }
          else {
            pcVar8 = ((char *)0x00a6d1b4);
          }
        }
      }
    }
    else {
      if ((local_198 != 0) && (iVar6 == 0)) {
        if (iVar2 == 0xffffffffu) {
          if (iVar5 == 0xffffffffu) {
            if (cVar1 == '\0') {
              uVar3 = cm_fn_0053ad00(0,0,0,&local_1b0);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6e000),(int)local_1b0
                           ,(int)local_1b0,uVar3,local_190);
            }
            else {
              uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1b0);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6e04c),(int)local_1b0
                           ,(int)local_1b0,uVar3,local_190);
            }
          }
          else {
            uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1b0);
            iVar5 = DAT_00ae23a8 + iVar5 * 0x122;
            iVar6 = (int)local_1b0;
            iVar2 = iVar5 + 4;
            iVar5 = (int)*(volatile s8 *)(iVar5 + 0x37);
            cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6e09c),iVar5,iVar5,iVar6
                         ,iVar6,iVar5,iVar5,iVar6,iVar6,iVar2,uVar3,local_190,iVar2,uVar3);
          }
        }
        else {
          uVar3 = cm_fn_0053ad00(local_1a8,0,0,&local_1b0);
          iVar6 = DAT_00ae23b4 + iVar2 * 0x245;
          iVar5 = (int)local_1b0;
          iVar2 = iVar6 + 4;
          iVar6 = (int)*(volatile s8 *)(iVar6 + 0x37);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6e09c),iVar6,iVar6,iVar5,
                       iVar5,iVar6,iVar6,iVar5,iVar5,iVar2,uVar3,local_190,iVar2,uVar3);
        }
        goto LAB_007aa2c4;
      }
      if (local_194 == 0) {
        if (local_1ae == '\0') {
          pcVar8 = ((char *)0x00a6e164);
        }
        else {
          if (iVar5 != 0xffffffffu) {
            if (iVar2 == 0xffffffffu) {
              iVar2 = DAT_00ae23a8 + iVar5 * 0x122;
              iVar5 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d870),iVar5,iVar5,
                           iVar2 + 4,local_190);
            }
            else {
              iVar5 = DAT_00ae23a8 + iVar5 * 0x122u;
              club_base = DAT_00ae23b4;
              iVar7 = (s32)*(volatile s8 *)(club_base + iVar2 * 0x245u + 0x37);
              iVar6 = (s32)*(volatile s8 *)(iVar5 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6d900),iVar7,iVar7,
                           iVar6,iVar6,club_base + iVar2 * 0x245u + 4,iVar5 + 4,local_190);
            }
            goto LAB_007aa2c4;
          }
          pcVar8 = ((char *)0x00a6e164);
        }
      }
      else if (local_1ae == '\0') {
        cVar1 = *(volatile s8 *)(DAT_00ae23bc + 0x3d + local_1ac * 0x6eu);
        if ((cVar1 != '\0') && (cVar1 != '\x10')) {
          uVar3 = cm_fn_0053ac40(local_1a8,&local_1b0);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6da38),(int)local_1b0,
                       (int)local_1b0,local_190,uVar3);
          goto LAB_007aa2c4;
        }
        pcVar8 = ((char *)0x00a6d9ac);
      }
      else {
        if (iVar5 != 0xffffffffu) {
          if (iVar2 == 0xffffffffu) {
            if ((((cVar1 == '\0') || (cVar1 == '\x10')) ||
                ((cVar1 == '\v' || ((cVar1 == '\f' || (cVar1 == '\x0f')))))) || (cVar1 == '\r')) {
              iVar2 = DAT_00ae23a8 + iVar5 * 0x122;
              iVar5 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6dc18),iVar5,iVar5,
                           iVar2 + 4,local_190);
            }
            else {
              uVar3 = cm_fn_0053ac40(local_1a8,&local_1b0);
              country_base = DAT_00ae23a8;
              iVar2 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6dcd8),iVar2,iVar2,
                           (int)local_1b0,(int)local_1b0,country_base + iVar5 * 0x122u + 4,local_190,
                           uVar3);
            }
          }
          else if (((cVar1 == '\0') || (cVar1 == '\x10')) || (cVar1 == '\v')) {
            /* CALL VA007A9A7C: bound=2 is nonzero. Entry EAX/EDX are
               not read on that original helper path. */
            iVar2 = cm_fn_0090cfc0(0,0,2);
            if (iVar2 == 0) {
              iVar2 = DAT_00ae23a8 + iVar5 * 0x122;
              iVar5 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6dd98),iVar5,iVar5,
                           iVar2 + 4,local_190);
            }
            else {
              iVar2 = DAT_00ae23a8 + iVar5 * 0x122;
              iVar5 = (int)*(volatile s8 *)(iVar2 + 0x37);
              cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6de5c),iVar5,iVar5,
                           iVar2 + 4,local_190);
            }
          }
          else {
            uVar3 = cm_fn_0053ac40(local_1a8,&local_1b0);
            iVar2 = DAT_00ae23b4 + iVar2 * 0x245;
            country_base = DAT_00ae23a8;
            iVar6 = (s32)*(volatile s8 *)(country_base + 0x37 + iVar5 * 0x122u);
            iVar7 = (int)*(volatile s8 *)(iVar2 + 0x37);
            cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6df18),iVar6,iVar6,
                         (int)local_1b0,(int)local_1b0,iVar7,iVar7,country_base + iVar5 * 0x122u + 4,
                         local_190,uVar3,iVar2 + 4);
          }
          goto LAB_007aa2c4;
        }
        if (((cVar1 != '\0') && (cVar1 != '\x10')) && (cVar1 != '\v')) {
          uVar3 = cm_fn_0053ac40(local_1a8,&local_1b0);
          cm_fn_0066f4e0(0x00de1f64,((char *)0x00a6db74),(int)local_1b0,
                       (int)local_1b0,local_190,uVar3);
          goto LAB_007aa2c4;
        }
        pcVar8 = ((char *)0x00a6dad0);
      }
    }
  }
  cm_fn_0066f4e0(0x00de1f64,pcVar8,local_190);
LAB_007aa2c4:
  cm_fn_004ae660((void *)destination, 0x00de1f64);
  iVar5 = local_1ac * 0x6eu;
  iVar2 = cm_fn_0053e990(iVar5 + DAT_00ae23bc,local_1a0,local_1a4,local_19c,0);
  if (iVar2 != 0) {
    cm_fn_004ae8a0((void *)destination, local_190,0x7d7,iVar5 + DAT_00ae23bc);
  }
  return;
}
