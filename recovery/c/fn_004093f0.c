/* WIP: five failed test iterations exhausted. Semantic cases 13/27/31 and others still differ on non-stack page 0x00ae2000 (cov=0.8632, inc=67); the remaining memory mismatch cause is unresolved. */
/* Draft for preferred-image VA 0x004093f0, image base 0x00400000, converted from Ghidra 12.1.4
   pseudocode by `cmre draft`. Unvalidated: check names, widths and signedness against
   `cmre show --asm`; acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
#define true 1
#define false 0
#define DAT_009cf658 CM_GLOBAL(volatile int, 0x009cf658)
#define DAT_009d00b8 CM_GLOBAL(volatile u32, 0x009d00b8)
#define DAT_009d00bc CM_GLOBAL(volatile u32, 0x009d00bc)
#define DAT_009d00c0 CM_GLOBAL(volatile u32, 0x009d00c0)
#define DAT_009d00f0 CM_GLOBAL(volatile u32, 0x009d00f0)
#define DAT_009d00fc CM_GLOBAL(volatile u32, 0x009d00fc)
#define DAT_009d01a4 CM_GLOBAL(volatile u32, 0x009d01a4)
#define DAT_009d01a8 CM_GLOBAL(volatile u32, 0x009d01a8)
#define DAT_009d01b0 CM_GLOBAL(volatile u32, 0x009d01b0)
#define DAT_009d01b8 CM_GLOBAL(volatile u32, 0x009d01b8)
#define DAT_009d01bc CM_GLOBAL(volatile u32, 0x009d01bc)
#define DAT_009d01c0 CM_GLOBAL(volatile u32, 0x009d01c0)
#define DAT_009d01d0 CM_GLOBAL(volatile u32, 0x009d01d0)
#define DAT_009d01d8 CM_GLOBAL(volatile u32, 0x009d01d8)
#define DAT_009d01dc CM_GLOBAL(volatile u32, 0x009d01dc)
#define DAT_009d01e0 CM_GLOBAL(volatile u32, 0x009d01e0)
#define DAT_009d01e8 CM_GLOBAL(volatile u32, 0x009d01e8)
#define DAT_009d01ec CM_GLOBAL(volatile u32, 0x009d01ec)
#define DAT_009d01f0 CM_GLOBAL(volatile u32, 0x009d01f0)
#define DAT_009d0204 CM_GLOBAL(volatile u32, 0x009d0204)
#define DAT_009d0208 CM_GLOBAL(volatile u32, 0x009d0208)
#define DAT_009d020c CM_GLOBAL(volatile u32, 0x009d020c)
#define DAT_009d0210 CM_GLOBAL(volatile u32, 0x009d0210)
#define DAT_009d021c CM_GLOBAL(volatile u32, 0x009d021c)
#define DAT_009d0228 CM_GLOBAL(volatile u32, 0x009d0228)
#define DAT_009d022c CM_GLOBAL(volatile u32, 0x009d022c)
#define DAT_00ae23b4 CM_GLOBAL(volatile int, 0x00ae23b4)
#define DAT_00ae24d0 CM_GLOBAL(volatile u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(volatile u32, 0x00b67a34)
#define s_E__dev_CM3_cm3_00_01_cm3_code_co_009874ec ((u32)0x009874ec)
#define s_Error_009870e0 ((char *)0x009870e0)
#define s_v_s__s__s__d_009870e8 ((char *)0x009870e8)
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern char * cm_fn_0090d130(...);  /* NEEDS-REVIEW: callee draft says (void); these are usually arguments MSVC pushed for the next call (right-to-left evaluation, deferred cleanup) */
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

void CM_THISCALL cm_fn_004093f0(int param_1)

{
  int *piVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  u8 *puVar5;
  u8 *puVar6;
  u32 uVar7;
  u8 scratch[512];
  u8 *local_200 = scratch;
  u8 *local_100 = scratch + 0x100;
  
  *(volatile short *)(param_1 + 0x34) = *(volatile short *)(param_1 + 0x3e);
  iVar2 = (int)(u32)cm_fn_00945501((int)*(volatile short *)(param_1 + 0x3e),0x13);
  iVar4 = 0;
  *(volatile int *)(param_1 + 0x18) = iVar2;
  if (iVar2 == 0) {
    cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_co_009874ec,0,0,local_100,local_200);
    puVar6 = local_100;
    puVar5 = local_200;
    uVar7 = 0x638;
LAB_00409ba5:
    uVar7 = (u32)cm_fn_0090d130(puVar5,puVar6,uVar7);
    cm_fn_00944c9f((u32)&DAT_00ae24d0,(u32)s_v_s__s__s__d_009870e8,uVar7);
    cm_fn_005e8290(s_Error_009870e0,(char *)&DAT_00ae24d0,0);
    DAT_00b67a34 = 0;
    return;
  }
  cVar3 = '\0';
  if (0 < *(volatile short *)(param_1 + 0x34)) {
    do {
      iVar2 = (int)cVar3;
      cVar3 = cVar3 + '\x01';
      *(volatile u32 *)(iVar2 * 0x13 + *(volatile int *)(param_1 + 0x18)) = 0xffffffff;
      *(volatile u16 *)(iVar2 * 0x13 + 8 + *(volatile int *)(param_1 + 0x18)) = *(volatile u16 *)(param_1 + 0x40)
      ;
    } while ((short)cVar3 < *(volatile short *)(param_1 + 0x34));
  }
  **(u32 **)(param_1 + 0x18) = DAT_009d01a4;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 10) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xc) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xd) = 0x47;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xf) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 8) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x13) = DAT_009d01ec;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1d) = 0x3f;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1f) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x20) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x22) = 0x1a;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1b) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x26) = DAT_009d01e8;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x30) = 0x3f;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x32) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x33) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x35) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x2e) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x39) = DAT_009d021c;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x43) = 0x3e;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x45) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x46) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x48) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x41) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x4c) = DAT_009d0208;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x56) = 0x37;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x58) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x59) = 0x2c;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x5b) = 0x1a;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x54) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x5f) = DAT_009d01d8;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x69) = 0x2f;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x6b) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x6c) = 0x37;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x6e) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x67) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x72) = DAT_009d01d0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x7c) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x7e) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x7f) = 0x33;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x81) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x7a) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x85) = DAT_009d01a8;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x8f) = 0x31;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x91) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x92) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x94) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x8d) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x98) = DAT_009d00fc;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xa2) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xa4) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xa5) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xa7) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xa0) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0xab) = DAT_009d01b0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xb5) = 0x33;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xb7) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xb8) = 0x2d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xba) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xb3) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0xbe) = DAT_009d01b8;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 200) = 0x28;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xca) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xcb) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xcd) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xc6) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0xd1) = DAT_009d00bc;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xdb) = 0x26;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xdd) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xde) = 0x34;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xe0) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xd9) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0xe4) = DAT_009d01dc;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xee) = 0x2e;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xf0) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xf1) = 0x2b;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0xf3) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xec) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0xf7) = DAT_009d01e0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x101) = 0x2b;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x103) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x104) = 0x2c;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x106) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0xff) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x10a) = DAT_009d0204;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x114) = 0x34;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x116) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x117) = 0x1d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x119) = 0x18;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x112) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x11d) = DAT_009d00c0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x127) = 0x29;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x129) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x12a) = 0x2b;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 300) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x125) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x130) = DAT_009d00f0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x13a) = 0x2d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x13c) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x13d) = 0x21;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x13f) = 0x18;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x138) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x143) = DAT_009d00b8;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x14d) = 0x2d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x14f) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x150) = 0x23;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x152) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x14b) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x156) = DAT_009d022c;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x160) = 0x2d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x162) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x163) = 0x1a;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x165) = 0x18;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x15e) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x169) = DAT_009d0228;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x173) = 0x32;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x175) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x176) = 0x14;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x178) = 0x18;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x171) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x17c) = DAT_009d01f0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x186) = 0x26;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x188) = 0x22;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x189) = 0x1f;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x18b) = 0x18;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x184) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 399) = DAT_009d01c0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x199) = 0x29;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x19b) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x19c) = 0x20;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x19e) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x197) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x1a2) = DAT_009d01bc;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1ac) = 0x1d;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1ae) = 0x1e;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1af) = 0x2b;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1b1) = 0x20;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1aa) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x1b5) = DAT_009d0210;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1bf) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1c1) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1c2) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1c4) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1bd) = *(volatile u16 *)(param_1 + 0x40);
  *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + 0x1c8) = DAT_009d020c;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1d2) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1d4) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1d5) = 0;
  *(volatile u8 *)(*(volatile int *)(param_1 + 0x18) + 0x1d7) = 0;
  *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 0x1d0) = *(volatile u16 *)(param_1 + 0x40);
  cVar3 = '\0';
  if (0 < *(volatile short *)(param_1 + 0x34)) {
    while ((piVar1 = *(int **)(DAT_00ae23b4 +
                               *(volatile int *)(*(volatile int *)(param_1 + 0x18) + cVar3 * 0x13) * 0x245 + 0x57),
           piVar1 != (int *)0x0 && (*piVar1 == DAT_009cf658))) {
      cVar3 = cVar3 + '\x01';
      if (*(volatile short *)(param_1 + 0x34) <= (short)cVar3) {
        return;
      }
    }
    cm_fn_009452ca(*(volatile int *)(param_1 + 0x18));
    iVar2 = (int)(u32)cm_fn_00945501((int)*(volatile short *)(param_1 + 0x34),0x13);
    *(volatile int *)(param_1 + 0x18) = iVar2;
    if (iVar2 == 0) {
      cm_fn_00944cff(s_E__dev_CM3_cm3_00_01_cm3_code_co_009874ec,0,0,local_200,local_100);
      puVar6 = local_100;
      puVar5 = local_200;
      uVar7 = 0x6d1;
      goto LAB_00409ba5;
    }
    iVar2 = 0;
    if (0 < *(volatile short *)(param_1 + 0x34)) {
      do {
        iVar2 = iVar2 + 1;
        *(volatile u32 *)(*(volatile int *)(param_1 + 0x18) + iVar4) = 0xffffffff;
        *(volatile u16 *)(*(volatile int *)(param_1 + 0x18) + 8 + iVar4) = *(volatile u16 *)(param_1 + 0x40);
        iVar4 = iVar4 + 0x13;
      } while (iVar2 < *(volatile short *)(param_1 + 0x34));
    }
  }
  return;
}

