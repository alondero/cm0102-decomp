/* fn_008cb340 -- CM 01/02 3.9.68 (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86),
   preferred-image VA 0x008cb340, image base 0x00400000.
   Entry: thiscall (ECX = this, read before written at 0x008cb34b); four stack
   arguments, popped by the callee (`ret 0x10` at 0x008cb4d2 / 0x008cb4de).

   Stack frame (original):
     L      = ESP right after `sub esp,0x10` (0x008cb340); 16 scratch bytes at L..L+15
     saved  = ebx, ebp, esi, edi pushed at 0x008cb343..0x008cb34a
   Argument homes (ESP at entry = E0): arg1 @ E0+4, arg2 @ E0+8, arg3 @ E0+12, arg4 @ E0+16.
     0x008cb345 mov ebp,[esp+0x1c] -> arg1 (ESP = L-24 there)
     0x008cb384 mov ebx,[esp+0x2c] -> arg3 (ESP = L-16 there)
     0x008cb3d3 mov eax,[esp+0x28] -> arg2 (ESP = L-16 after `add esp,0x14`)

   Local scratch: 0x008cb393 stores *arg1 at [L]; 0x008cb3b7 `lea eax,[esp+0x1c]`
   (ESP = L-28) yields L, which is passed to cm_fn_00946464 as its first argument.
   The other 12 scratch bytes stay uninitialized, as in the original. */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Original globals at their preferred-image VAs. All four are DWORD reads in
   the listing, so they are read/written as volatile u32 to keep the width and
   the access order of the original instructions. */
#define G_00ad9dac CM_GLOBAL(volatile u32, 0x00ad9dac)
#define G_00ae1050 CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_00ae1080 CM_GLOBAL(volatile u32, 0x00ae1080)
#define G_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_00ae2a38 CM_GLOBAL(volatile u32, 0x00ae2a38)

/* 0x008b4e00 is pushed as a plain address (0x008cb38e). */
#define PTR_008b4e00 ((void *)0x008b4e00)

/* Callees stay explicit boundaries to the original code. */
extern u32 CM_THISCALL cm_fn_00448ad0(u32 self, u32 amount, u32 club1, u32 club2);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_00599800(u32 self, u32 low, u32 high);
extern u32 CM_THISCALL cm_fn_00599890(u32 self, u32 low, u32 high);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
/* 0x008cb34b saves incoming ECX in EDI; 0x008cb3f0 restores it to ECX.
   Four stack arguments follow at 0x008cb3f2..0x008cb3f5; the caller's
   epilogue at 0x008cb4c6 shows that the callee removes them. */
extern void CM_THISCALL cm_fn_008db8a0(u32 self, int *param_1, u32 param_2, u32 param_3, int param_4);
/* 0x00946464 is cdecl here: the original caller removes the five arguments with
   `add esp,0x14` at 0x008cb3c4. */
extern u32 cm_fn_00946464(u32 param_1, u32 param_2, u32 param_3, int param_4, void *param_5);

u32 CM_THISCALL
cm_fn_008cb340(u32 self, u32 *arg1, s32 arg2, u32 arg3, u32 arg4)
{
  u32 p1;        /* eax */
  u32 v;         /* indexed value loaded from a record */
  u32 addr;      /* base + 581 * v */
  u32 rec;       /* esi: record returned by cm_fn_00946464 */
  u32 club_hi;   /* ecx */
  u32 club_lo;   /* edx */
  u32 res1;      /* ebx: first cm_fn_005a0590 result */
  u32 res2;      /* edi: second cm_fn_005a0590 result */
  u32 a;         /* address argument of cm_fn_005a0590 */
  u32 c;         /* club1 argument of cm_fn_00448ad0 */
  u32 val;       /* dword at [esi+4] */
  s32 sv;        /* signed value of that dword (cdq at 0x008cb4b1/0x008cb4be) */
  u32 hi;        /* cdq high half */
  u32 scratch[4];

  /* 0x008cb34d mov ecx,[0xae1050]; 0x008cb353 push ebp; 0x008cb354 call 0x4cf5a0 */
  p1 = cm_fn_004cf5a0(G_00ae1050, (u32)arg1);
  /* 0x008cb35b mov dword ptr [0xae1080],eax */
  G_00ae1080 = p1;
  /* 0x008cb359 test eax,eax; 0x008cb360 je 0x8cb4d5 */
  if (p1 == 0)
    goto fail;

  /* 0x008cb366 mov eax,[eax+4]; 0x008cb369 mov edx,[0xae23b4];
     0x008cb36f add edx,eax; 0x008cb371..0x008cb379 lea eax,[edx+ecx*4] -> base + 581*v */
  v = *(volatile u32 *)(p1 + 4);
  addr = G_00ae23b4 + 581u * v;
  /* 0x008cb37c test eax,eax; 0x008cb37e je 0x8cb4d5 */
  if (addr == 0)
    goto fail;

  /* 0x008cb388 mov eax,[ebp]; 0x008cb393 mov dword ptr [esp+0x14],eax (== L) */
  scratch[0] = *arg1;

  /* 0x008cb38b cmp bl,5 */
  if ((u8)arg3 == 5) {
    /* 0x008cb39b mov ecx,[edi+0x8a0]; 0x008cb3a1 mov edx,[edi+0x898] */
    club_hi = *(volatile u32 *)(self + 0x8a0);
    club_lo = *(volatile u32 *)(self + 0x898);
  } else {
    /* 0x008cb3aa mov ecx,[edi+0x894]; 0x008cb3b0 mov edx,[edi+0x88c] */
    club_hi = *(volatile u32 *)(self + 0x894);
    club_lo = *(volatile u32 *)(self + 0x88c);
  }

  /* 0x008cb38e push 0x8b4e00; 0x008cb397 push 0xd; 0x008cb3a7/0x8cb3b6 push ecx;
     0x008cb3b7 lea eax,[esp+0x1c] (== L); 0x008cb3bb push edx; 0x008cb3bc push eax;
     0x008cb3bd call 0x946464 */
  rec = cm_fn_00946464((u32)&scratch[0], club_lo, club_hi, 0xd, PTR_008b4e00);
  /* 0x008cb3c7 test esi,esi; 0x008cb3c9 je 0x8cb4d5 */
  if (rec == 0)
    goto fail;

  /* 0x008cb3cf movsx ecx,byte ptr [esi+8]; 0x008cb3d7 cmp ecx,eax (arg2);
     0x008cb3d9 je; 0x008cb3db cmp eax,-1; 0x008cb3de jne 0x8cb4d5 */
  if (((s32)(*(volatile s8 *)(rec + 8)) != arg2) && (arg2 != -1))
    goto fail;
  /* 0x008cb3e4 test eax,eax; 0x008cb3e6 jle 0x8cb4d5 */
  if (arg2 <= 0)
    goto fail;

  /* 0x008cb3ec mov edx,[esp+0x30] (arg4); 0x008cb3f0 mov ecx,edi;
     0x008cb3f2 push edx; 0x008cb3f3 push ebx (arg3); 0x008cb3f4 push eax (arg2);
     0x008cb3f5 push esi; 0x008cb3f6 call 0x8db8a0 */
  cm_fn_008db8a0(self, (int *)rec, (u32)arg2, arg3, (int)arg4);

  /* 0x008cb3fb mov eax,[esi+9]; 0x008cb3fe mov edx,[0xae23b4]; 0x008cb404 add edx,eax;
     0x008cb406..0x008cb40e lea eax,[edx+ecx*4] -> base + 581*v;
     0x008cb411 mov ecx,[0xae2a38]; 0x008cb417 push eax; 0x008cb418 call 0x5a0590 */
  v = *(volatile u32 *)(rec + 9);
  a = G_00ae23b4 + 581u * v;
  res1 = cm_fn_005a0590(G_00ae2a38, a);

  /* 0x008cb41d mov ecx,[0xae1050]; 0x008cb423 push ebp; 0x008cb426 call 0x4cf5a0 */
  p1 = cm_fn_004cf5a0(G_00ae1050, (u32)arg1);
  /* 0x008cb42d mov dword ptr [0xae1080],eax */
  G_00ae1080 = p1;
  /* 0x008cb432 je 0x8cb44c */
  if (p1 == 0) {
    a = 0;
  } else {
    /* 0x008cb434 mov eax,[eax+4]; 0x008cb437 mov edx,[0xae23b4];
       0x008cb43d..0x008cb447 lea eax,[edx+ecx*4] -> base + 581*v */
    v = *(volatile u32 *)(p1 + 4);
    a = G_00ae23b4 + 581u * v;
  }
  /* 0x008cb44e mov ecx,[0xae2a38]; 0x008cb454 push eax; 0x008cb455 call 0x5a0590 */
  res2 = cm_fn_005a0590(G_00ae2a38, a);

  /* 0x008cb45a mov ecx,[0xae1050]; 0x008cb460 push ebp; 0x008cb463 call 0x4cf5a0 */
  p1 = cm_fn_004cf5a0(G_00ae1050, (u32)arg1);
  /* 0x008cb468 mov edx,[0xae23b4] */
  addr = G_00ae23b4;
  /* 0x008cb46e mov dword ptr [0xae1080],eax */
  G_00ae1080 = p1;
  /* 0x008cb473 test eax,eax; 0x008cb475 je 0x8cb489 */
  c = 0;
  if (p1 != 0) {
    /* 0x008cb477 mov eax,[eax+4]; 0x008cb47a..0x008cb480 ecx = 145*v;
       0x008cb482 add eax,edx; 0x008cb484 lea ecx,[eax+ecx*4] -> base + 581*v */
    v = *(volatile u32 *)(p1 + 4);
    c = addr + 581u * v;
  }

  /* 0x008cb48b mov eax,[esi+9]; 0x008cb48e add edx,eax; 0x008cb490..0x008cb496 ebp = 145*v;
     0x008cb498 mov eax,[esi+4]; 0x008cb49b lea edx,[edx+ebp*4] -> base + 581*v.
     EDX retains the base loaded at 0x008cb468; there is no second global read. */
  v = *(volatile u32 *)(rec + 9);
  a = addr + 581u * v;
  val = *(volatile u32 *)(rec + 4);
  /* 0x008cb49e push edx; 0x008cb49f push ecx; 0x008cb4a0 mov ecx,[0xad9dac];
     0x008cb4a6 push eax; 0x008cb4a7 call 0x448ad0 */
  cm_fn_00448ad0(G_00ad9dac, val, c, a);

  /* 0x008cb4ac mov eax,[esi+4]; 0x008cb4af mov ecx,ebx; 0x008cb4b1 cdq;
     0x008cb4b2 push edx; 0x008cb4b3 push eax; 0x008cb4b4 call 0x599800 */
  val = *(volatile u32 *)(rec + 4);
  sv = (s32)val;
  hi = (sv < 0) ? 0xFFFFFFFFu : 0u;
  cm_fn_00599800(res1, (u32)sv, hi);

  /* 0x008cb4b9 mov eax,[esi+4]; 0x008cb4bc mov ecx,edi; 0x008cb4be cdq;
     0x008cb4bf push edx; 0x008cb4c0 push eax; 0x008cb4c1 call 0x599890 */
  val = *(volatile u32 *)(rec + 4);
  sv = (s32)val;
  hi = (sv < 0) ? 0xFFFFFFFFu : 0u;
  cm_fn_00599890(res2, (u32)sv, hi);

  /* 0x008cb4c6..0x008cb4d2: restore registers, eax = 1, ret 0x10 */
  return 1;

fail:
  /* 0x008cb4d5..0x008cb4de: restore registers, eax = 0, ret 0x10 */
  return 0;
}
