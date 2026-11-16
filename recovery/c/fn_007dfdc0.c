/* cm_fn_007dfdc0 -- preferred-image VA 0x007dfdc0, CM0102 3.9.68
   (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).
   Reconstructed from the original instructions only.

   Frame map.  L is the stack pointer immediately after `sub esp,0xa4`
   (0x007dfdd5); L[0x00..0xa3] is the single scratch block.  Every original
   `[esp+d]` below is mapped with the pushes that are live at that
   instruction (prologue pushes ebx/ebp => esp=L+8; `push esi` at 0x007dfe05
   => esp=L+4; `push edi` at 0x007dff1c => esp=L+4; `push 0x70` at
   0x007dff50 => esp=L).

     L+0x02  byte                flag_a     0x007dfde6 st, 0x007dffdc ld
     L+0x03  byte                flag_b     0x007dfdeb st, 0x007dff82/0x7dff6e
     L+0x04  8-byte object        obj_A      g1/g2 copied at 0x007dfe55/59
     L+0x0c  20-byte object       obj_R2     fields 0x007e000f..0x007e006f
     L+0x10  8-byte object        obj_D      0x007dffe9 (ECX at 0x7e001f)
     L+0x20  8-byte object        obj_B      0x007dfe2a (ECX at 0x7dfe39)
     L+0x28  dword                handle     0x007dfe17 st, 0x007dff64 ld
     L+0x2c  8-byte object        obj_C      0x007dfe21 (ECX at 0x7dfe25)
     L+0x34  112-byte record      rec        ECX 0x007e82e0/0x7e8e10 out;
                                              src of cm_fn_0068d0d0 0x007dff5f
       rec+0x04 (L+0x38) 8-byte object  0x007dfe25 ECX (date ctor)
       rec+0x0c (L+0x40) 4 tag bytes     0x007dff3c..0x007dff4b
       rec+0x14 (L+0x48) 8-byte object  0x007dfe39 ECX (date ctor)
       rec+0x20 (L+0x54) 79-byte payload 0x007dff52 rep movsd/movsw/movsb
   The two frame holes inside rec (rec+0x10..+0x13 = L+0x44..0x47 and
   rec+0x6f = L+0xa3) are never written by any instruction of 0x007dfdc0, so
    this C allocation leaves them uninitialized, but does NOT transport the
    original physical stack bytes. Fidelity of the 112-byte call boundary
    remains UNKNOWN. Zeroing or shortening the copy would invent behavior.
    Resolving this requires an authorized original-stack transport boundary
    or original-instruction proof that every exposed stale byte is unobserved.

   Note on the cm_fn_00557280 call (0x007e009b): the listing loads ECX from
   0x00ae28f0, but the callee's recovered C signature is CM_STDCALL with
   three stack arguments, so no C argument carries that value. */

#include "cmre.h"

typedef struct {
  void *prev;
  void *handler;
  u32 state;
} cmre_seh_frame;

#define CM_AE23BC CM_GLOBAL(u32, 0x00ae23bc)
#define CM_AE28F0 CM_GLOBAL(u32, 0x00ae28f0)
#define CM_AE2C90 CM_GLOBAL(u32, 0x00ae2c90)
#define CM_AE2C94 CM_GLOBAL(u32, 0x00ae2c94)

/* Receiver (ECX) first for each CM_THISCALL declaration, as the listing sets
   ECX immediately before the call. */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern int CM_FASTCALL cm_fn_0054a3f0(short *param_1);
/* 0x007dfeb0 / 0x007dfeee push EAX straight after this call, so the original
   return is the EAX result (the legacy record declared void). */
extern u32 CM_THISCALL cm_fn_0054c5b0(short *param_1, u32 *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_00556940(u32 self, volatile u32 key_arg, volatile u32 record_arg);
extern void CM_STDCALL cm_fn_00557280(int param_1, char *param_2, int param_3);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, volatile u32 create);
extern u32 CM_THISCALL cm_fn_007e8e10(volatile u8 *input, volatile u8 *out, volatile u8 *mode_data, u32 use_mode);

void CM_THISCALL cm_fn_007dfdc0(void *self, volatile void *param_1, const void *param_2)
{
  u8 L[0xa4];
  volatile cmre_seh_frame frame;
  volatile u8 *p1;
  const u8 *p2;
  u32 h;
  u8 b;
  u32 v;
  u32 key;
  u32 i;
  u8 c;
  s8 c1;

  p1 = (volatile u8 *)param_1;
  p2 = (const u8 *)param_2;

  /* SEH prologue: state, handler, previous, then link (0x007dfdc0..0x007dfdce) */
  frame.state = 0xffffffffu;
  frame.handler = (void *)(unsigned long)0x009602acUL;
  frame.prev = (void *)CM_FS(void *, 0);
  CM_FS(void *, 0) = (void *)(unsigned long)&frame;

  L[0x02] = 0;                        /* 0x007dfde6 */
  L[0x03] = 0xff;                     /* 0x007dfdeb */

  if ((p1[0x2c] == 0) && (p1[0x2d] == 0)) {   /* 0x007dfdf0..0x007dfdfc */
    goto epilogue;                    /* 0x007e00a6 */
  }

  h = cm_fn_007e82e0((u32)(unsigned long)self, *(volatile u32 *)(void *)p1, 1u);
  *(volatile u32 *)(void *)(L + 0x28) = h;      /* 0x007dfe17 */

  if (p1[0x2d] != 1) {                /* 0x007dfe12 */
    b = L[0x03];                      /* 0x007dff82 */
    goto lab_7dff86;
  }

  cm_fn_0054a110((u16 *)(void *)(L + 0x38));    /* 0x007dfe25 */
  frame.state = 0u;                             /* 0x007dfe2e (dword) */
  cm_fn_0054a110((u16 *)(void *)(L + 0x48));    /* 0x007dfe39 */
  v = CM_AE2C90;                                /* 0x007dfe3e */
  i = CM_AE2C94;                                /* 0x007dfe44 */
  frame.state = 1u;                             /* 0x007dfe4a (dword) */
  *(u32 *)(void *)(L + 0x04) = v;               /* 0x007dfe55 */
  *(u32 *)(void *)(L + 0x08) = i;               /* 0x007dfe59 */

  c = p1[0x2c];                                 /* 0x007dfe5d */
  *(volatile u8 *)&frame.state = 2;             /* 0x007dfe60 (byte) */
  if (c == 0) {                                 /* 0x007dfe68 */
    c1 = (s8)p1[0x2e];                          /* 0x007dfe6c */
  }
  else if (c == 1) {                            /* 0x007dfe71 */
    c1 = (s8)p1[0x2f];                          /* 0x007dfe79 */
  }
  else {
    goto lab_7dff69;                            /* 0x007dfe73 */
  }
  if (c1 <= 9) {                                /* 0x007dfe7c (signed byte) */
    goto lab_7dff69;                            /* 0x007dfe7e */
  }

  /* 0x007dfe84..0x007dfe8e: pushes 1, param_1, &rec */
  if (cm_fn_007e8e10((volatile u8 *)self, (volatile u8 *)(void *)(L + 0x34),
                     p1, 1u) == 0) {            /* 0x007dfe93 */
    goto lab_7dff69;                            /* 0x007dfe95 */
  }

  v = cm_fn_0054c5b0((short *)(unsigned long)0x00ae2c90UL,   /* ECX 0x007dfea2 */
                     (u32 *)(void *)(L + 0x2c), 1);          /* 0x007dfea7 */
  *(volatile u8 *)&frame.state = 3;             /* 0x007dfeb6 (byte) */
  cm_fn_00417c10((u32 *)(void *)(L + 0x04),     /* ECX 0x007dfeb2 */
                 (u32 *)(void *)(L + 0x20),
                 (u16 *)(unsigned long)v);      /* 0x007dfebe */
  *(volatile u8 *)&frame.state = 2;             /* 0x007dfec7 (byte) */
  while ((s16)cm_fn_0054a3f0((short *)(void *)(L + 0x04)) != 3) {     /* 0x007dfecf */
    v = cm_fn_0054c5b0((short *)(void *)(L + 0x04),   /* ECX 0x007dfee1 */
                       (u32 *)(void *)(L + 0x20), 1); /* 0x007dfee5 */
    *(volatile u8 *)&frame.state = 4;           /* 0x007dfef4 (byte) */
    cm_fn_00417c10((u32 *)(void *)(L + 0x04),   /* ECX 0x007dfef0 */
                   (u32 *)(void *)(L + 0x2c),
                   (u16 *)(unsigned long)v);    /* 0x007dfefc */
    *(volatile u8 *)&frame.state = 2;           /* 0x007dff05 (byte) */
  }
  /* 0x007dff18..0x007dff27: ECX = &rec[0x14], args &obj_B, &obj_A */
  cm_fn_00417c10((u32 *)(void *)(L + 0x48),
                 (u32 *)(void *)(L + 0x20),
                 (u16 *)(void *)(L + 0x04));

  L[0x40] = 0x11;                     /* 0x007dff3c */
  L[0x41] = 0xff;                     /* 0x007dff41 */
  L[0x42] = 0xff;                     /* 0x007dff46 */
  L[0x43] = 0xff;                     /* 0x007dff4b */

  /* 0x007dff52..0x007dff5e: 0x13 dwords + 1 word + 1 byte from param_2 into
     rec+0x20 (76 + 2 + 1 bytes); nothing is written past that. */
  {
    u8 *d = L + 0x54;
    const u8 *s = p2;
    for (i = 0; i < 0x13u; i++) {
      ((u32 *)(void *)d)[i] = ((const u32 *)(const void *)s)[i];
    }
    *(u16 *)(void *)(d + 0x4c) = *(const u16 *)(const void *)(s + 0x4c);
    d[0x4e] = s[0x4e];
  }

  /* 0x007dff5b ECX = [self+7]; 0x007dff50 count = 0x70; 0x007dff5a src = &rec.
     The original's EDX at the call is the physical leftover of the
      cm_fn_00417c10 call at 0x007dff27 (not 0x007dfebe). No instruction
      between 0x007dff2c and 0x007dff5f writes EDX. The zero below is an
      UNRESOLVED fidelity defect, not an evidenced register value.
      Both blockers above refer to preferred-image VAs in CM0102 3.9.68,
      SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
      Plain C here has no physical-stack or post-call EDX transport primitive;
      do not treat a passing semantic gate as resolving either blocker. */
  cm_fn_0068d0d0((volatile u8 *)(unsigned long)
                   *(volatile u32 *)(void *)((u8 *)self + 7),
                 0u, (const u8 *)(void *)(L + 0x34), (s16)0x70);

  h = *(volatile u32 *)(void *)(L + 0x28);      /* 0x007dff64 */

lab_7dff69:
  L[0x02] = 1;                        /* 0x007dff69 */
  L[0x03] = 2;                        /* 0x007dff6e */
  b = 1;                              /* 0x007dff73 */
  frame.state = 0xffffffffu;          /* 0x007dff75 (dword) */

lab_7dff86:
  if (p1[0x2c] != 0) {                /* 0x007dff86 */
    u8 n = (u8)(*(volatile u8 *)(void *)(unsigned long)(h + 4u) + 1u);  /* 0x007dff8d */
    *(volatile u8 *)(void *)(unsigned long)(h + 4u) = n;                /* 0x007dff94 */
    if ((n == 4) || (n == 7) || ((n > 8) && ((n & 1) != 0))) {          /* 0x007dff97..0x7dffb8 */
      L[0x03] = 1;                    /* 0x007dffc1 */
      L[0x02] = (u8)(L[0x02] + 1u);   /* 0x007dffca */
      if (n == 4) {
        b = n;                        /* 0x007dffd0 */
      }
      else {
        b = (u8)(((n == 7) ? 1u : 0u) + 2u);   /* 0x007dffd6, 0x007dffd9 */
      }
    }
  }

  if (L[0x02] != 0) {                 /* 0x007dffdc, 0x007dffe1 */
    cm_fn_0054a110((u16 *)(void *)(L + 0x10));   /* 0x007dffe9 */
    c = L[0x02];                                 /* 0x007dfff2 */
    frame.state = 5u;                            /* 0x007e0004 (dword) */
    L[0x0c] = c;                                 /* 0x007e000f */
    *(u16 *)(void *)(L + 0x0e) = 0xffff;         /* 0x007e0013 */
    L[0x0d] = 0;                                 /* 0x007e001a */
    /* 0x007dfffa..0x007e001f: ECX = &obj_D, args &obj_B, 0x00ae2c90 */
    cm_fn_00417c10((u32 *)(void *)(L + 0x10),
                   (u32 *)(void *)(L + 0x20),
                   (u16 *)(unsigned long)0x00ae2c90UL);

    *(u16 *)(void *)(L + 0x1d) = (u16)(s16)(s8)b;    /* 0x007e002b */
    c = p1[0x48];                                    /* 0x007e0028 */
    v = L[0x1f];                                     /* 0x007e0030 */
    {
      u8 dl = L[0x03];                               /* 0x007e0034 */
      u8 al = (u8)(v & 0xfd);                        /* 0x007e0038 */
      L[0x1c] = dl;                                  /* 0x007e003a */
      al = (u8)(al & 0xfb);                          /* 0x007e0046 */
      al = (u8)(al | (u8)((u8)((c == 0) ? 1u : 0u) << 2));   /* 0x007e0040..0x7e004b */
      L[0x1f] = al;                                  /* 0x007e0054 */
    }
    key = *(volatile u32 *)(void *)p1;               /* 0x007e004d */
    L[0x18] = 0x11;                                  /* 0x007e0060 */
    L[0x19] = 0xff;                                  /* 0x007e0065 */
    L[0x1a] = 0xff;                                  /* 0x007e006a */
    L[0x1b] = 0xff;                                  /* 0x007e006f */
    /* 0x007e0058 ECX = [0x00ae28f0], pushes &obj_R2 then key */
    cm_fn_00556940(CM_AE28F0, key, (u32)(unsigned long)(void *)(L + 0x0c));  /* 0x007e0074 */

    key = *(volatile u32 *)(void *)p1;               /* 0x007e0080 */
    /* 0x007e0079 loads the arg2 slot (0x00ae... caller's param_2) before the
       reload of [param_1]; 0x007e0088..0x007e0097 build 0x00ae23bc+key*110. */
    cm_fn_00557280((int)(CM_AE23BC + (u32)(key * 110u)),
                   (char *)(void *)(L + 0x0c),
                   (int)(unsigned long)(const void *)p2);        /* 0x007e00a1 */
  }

epilogue:
  {
    void *prev = (void *)frame.prev;      /* 0x007e00a6 */
    CM_FS(void *, 0) = prev;              /* 0x007e00af */
  }
  return;
}
