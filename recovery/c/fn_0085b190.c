/* fn_0085b190 — preferred-image VA 0x0085b190, image base 0x00400000.
   Translated from the original listing in the farm bundle
   (CM0102/Analysis/cmre/farm/20261115T1250Z-sol-commandcode-supervised/bundle/fn_0085b190.md).

   Frame facts taken from the listing:
   - "ret 0xc" pops 12 bytes: the function takes three stack arguments.
   - "mov eax, dword ptr [esp + 8]" loads argument 2 (the mode flag).
   - The routine keeps three caller-owned argument homes live on the stack:
       [E0+4]  argument 1 home   (byte store at 0x0085b204, reload at 0x0085b227)
       [E0+8]  argument 2 home   (byte store at 0x0085b211, reloads at 0x0085b2dd /
                                 0x0085b33d / 0x0085b451 / 0x0085b4fd; dword store at
                                 0x0085b24b, reload at 0x0085b267)
       [E0+12] argument 3 home   (reloads at 0x0085b254 / 0x0085b3bb / 0x0085b3d0 /
                                 0x0085b56f / 0x0085b581 / 0x0085b642)
     and two scratch homes inside "sub esp, 8":
       [E0-4]  dword, result of the index-3 call (store 0x0085b1f7, reads 0x0085b287 ...)
       [E0-5]  byte,  result of the index-6 call (store 0x0085b21d, reads 0x0085b293 ...)
   - cm_fn_00785830, cm_fn_004ae660, cm_fn_004ae770 and cm_fn_004ae8a0 are thiscall:
     the callee pops its stack arguments (the listing's epilogue only ever balances
     the frame it pushed itself).
   - cm_fn_0066f4e0 is cdecl and variadic: the listing cleans up 2, 3, 5, 8 or 9
     pushed arguments itself after each call.
*/

#include "cmre.h"

#define DAT_009cf6e8 CM_GLOBAL(u32, 0x009cf6e8)
#define DAT_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(u32, 0x00ae23d0)
#define BUF_00de1f64 ((char *)0x00de1f64)

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae770(u32 self, u32 addition);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);

/* The original ends with "ret 0xc": it pops its three stack arguments itself
   (callee-pops convention), so the entry keeps that convention. */
void CM_STDCALL cm_fn_0085b190(volatile u32 arg1, volatile u32 arg2, u32 arg3)
{
  u32 h3 = arg3;      /* argument-3 home */
  u32 self;
  u32 rec0 = 0;       /* edi: DAT_00ae23b4 + 581 * r0 */
  u32 rec1 = 0;       /* esi: DAT_00ae23d0 + 107 * r1 (zero path: DAT_00ae23b4 + 581 * r0) */
  u32 r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0;
  u32 home_idx3 = 0;  /* dword scratch home */
  u32 home_idx6 = 0;  /* byte scratch home */
  u8  bl = 0;
  char *fmt;
  s8  b0 = 0;         /* byte at record + 0x52 (edi) */
  s8  b1 = 0;         /* byte at record + 0x52 (esi) */
  u32 p0 = 0;         /* edi + 0x38 */
  u32 p1 = 0;         /* esi + 0x38 */

  if (arg2 == 0) {
    /* 0x0085b5d4 */
    self = arg1;
    r0 = (u32)cm_fn_00785830((void *)self, 0);
    rec1 = DAT_00ae23b4 + r0 + (r0 * 145u) * 4u;
    r2 = (u32)cm_fn_00785830((void *)self, 2);
    bl = (u8)r2;
    if (bl == 1) {
      fmt = (char *)0x00a82d3c;
    } else if (bl == 2) {
      fmt = (char *)0x00a82cf4;
    } else if (bl == 3) {
      fmt = (char *)0x00a82cac;
    } else {
      return;
    }
    b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
    p1 = rec1 + 0x38;
    cm_fn_0066f4e0((int)(u32)BUF_00de1f64, fmt, (int)b1, (int)b1, (u32)p1);
    cm_fn_004ae660((void *)(u32)h3, (u32)(u32)BUF_00de1f64);
    cm_fn_004ae8a0(h3, (const char *)p1, 0x7d5u, rec1);
    return;
  }

  /* 0x0085b1a5 */
  self = arg1;
  r0 = (u32)cm_fn_00785830((void *)self, 0);
  rec0 = DAT_00ae23b4 + r0 + (r0 * 145u) * 4u;
  r1 = (u32)cm_fn_00785830((void *)self, 1);
  rec1 = DAT_00ae23d0 + r1 * 107u;
  r2 = (u32)cm_fn_00785830((void *)self, 2);
  bl = (u8)r2;
  r3 = (u32)cm_fn_00785830((void *)self, 3);
  home_idx3 = r3;
  r4 = (u32)cm_fn_00785830((void *)self, 4);
  *(volatile u8 *)&arg1 = (u8)r4;
  r5 = (u32)cm_fn_00785830((void *)self, 5);
  *(volatile u8 *)&arg2 = (u8)r5;
  r6 = (u32)cm_fn_00785830((void *)self, 6);
  home_idx6 = r6 & 0xffu;                       /* byte store to the scratch home */

  if (bl == 1) {
    /* 0x0085b227 */
    s32 idx4 = *(volatile s8 *)&arg1;
    b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
    b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
    p1 = rec1 + 0x38;
    p0 = rec0 + 0x38;
    arg2 = p0;
    cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a834dc,
                   (int)b0, (int)b0, (int)b1, (int)b1,
                   (u32)p0, (int)idx4, (u32)p1);
    cm_fn_004ae660((void *)(u32)h3, (u32)(u32)BUF_00de1f64);
    cm_fn_004ae8a0(h3, (const char *)arg2, 0x7d5u, rec0);
    cm_fn_004ae8a0(h3, (const char *)p1, 0x7d0u, rec1);
    if (home_idx3 == 0) {
      return;
    }
    if ((s8)(u8)(home_idx6 & 0xffu) < 0) {
      return;
    }
    cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a834a8,
                   (int)(s8)(u8)(home_idx6 & 0xffu));
    cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
    return;
  }

  if (bl == 2) {
    /* 0x0085b2d4 */
    u32 d_esi = *(volatile u32 *)(u8 *)rec1;
    u32 d_cmp = DAT_009cf6e8;
    u8  idx5_byte = *(volatile u8 *)&arg2;
    s32 idx4 = *(volatile s8 *)&arg1;
    s32 idx5 = (s8)idx5_byte;
    s32 diff;

    if (d_esi == d_cmp) {
      diff = idx4 - idx5;                       /* 0x0085b2eb */
      if (diff == 1) {
        b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
        b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
        p1 = rec1 + 0x38;
        p0 = rec0 + 0x38;
        cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8340c,
                       (int)b0, (int)b0, (int)b1, (int)b1, (u32)p0, (u32)p1);
      } else {
        /* 0x0085b31d */
        b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
        p1 = rec1 + 0x38;
        p0 = rec0 + 0x38;
        b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
        if (idx5_byte != 0) {
          cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a83360,
                         (int)b0, (int)b0, (int)b1, (int)b1,
                         (u32)p0, (int)diff, (u32)p1);
        } else {
          cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a832b8,
                         (int)b0, (int)b0, (int)b1, (int)b1,
                         (u32)p0, (int)*(volatile s8 *)&arg1, (u32)p1);
        }
      }
    } else {
      diff = idx4 - idx5;                       /* 0x0085b34f */
      if (diff == 1) {
        b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
        b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
        p1 = rec1 + 0x38;
        p0 = rec0 + 0x38;
        cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8321c,
                       (int)b0, (int)b0, (int)b1, (int)b1, (u32)p0, (u32)p1);
      } else {
        /* 0x0085b37e */
        b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
        p1 = rec1 + 0x38;
        p0 = rec0 + 0x38;
        b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
        if (idx5_byte != 0) {
          cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8316c,
                         (int)b0, (int)b0, (int)b1, (int)b1,
                         (u32)p0, (int)diff, (u32)p1);
        } else {
          cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a830c4,
                         (int)b0, (int)b0, (int)b1, (int)b1,
                         (u32)p0, (int)*(volatile s8 *)&arg1, (u32)p1);
        }
      }
    }

    /* 0x0085b3bb */
    cm_fn_004ae660((void *)(u32)h3, (u32)(u32)BUF_00de1f64);
    cm_fn_004ae8a0(h3, (const char *)p0, 0x7d5u, rec0);
    cm_fn_004ae8a0(h3, (const char *)p1, 0x7d0u, rec1);
    if (home_idx3 == 0) {
      return;
    }
    {
      s8 idx6 = (s8)(u8)(home_idx6 & 0xffu);
      if (idx6 == 0) {
        /* 0x0085b3fd */
        cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8309c);
        cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
        return;
      }
      if (idx6 == 1) {
        /* 0x0085b429 */
        cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a83074);
        cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
        return;
      }
      /* 0x0085b451 */
      if (*(volatile u8 *)&arg2 != 0) {
        if (idx6 < 0) {
          goto copy_path;
        }
        cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8303c, (int)idx6);
        cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
        return;
      }
      if (idx6 < 0) {
        goto copy_path;
      }
      cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a8300c, (int)idx6);
      cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
      return;
    }

  copy_path:
    /* 0x0085b4b9 */
    {
      u32 ecx = 0xffffffffu;
      u32 edi = (u32)0x00ad9c64u;
      u32 esi;
      u32 edx;
      u8  al = 0;

      /* repne scasb */
      while (ecx != 0) {
        u8 b = *(volatile u8 *)edi;
        edi += 1;
        ecx -= 1;
        if (b == al) {
          break;
        }
      }
      ecx = ~ecx;
      edi = edi - ecx;
      edx = ecx;
      esi = edi;
      edi = (u32)0x00de1f64u;
      /* rep movsd */
      ecx = ecx >> 2;
      while (ecx != 0) {
        *(volatile u32 *)edi = *(volatile u32 *)esi;
        edi += 4;
        esi += 4;
        ecx -= 1;
      }
      /* rep movsb */
      ecx = edx & 3;
      while (ecx != 0) {
        *(volatile u8 *)edi = *(volatile u8 *)esi;
        edi += 1;
        esi += 1;
        ecx -= 1;
      }
    }
    cm_fn_004ae770(h3, (u32)0x00de1f64u);
    return;
  }

  if (bl == 3) {
    /* 0x0085b4fd */
    s32 idx5 = *(volatile s8 *)&arg2;
    s32 e = 25 - idx5;
    if (e >= 2) {
      fmt = (char *)0x00a82dc4;
    } else if ((e & 0xff) == 0) {
      fmt = (char *)0x00a82f34;
    } else if ((e & 0xff) == 1) {
      fmt = (char *)0x00a82e7c;
    } else {
      fmt = (char *)0x00a82dc4;
    }
    b1 = *(volatile s8 *)((u8 *)rec1 + 0x52);
    b0 = *(volatile s8 *)((u8 *)rec0 + 0x52);
    p1 = rec1 + 0x38;
    p0 = rec0 + 0x38;
    cm_fn_0066f4e0((int)(u32)BUF_00de1f64, fmt,
                   (int)b0, (int)b0, (int)b1, (int)b1, (u32)p0, (u32)p1);
    cm_fn_004ae660((void *)(u32)h3, (u32)(u32)BUF_00de1f64);
    cm_fn_004ae8a0(h3, (const char *)p0, 0x7d5u, rec0);
    cm_fn_004ae8a0(h3, (const char *)p1, 0x7d0u, rec1);
    if (home_idx3 == 0) {
      return;
    }
    cm_fn_0066f4e0((int)(u32)BUF_00de1f64, (char *)0x00a82d88);
    cm_fn_004ae770(h3, (u32)(u32)BUF_00de1f64);
    return;
  }

  return;
}
