/* fn_00775cd0 - recovered from the 3.9.68 image (preferred-image VA 0x00775cd0,
   module cm3\code\national_teams.cpp).  thiscall: ECX = this (edi), stack args
   are (record pointer, short threshold); `ret 8` pops the two stack arguments.

   Frame (L = esp right after `sub esp,0x210`; the four saved registers live at
   L-0x10..L-0x01, so every [esp + n] below is translated through that base):
     L+0x00  kind byte (byte stores, later dword reloads)
     L+0x04  index slot (dword store, byte store, dword reloads)
     L+0x08  nation slot
     L+0x0c  base slot
     L+0x10  buffer A (0x100 bytes)
     L+0x110 buffer B (0x100 bytes) */
#include "cmre.h"

#define G_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_00ae2c90 CM_GLOBAL(u32, 0x00ae2c90)
#define G_00ae2c94 CM_GLOBAL(u32, 0x00ae2c94)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define G_00b63c98 CM_GLOBAL(u32, 0x00b63c98)
#define G_00b63d28 CM_GLOBAL(u32, 0x00b63d28)
#define G_00de1d08 CM_GLOBAL(u32, 0x00de1d08)

extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 value, u32 unused);
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_005409b0(u32 *input);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_005fd6b0(u32 self, u32 person, u32 output);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern u32 CM_THISCALL cm_fn_0062ed70(u32 self, u32 arg);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0076c060(u32 self, u32 nation);
extern u8 CM_STDCALL cm_fn_0076d810(u32 arg1, u32 arg2, u8 arg3);
extern u32 CM_STDCALL cm_fn_0076f7d0(volatile u32 person_arg, volatile u32 nation_arg, volatile u32 kind_arg);
extern u32 CM_STDCALL cm_fn_007701f0(volatile u32 person_arg, volatile u32 nation_arg, volatile u32 kind_arg, volatile u32 fourth_arg);
extern void CM_THISCALL cm_fn_00775f90(u32 self, u32 param_2, u32 param_3, s8 param_4, u32 param_5);
extern u8 CM_STDCALL cm_fn_007762f0(u32 value, u8 mode, u32 object);
extern u32 CM_THISCALL cm_fn_00896c60(u32 self, u32 club, volatile u32 person_arg, volatile u32 last_arg, volatile u32 option);
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define F_KIND_B (scratch[0x00])
#define F_KIND_D (*(volatile u32 *)&scratch[0x00])
#define F_IDX_D  (*(volatile u32 *)&scratch[0x04])
#define F_IDX_B  (scratch[0x04])
#define F_NAT_D  (*(volatile u32 *)&scratch[0x08])
#define F_BASE_D (*(volatile u32 *)&scratch[0x0c])
#define F_BUF_A  ((void *)(u32)&scratch[0x10])
#define F_BUF_B  ((void *)(u32)&scratch[0x110])

void CM_THISCALL cm_fn_00775cd0(int *self, int rec_, short threshold)
{
  volatile u8 scratch[0x210] __attribute__((aligned(4)));
  u8 *rec = (u8 *)(u32)rec_;
  u32 nation;
  u32 ebx;
  u32 base;

  /* 0x775cd0 sub esp,0x210 ; 0x775cd6 cmp word [esp+0x218],0xe ; jl exit */
  if (threshold < 14)
    return;

  /* 0x775ceb mov esi,[esp+0x224] ; 0x775cf2 mov ebp,[esi+0x24] */
  nation = *(volatile u32 *)(rec + 0x24);

  if (nation == 0) {
    char *msg;

    /* 0x775cf9..0x775d0d: cm_fn_00944cff(fmt, 0, 0, bufB, bufA) */
    cm_fn_00944cff(0x00a5b2a4u, nation, nation, F_BUF_B, F_BUF_A);
    /* 0x775d15..0x775d27: cm_fn_0090d130(bufB, bufA, 0x1e05) */
    msg = cm_fn_0090d130((u32)(u32)F_BUF_B, (u32)(u32)F_BUF_A, 0x1e05);
    /* 0x775d2c..0x775d37: cm_fn_00944c9f(&DAT, "%s %s %s %d", msg).  The
       three argument pushes of the 0x0090d130 call above are never popped
       before this call, so the vararg reads find bufB, bufA and 0x1e05 in
       those slots; pass them explicitly. */
    cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8u, (u32)msg,
                   (u32)(u32)F_BUF_B, (u32)(u32)F_BUF_A, (u32)0x1e05u);
    /* 0x775d3c..0x775d47 */
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, (int)nation);
    G_00b67a34 = nation;
    return;
  }

  /* 0x775d62 cmp byte [esi+0x28],0xb ; jne exit */
  if (*(volatile u8 *)(rec + 0x28) != 0x0b)
    return;

  /* 0x775d6c mov ebx,[ebp+0x53] */
  ebx = *(volatile u32 *)(nation + 0x53);

  /* 0x775d70..0x775d81: eax=[ebx]; 11*a; 55*a; ecx=this->f4 + edx*2 */
  {
    u32 a = *(volatile u32 *)ebx;
    u32 t = a * 5u;              /* lea ecx,[eax+eax*4] */
    t = a + t * 2u;              /* lea eax,[eax+ecx*2] -> 11*a */
    t = t + t * 4u;              /* lea edx,[eax+eax*4] -> 55*a */
    F_IDX_D = *(volatile u32 *)((u8 *)self + 4) + t * 2u;
  }

  /* 0x775d85 call 0x5409b0 (nation) */
  if (cm_fn_005409b0((u32 *)nation) != 0)
    F_KIND_B = 2;                                     /* 0x775d91 */
  else
    F_KIND_B = (u8)((*(volatile u8 *)(rec + 0x5e) >> 3) & 1); /* 0x775d98 */

  /* 0x775da4..0x775dab: cm_fn_0062ed70([0xb63d28], rec) */
  {
    char *q = (char *)cm_fn_0062ed70(G_00b63d28, (u32)rec);
    if (q != (char *)0 && *(volatile u8 *)q == 0x51) {
      cm_fn_00896c60(G_00de1d08, nation, (u32)rec, 1, 1);
      cm_fn_00896c60(G_00de1d08, nation, (u32)rec, 0, 1);
      cm_fn_007762f0(ebx, (u8)F_KIND_D, (u32)rec);
      return;
    }
  }

  /* 0x775df6..0x775e1a: base = index + kind*20 ; cm_fn_00456740(base+0x3a,
     [0xae2c90], [0xae2c94]) */
  {
    s32 k = (s32)(s8)F_KIND_B;
    u32 idx = F_IDX_D;
    u32 g94 = G_00ae2c94;
    u32 g90;
    u32 kk;

    kk = (u32)k * 5u;                       /* lea eax,[eax+eax*4] */
    base = idx + kk * 4u;                   /* lea eax,[ecx+eax*4] */
    g90 = G_00ae2c90;
    F_BASE_D = base;                        /* 0x775e13 */
    if (cm_fn_00456740(base + 0x3au, g90, g94) == 0)
      return;                               /* 0x775e1f je exit */
  }

  /* 0x775e27..0x775e72 */
  {
    u32 f0 = *(volatile u32 *)self;

    F_NAT_D = *(volatile u32 *)(rec + 0x24);        /* 0x775e2e */
    if (f0 != 0) {
      u32 node;

      *(volatile u32 *)(f0 + 0xa) = *(volatile u32 *)(f0 + 2);
      node = cm_fn_0068d1b0(*(volatile u32 *)self);
      while (node != 0) {
        u32 nat = F_NAT_D;

        if (*(volatile u32 *)node == *(volatile u32 *)nat) {
          if (cm_fn_00456740(0x00ae2c90u, *(volatile u32 *)(node + 8),
                             *(volatile u32 *)(node + 0xc)) != 0)
            return;
        }
        node = cm_fn_0068d1b0(*(volatile u32 *)self);
      }
    }
  }

  /* 0x775e74..0x775e7d: cm_fn_0076f7d0(rec, ebx, kind) */
  cm_fn_0076f7d0((u32)rec, ebx, F_KIND_D);

  /* 0x775e82..0x775ed6 */
  cm_fn_00896c60(G_00de1d08, nation, (u32)rec, 1, 1);
  cm_fn_00896c60(G_00de1d08, nation, (u32)rec, 0, 1);
  cm_fn_007762f0(ebx, (u8)F_KIND_D, (u32)rec);

  /* 0x775eb2..0x775edf: increment [base+0x45], flag kind==2, then store the
     call's AL over the low byte of the index slot */
  {
    u8 kind = (u8)F_KIND_D;              /* 0x775eb6 mov dl,[esp+0x10] */
    u32 b = F_BASE_D;                    /* 0x775eb2 mov eax,[esp+0x1c] */
    u8 cl = *(volatile u8 *)(b + 0x45);

    cl = (u8)(cl + 1);                   /* 0x775ebd inc cl */
    *(volatile u8 *)(b + 0x45) = cl;     /* 0x775ec2 */
    {
      u32 idx = F_IDX_D;                 /* 0x775ec5 mov ecx,[esp+0x14] */
      u8 r = cm_fn_0076d810((u32)rec, idx, (u8)(kind == 2));

      F_IDX_B = r;                       /* 0x775edf mov byte [esp+0x28],al */
    }
  }

  /* 0x775ed6..0x775ee8: cm_fn_00600a50(nation, 0, 1, 0, 0) */
  if (cm_fn_00600a50(nation, 0, 1, 0, 0) == 0) {
    if (*(volatile u32 *)(nation + 0xcf) != 0)
      goto fail_path;
    if (cm_fn_00600a50(cm_fn_0053b3d0(ebx), 0, 1, 0, 0) == 0)
      goto fail_path;
  }

  /* 0x775f17..0x775f3e */
  {
    u32 q = cm_fn_0053b3d0(ebx);
    u8 cl = (u8)F_KIND_D;                       /* 0x775f1d */
    u32 fcf = *(volatile u32 *)(q + 0xcf);      /* 0x775f21 */

    /* 0x00775f2c: JE skips the helper and goes straight to 0x00775f68. */
    if (cl == 0)
      goto success_path;
    if (cm_fn_005fd6b0(G_00b63c98, fcf, 0) != 0)
      goto success_path;
  }

fail_path:
  /* 0x775f40..0x775f56 */
  cm_fn_00775f90((u32)(u32)self, ebx, F_KIND_D, (s8)F_IDX_D, (u32)rec);
  cm_fn_0076c060((u32)(u32)self, ebx);
  return;

success_path:
  /* 0x775f68..0x775f76 */
  cm_fn_007701f0((u32)rec, ebx, F_KIND_D, F_IDX_D);
  return;
}
