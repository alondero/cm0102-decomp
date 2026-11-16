/* Recovered from original instructions, VA 0x006a69c0 (preferred image base
   0x00400000, CM0102 3.9.68 primary executable). thiscall: ECX (this) is read
   at 0x006a69d0 before being written, the callee pops 0x3c bytes (15 stack
   arguments).

   Frame: `sub esp,0xce8` then push ebx/ebp/esi/edi (0x10 bytes), so with no
   extra push live `[esp+0xd00]` is the second stack argument. One scratch
   allocation holds the original local bytes at their listing offsets:

     L+0x000  dword: saved `this`                  (0x006a69d5)
     L+0x004  char[100]: name buffer               (0x006a6a12 / 0x006a6a2a)
     L+0x068  char[100]: not referenced by this body
     L+0x0cc  char[1500]: message buffer           (0x006a7c7f / 0x006a796a)
     L+0x6a8  char[100]: helper buffer             (0x006a69f8)
     L+0x70c  char[1500]: comment buffer           (0x006a7d67 / 0x006a7f9a)

   The scratch bytes are left uninitialized except where the listing writes
   them (0x006a7f9a clears the first comment byte; 0x006a7d6e writes the low
   byte of the ninth stack argument). */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern unsigned long long CM_THISCALL cm_fn_005c6710(void *self, u32 arg);
extern unsigned long long CM_CDECL cm_fn_006181d0(u32 arg1,u32 arg2, u32 arg3,u32 arg4);
/* Formatter: destination first, then format; call sites pass 2..10 arguments. */
extern char * cm_fn_0066f4e0(int param_1,char *param_2,...);
/* 0x006ab2d0 receives ECX = the record saved at L+0 and pops its four stack
   arguments (no `add esp` follows the call at 0x006a7d8b). */
extern void CM_THISCALL cm_fn_006ab2d0(void *self,int *param_1,char param_2,u8 *param_3,int param_4);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);

u32 CM_THISCALL
cm_fn_006a69c0(void *self, u32 s0, u32 s1, u32 s2, u32 s3, u32 s4,
               u32 s5, u32 s6, u32 s7, u32 s8, u32 s9,
               u32 s10, u32 s11, u32 s12, u32 s13, u32 s14)
{
  u8 scratch[0xce8];
  u32 edi_v;
  int edi_s;
  int G;
  int E;
  u32 idx;
  u32 tgt;
  u32 recv;
  const char *cfmt;
  u32 u;

  (void)s5;
  (void)s6;

  /* 0x006a69d5 */
  *(u32 *)(scratch + 0) = (u32)self;

  /* 0x006a69d3: third stack argument null -> 0x006a805e before any name
     buffer write. Nothing is synthesized there: the reload of [esp+0xd00] with
     only the saved registers pushed selects s1, whose +0x37 byte is read, and
     the still-unwritten name buffer is forwarded as-is. */
  if (s2 == 0) goto L_6a805e;

  /* 0x006a69e7 */
  if (cm_fn_0053b900((u32 *)s0) != 0) {
    /* 0x006a69f3: helper fills L+0x6a8, then the name is formatted into L+4. */
    cm_fn_006181d0(*(u32 *)(s0 + 0x53), (u32)(scratch + 0x6a8), 100, 9);
    cm_fn_0066f4e0((int)(scratch + 4), (char *)0x00a0fc8c, 100, 100,
                   (int)(scratch + 0x6a8));
  }
  else {
    /* 0x006a6a23 */
    G = (int)*(signed char *)(s0 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 4), (char *)0x00a0fc54, G, G, s0 + 4);
  }

  /* 0x006a6a3c: loaded before `add esp,0x14` with the five format arguments
     still pushed, so [esp+0xd1c] is L+0xcf8: the fourth stack argument. */
  edi_v = s3;
  edi_s = (int)edi_v;

  /* ---- dispatch tree on the fourth stack argument (s3) ---- */
  if (edi_s > 0xa08) goto L_6a71bd;
  if (edi_s == 0xa08) goto L_6a718b;
  if (edi_s > 0x581) goto L_6a6d12;
  if (edi_s == 0x581) goto L_6a6cb0;
  if (edi_s > 0x481) goto L_6a6a96;
  if (edi_s == 0x481) goto L_6a6cb0;

  /* 0x006a6a78: family A, values 0x401..0x460, byte map 0x006a80c4,
     target table 0x006a8098. */
  if ((u32)(edi_v - 0x401) > 0x5f) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a80c4 + (u32)(edi_v - 0x401));
  tgt = CM_GLOBAL(u32, 0x006a8098 + 4 * idx);
  switch (tgt) {
  case 0x006a6c4e: goto L_6a6c4e;
  case 0x006a6b8a: goto L_6a6b8a;
  case 0x006a701c: goto L_6a701c;
  case 0x006a6dc8: goto L_6a6dc8;
  case 0x006a6bec: goto L_6a6bec;
  case 0x006a6b20: goto L_6a6b20;
  case 0x006a6ab6: goto L_6a6ab6;
  case 0x006a6d56: goto L_6a6d56;
  case 0x006a6ed2: goto L_6a6ed2;
  case 0x006a6fba: goto L_6a6fba;
  default: goto L_6a708a;
  }

L_6a6a96:
  /* 0x006a6a96: family B, 0x482..0x544, map 0x006a814c, table 0x006a8124. */
  if ((u32)(edi_v - 0x482) > 0xc2) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a814c + (u32)(edi_v - 0x482));
  tgt = CM_GLOBAL(u32, 0x006a8124 + 4 * idx);
  switch (tgt) {
  case 0x006a6e94: goto L_6a6e94;
  case 0x006a6e2a: goto L_6a6e2a;
  case 0x006a6f58: goto L_6a6f58;
  case 0x006a701c: goto L_6a701c;
  case 0x006a6c4e: goto L_6a6c4e;
  case 0x006a6b8a: goto L_6a6b8a;
  case 0x006a6bec: goto L_6a6bec;
  case 0x006a6b20: goto L_6a6b20;
  case 0x006a6ab6: goto L_6a6ab6;
  default: goto L_6a708a;
  }

L_6a6d12:
  if (edi_s > 0x810) goto L_6a70df;
  if (edi_s == 0x810) goto L_6a7391;
  if (edi_s > 0x650) goto L_6a6f34;
  if (edi_s == 0x650) goto L_6a6ed2;

  /* 0x006a6d36: family C, 0x582..0x648, map 0x006a8228, table 0x006a8210. */
  if ((u32)(edi_v - 0x582) > 0xc6) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8228 + (u32)(edi_v - 0x582));
  tgt = CM_GLOBAL(u32, 0x006a8210 + 4 * idx);
  switch (tgt) {
  case 0x006a6e94: goto L_6a6e94;
  case 0x006a6e2a: goto L_6a6e2a;
  case 0x006a701c: goto L_6a701c;
  case 0x006a6dc8: goto L_6a6dc8;
  case 0x006a6d56: goto L_6a6d56;
  default: goto L_6a708a;
  }

L_6a6f34:
  if (edi_s > 0x6a0) goto L_6a707e;
  if (edi_s == 0x6a0) goto L_6a701c;
  if (edi_s == 0x660) goto L_6a6fba;
  if (edi_s == 0x688) goto L_6a6f58;
  goto L_6a708a;

L_6a707e:
  if (edi_s == 0x808) goto L_6a718b;
  goto L_6a708a;

L_6a70df:
  if (edi_s > 0x888) goto L_6a7107;
  if (edi_s == 0x888) goto L_6a73c3;

  /* 0x006a70ed: family D, 0x820..0x860, map 0x006a8304, table 0x006a82f0. */
  if ((u32)(edi_v - 0x820) > 0x40) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8304 + (u32)(edi_v - 0x820));
  tgt = CM_GLOBAL(u32, 0x006a82f0 + 4 * idx);
  switch (tgt) {
  case 0x006a72f5: goto L_6a72f5;
  case 0x006a7211: goto L_6a7211;
  case 0x006a7327: goto L_6a7327;
  case 0x006a728b: goto L_6a728b;
  default: goto L_6a708a;
  }

L_6a7107:
  /* 0x006a7107: family E, 0x890..0x988, map 0x006a835c, table 0x006a8348. */
  if ((u32)(edi_v - 0x890) > 0xf8) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a835c + (u32)(edi_v - 0x890));
  tgt = CM_GLOBAL(u32, 0x006a8348 + 4 * idx);
  switch (tgt) {
  case 0x006a74bb: goto L_6a74bb;
  case 0x006a7451: goto L_6a7451;
  case 0x006a7127: goto L_6a7127;
  case 0x006a7159: goto L_6a7159;
  default: goto L_6a708a;
  }

L_6a71bd:
  if (edi_s > 0x1088) goto L_6a75be;
  if (edi_s == 0x1088) goto L_6a7a18;
  if (edi_s > 0x1008) goto L_6a75a0;
  if (edi_s == 0x1008) goto L_6a7536;
  if (edi_s > 0xa88) goto L_6a742d;
  if (edi_s == 0xa88) goto L_6a73c3;

  /* 0x006a71f3: family F, 0xa10..0xa60, map 0x006a8470, table 0x006a8458. */
  if ((u32)(edi_v - 0xa10) > 0x50) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8470 + (u32)(edi_v - 0xa10));
  tgt = CM_GLOBAL(u32, 0x006a8458 + 4 * idx);
  switch (tgt) {
  case 0x006a7391: goto L_6a7391;
  case 0x006a72f5: goto L_6a72f5;
  case 0x006a7211: goto L_6a7211;
  case 0x006a7327: goto L_6a7327;
  case 0x006a728b: goto L_6a728b;
  default: goto L_6a708a;
  }

L_6a742d:
  if (edi_s > 0x1001) goto L_6a7525;
  if (edi_s == 0x1001) goto L_6a7905;
  if (edi_s == 0xa90) goto L_6a74bb;
  if (edi_s == 0xaa0) goto L_6a7451;
  goto L_6a708a;

L_6a7525:
  if (edi_s == 0x1002) goto L_6a7802;
  goto L_6a708a;

L_6a75a0:
  /* 0x006a75a0: family G, 0x1020..0x1084, map 0x006a84ec, table 0x006a84c4. */
  if ((u32)(edi_v - 0x1020) > 0x64) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a84ec + (u32)(edi_v - 0x1020));
  tgt = CM_GLOBAL(u32, 0x006a84c4 + 4 * idx);
  switch (tgt) {
  case 0x006a76c4: goto L_6a76c4;
  case 0x006a7834: goto L_6a7834;
  case 0x006a7798: goto L_6a7798;
  case 0x006a772e: goto L_6a772e;
  case 0x006a75f0: goto L_6a75f0;
  case 0x006a765a: goto L_6a765a;
  case 0x006a789e: goto L_6a789e;
  case 0x006a797f: goto L_6a797f;
  case 0x006a7b23: goto L_6a7b23;
  default: goto L_6a708a;
  }

L_6a75be:
  if (edi_s > 0x1182) goto L_6a79e6;
  if (edi_s == 0x1182) goto L_6a797f;

  /* 0x006a75d0: family H, 0x10a0..0x1181, map 0x006a8580, table 0x006a8554. */
  if ((u32)(edi_v - 0x10a0) > 0xe1) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8580 + (u32)(edi_v - 0x10a0));
  tgt = CM_GLOBAL(u32, 0x006a8554 + 4 * idx);
  switch (tgt) {
  case 0x006a7abc: goto L_6a7abc;
  case 0x006a7905: goto L_6a7905;
  case 0x006a7802: goto L_6a7802;
  case 0x006a76c4: goto L_6a76c4;
  case 0x006a7834: goto L_6a7834;
  case 0x006a7798: goto L_6a7798;
  case 0x006a772e: goto L_6a772e;
  case 0x006a75f0: goto L_6a75f0;
  case 0x006a765a: goto L_6a765a;
  case 0x006a789e: goto L_6a789e;
  default: goto L_6a708a;
  }

L_6a79e6:
  if (edi_s > 0x1220) goto L_6a7bc7;
  if (edi_s == 0x1220) goto L_6a7b9d;

  /* 0x006a79f8: family I, 0x1184..0x1208, map 0x006a8678, table 0x006a8664. */
  if ((u32)(edi_v - 0x1184) > 0x84) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8678 + (u32)(edi_v - 0x1184));
  tgt = CM_GLOBAL(u32, 0x006a8664 + 4 * idx);
  switch (tgt) {
  case 0x006a7b23: goto L_6a7b23;
  case 0x006a7a18: goto L_6a7a18;
  case 0x006a7abc: goto L_6a7abc;
  case 0x006a7a92: goto L_6a7a92;
  default: goto L_6a708a;
  }

L_6a7bc7:
  /* 0x006a7bc7: family J, 0x1248..0x12a0, map 0x006a8714, table 0x006a8700. */
  if ((u32)(edi_v - 0x1248) > 0x58) goto L_6a708a;
  idx = (u32)CM_GLOBAL(u8, 0x006a8714 + (u32)(edi_v - 0x1248));
  tgt = CM_GLOBAL(u32, 0x006a8700 + 4 * idx);
  switch (tgt) {
  case 0x006a7be5: goto L_6a7be5;
  case 0x006a7c33: goto L_6a7c33;
  case 0x006a7c0c: goto L_6a7c0c;
  case 0x006a7c5a: goto L_6a7c5a;
  default: goto L_6a708a;
  }

  /* ---------------- format blocks ---------------- */

L_6a6ab6:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6ac6 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0fbac, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6aee */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0fad8, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6b20:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6b30 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0fa34, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6b58 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f958, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6b8a:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6b9a */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f8ac, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6bc2 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f7f8, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6bec:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6bfc */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f750, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6c24 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f694, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6c4e:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6c5e */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f5f4, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6c86 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f548, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6cb0:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6cc0 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f49c, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6ce8 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f3f0, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6d56:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6d66 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f350, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6d9e */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f2a0, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6dc8:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6dd8 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f204, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6e00 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f164, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6e2a:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6e3a */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f0bc, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6e62 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0efec, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6e94:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6ea2 jumps to 0x006a6c5e */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0f5f4, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6ea8 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ef3c, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6ed2:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6ee2 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ee98, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6f0a */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ede4, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6f58:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6f68 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ed4c, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6f90 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ecac, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a6fba:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a6fca */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ec04, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a6ff2 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0eb54, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a701c:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a702c */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0eab4, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7054 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ea10, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

  /* 0x006a708a: unmatched selector. */
L_6a708a:
  if ((edi_v & 0x800) == 0) goto L_6a7fd3;
  /* 0x006a7096 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a0e94c, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  return 0;

L_6a7fd3:
  /* 0x006a7fd3 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  if ((edi_v & 0x400) != 0) {
    /* 0x006a7fea */
    cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a0be98, G, G, E, E,
                   (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
    return 0;
  }
  /* 0x006a8024 */
  cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a0bdd0, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  return 0;

L_6a7127:
  /* 0x006a7127 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e86c, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7159:
  /* 0x006a7159 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e790, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a718b:
  /* 0x006a718b */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e6cc, E, E, G, G,
                 (int)(scratch + 4), s4, s2 + 0x38, s1 + 4);
  goto L_6a7c8f;

L_6a7211:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7221 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e62c, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7259 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e55c, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a728b:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a729b */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e4c8, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a72c3 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e404, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a72f5:
  /* 0x006a72f5 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e350, E, E, G, G,
                 (int)(scratch + 4), s4, s2 + 0x38, s1 + 4);
  goto L_6a7c8f;

L_6a7327:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7337 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e2c8, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a735f */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e210, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7391:
  /* 0x006a7391 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e168, E, E, G, G,
                 (int)(scratch + 4), s4, s2 + 0x38, s1 + 4);
  goto L_6a7c8f;

L_6a73c3:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a73d3 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e0d0, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a73fb */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0e008, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7451:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7461 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0df7c, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7489 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0dec0, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a74bb:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a74cb */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0de38, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a74f3 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0dd88, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7536:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7546 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0dcd4, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a756e */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0dc08, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a75f0:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7600 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0db68, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7628 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0da90, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a765a:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a766a */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d9dc, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7692 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d908, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a76c4:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a76d4 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d85c, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a76fc */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d794, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a772e:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a773e */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d6f0, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7766 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d624, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7798:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a77a8 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d560, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a77d0 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d490, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7802:
  /* 0x006a7802: ninth and tenth arguments swap places. */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d3cc, E, E, G, G,
                 (int)(scratch + 4), s4, s2 + 0x38, s1 + 4);
  goto L_6a7c8f;

L_6a7834:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7844 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d314, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a786c */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d244, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a789e:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a78ae */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d194, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a78d6 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d0d0, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7905:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7915 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0d030, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a793d */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cf74, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a797f:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a798f */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ceb4, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a79b7 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cdec, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7a18:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7a28 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cd48, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7a60 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cc70, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7a92:
  /* 0x006a7a92 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cbd0, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7abc:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7acc */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0cb30, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7af1 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0ca60, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7b23:
  recv = CM_GLOBAL(u32, 0x00ae2c74);
  if ((u32)cm_fn_005c6710((void *)recv, s2) != 0) {
    /* 0x006a7b33 */
    G = (int)*(signed char *)(s1 + 0x37);
    cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c9b8, G, G,
                   (int)(scratch + 4), s4, s1 + 4);
    goto L_6a7c8f;
  }
  /* 0x006a7b6b */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c8f4, G, G, E, E,
                 (int)(scratch + 4), s4, s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7b9d:
  /* 0x006a7b9d */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c854, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7be5:
  /* 0x006a7be5 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c780, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7c0c:
  /* 0x006a7c0c */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c6b8, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7c33:
  /* 0x006a7c33 */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c610, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

L_6a7c5a:
  /* 0x006a7c5a */
  E = (int)*(signed char *)(s2 + 0x52);
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)(scratch + 0xcc), (char *)0x00a0c560, G, G, E, E,
                 (int)(scratch + 4), s1 + 4, s2 + 0x38);
  goto L_6a7c8f;

  /* ---------------- comment selection ---------------- */

L_6a7c8f:
  /* 0x006a7c8f: fifteenth stack argument low byte. */
  if ((u8)s14 == 0) goto L_6a7f9a;
  if ((u8)s10 != 2) goto L_6a7e35;

  /* 0x006a7cad */
  if ((signed char)s11 < (signed char)s12) goto L_6a7f9a;
  if ((signed char)s12 > (int)(signed char)s11 + 4) {
    cfmt = (const char *)0x00a0c514;
    goto L_6a7d39;
  }
  if ((signed char)s12 > (int)(signed char)s11 - 4) {
    cfmt = (const char *)0x00a0c4c4;
    goto L_6a7d39;
  }
  /* 0x006a7cf5 */
  if ((signed char)s12 <= (signed char)s11) goto L_6a7f9a;
  /* 0x006a7cfd */
  u = 0x49 * *(u32 *)(s0);
  G = (int)*(u16 *)(u + *(u32 *)(scratch + 0) + 0xe);
  if (((u32)G & 1) != 0) {
    cfmt = (const char *)0x00a0c474;
    goto L_6a7d39;
  }
  if (((u32)G & 2) != 0) {
    cfmt = (const char *)0x00a0c42c;
    goto L_6a7d39;
  }
  if (((u32)G & 4) != 0) {
    cfmt = (const char *)0x00a0c3e8;
    goto L_6a7d39;
  }
  if (((u32)G & 8) != 0) {
    cfmt = (const char *)0x00a0c3a0;
    goto L_6a7d39;
  }
  if (((u32)G & 0x40) != 0) {
    cfmt = (const char *)0x00a0c354;
    goto L_6a7d39;
  }
  if (((u32)G & 0x20) != 0) {
    cfmt = (const char *)0x00a0c310;
    goto L_6a7d39;
  }
  cfmt = (const char *)0x00a0c2c4;
  goto L_6a7d39;

L_6a7e35:
  /* 0x006a7e35 */
  if ((u8)s10 != 0xfe) goto L_6a7f9a;
  if ((signed char)s11 < (signed char)s13) goto L_6a7f9a;
  E = (int)(signed char)s12;
  G = (int)(signed char)s11;
  if (E > G + 4) {
    cfmt = (const char *)0x00a0c270;
    goto L_6a7d39;
  }
  if (E >= G - 4) goto L_6a7ef9;
  /* 0x006a7e7e */
  u = *(u32 *)(s0 + 0x57);
  if (u == 0) goto L_6a7ef9;
  u = *(u32 *)(CM_GLOBAL(u32, 0x00adadfc) + 4 * *(u32 *)u);
  if (u == 0) goto L_6a7ef9;
  {
    /* object at `u`; bl = signed byte at +0x36 */
    int bl = (int)*(signed char *)(u + 0x36);
    E = (int)(signed char)s12;                 /* esi */
    /* VA 0x006a7e93..0x006a7ea5: saved receiver, then its table base. */
    u32 row = 0x49 * *(u32 *)s0;
    u32 receiver = *(u32 *)(scratch + 0);
    u32 base = *(u32 *)receiver;
    if (((u32)(*(u8 *)(row + base + 0xe)) & 0x20) != 0) {
      /* 0x006a7eac: ECX = object, indirect call through vtable slot +0x9c. */
      u32 vt = *(u32 *)u;
      u32 rv = ((u32 (CM_THISCALL *)(void *))CM_GLOBAL(u32, vt + 0x9c))((void *)u);
      if (rv != 0) {
        u32 obj = *(u32 *)(CM_GLOBAL(u32, 0x00adadfc) + 4 * *(u32 *)(*(u32 *)(s0 + 0x57)));
        int diff = bl - (int)*(signed char *)(obj + 0xc1);
        if (E > diff) {
          cfmt = (const char *)0x00a0c22c;
          goto L_6a7d39;
        }
      }
    }
  }
  cfmt = (const char *)0x00a0c1d8;
  goto L_6a7d39;

L_6a7ef9:
  /* 0x006a7ef9 */
  if (E <= G + 2) goto L_6a7f9a;
  u = 0x49 * *(u32 *)(s0);
  /* VA 0x006a7f0d dereferences the unchanged EBX receiver. */
  G = (int)*(u16 *)(u + *(u32 *)self + 0xe);
  if (((u32)G & 1) != 0) {
    cfmt = (const char *)0x00a0c174;
    goto L_6a7d39;
  }
  if (((u32)G & 2) != 0) {
    cfmt = (const char *)0x00a0c118;
    goto L_6a7d39;
  }
  if (((u32)G & 4) != 0) {
    cfmt = (const char *)0x00a0c0c0;
    goto L_6a7d39;
  }
  if (((u32)G & 8) != 0) {
    cfmt = (const char *)0x00a0c068;
    goto L_6a7d39;
  }
  if (((u32)G & 0x40) != 0) {
    cfmt = (const char *)0x00a0c008;
    goto L_6a7d39;
  }
  if (((u32)G & 0x20) != 0) {
    cfmt = (const char *)0x00a0bfc0;
    goto L_6a7d39;
  }
  cfmt = (const char *)0x00a0bf5c;
  goto L_6a7d39;

L_6a7d39:
  /* 0x006a7d39: two arguments. */
  cm_fn_0066f4e0((int)(scratch + 0x70c), (char *)cfmt);
  if (*(u8 *)(scratch + 0x70c) != 0) goto L_6a7d9f;

L_6a7d4c:
  /* 0x006a7d4c */
  if (((u32)edi_v & 0x1800) == 0) goto L_6a7fa7;
  /* 0x006a7d58 */
  u = s8;
  *(volatile u8 *)&s9 = (u8)(((u32)edi_v >> 11) & 1);
  {
    u32 s9d = *(volatile u32 *)&s9;
    u32 s7v = s7;
    u32 ths = *(u32 *)(scratch + 0);
    cm_fn_006ab2d0((void *)ths, (int *)s7v, (char)(u8)s9d,
                   (u8 *)(scratch + 0x70c), (int)u);
  }
  if (*(u8 *)(scratch + 0x70c) != 0) goto L_6a7d9f;

L_6a7fa7:
  /* 0x006a7fa7 */
  cm_fn_00944c9f(0x00de1f64u, 0x00991944u, (u32)(scratch + 0xcc));
  return 1;

L_6a7f9a:
  /* 0x006a7f9a */
  *(u8 *)(scratch + 0x70c) = 0;
  goto L_6a7d4c;

L_6a7d9f:
  /* 0x006a7d9f */
  cm_fn_00944c9f(0x00de1f64u, 0x00991948u, (u32)(scratch + 0xcc),
                (u32)(scratch + 0x70c));
  return 1;

L_6a805e:
  /* 0x006a805e: third stack argument is null. The reload of [esp+0xd00] there
     has only the four saved-register pushes live, so the object is the second
     stack argument (s1); the name buffer at L+4 has not been written on this
      path. UNRESOLVED unwritten-format-buffer: this independent scratch
      allocation does not transport the original frame's preexisting L+4
      bytes. VA 0x006a69d9 reaches this formatter before any name-buffer write;
      no prior-write or unreachability proof is available in this listing.
      Do not clear the buffer or substitute a fabricated string. */
  G = (int)*(signed char *)(s1 + 0x37);
  cm_fn_0066f4e0((int)0x00de1f64, (char *)0x00a0bd50, G, G,
                 (int)(scratch + 4), s1 + 4);
  return 0;
}
