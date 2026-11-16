/* Reconstruction of preferred-image VA 0x00423880 (image base 0x00400000), module
   e:\dev\cm3\cm3 00-01\cm3\code\comp\leagues\bel_third.cpp, from the original
   instructions of the pinned CM0102 3.9.68 binary
   (SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86).

   Frame (all offsets are from the byte after `sub esp,0x228` at 0x00423895):
     +0x000 dword  counted total (0x0042390f / 0x004239a0), reused at 0x00423aaa
                   for the allocation result
     +0x004 dword  first  output slot of the virtual call at 0x00423a98
                   (`lea edx,[esp+0x18]` with one dword already pushed)
     +0x008 dword  second output slot of the same call (`lea ecx,[esp+0x1c]`)
     +0x00c 6 dwords rank array (0x00423a1b even indices, 0x00423a5f odd indices)
     +0x024 dword  `[obj+0xb1]` record field +0x375, stored at 0x004238a-0x423a8a
                   and never read again (write-only local in the original)
     +0x028 256 bytes first string buffer (0x00423b08)
     +0x128 256 bytes second string buffer (0x00423b0c)
   The 0x228 bytes are never initialised by the original.

   The virtual call at 0x00423a98 pushes four dwords (`push 0`, two `lea`
   output pointers, `push 8`) and the routine adjusts ESP only for its own
   frame afterwards (0x00423b8e `add esp,0x234` = 0x228 + the three SEH words),
   so the target pops the pushed 16 bytes itself: RET 16, i.e. a four-argument
   callee with `this` in ECX.  Both output slots are re-read as full dwords by
   the caller (0x00423abb `mov ecx,[esp+0x18]`, 0x00423ad3 `mov edx,[esp+0x38]`),
   which is the only width evidence available.  The slots are therefore left
   uninitialised and read back as dwords; nothing here assumes the target's
   identity. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0051c800(u32 self, volatile u32 round_arg);
/* Caller-side DWORD carriers: 0x00423aca/0x00423add push the full output
   slots; 0x00423abf replaces only DX before 0x00423ad2 pushes EDX.
   Evidence: preferred-image VAs in the pinned binary identified above.
   This does not establish the virtual target's output-write widths. */
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs, u32 rounds,
                                       u32 competition, u32 schedule, u32 year, u8 flag_a,
                                       u8 flag_b, u32 round_flag, u8 flag_c, u32 builder,
                                       u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
/* Legacy boundary (original code only). The stack words pushed at
   0x00423b2f/0x00423b34/0x00423b35 remain live across 0x00423b36 and are
   also formatting arguments at 0x00423b46: no intervening cleanup.
   0x00423b5b removes six formatting words plus three error-report words.
   Evidence: preferred-image VAs in the pinned binary identified above. */
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* Virtual slots.  `call dword ptr [edx+NN]` follows `mov ecx, esi`, and the
   caller's stack balance shows the callee removes the pushed dwords
   (5 for +0x40 and +0x68, 4 for +0x3c), so these are thiscall/POP-args
   boundaries.  Slot +0x40's result is consumed at 0x0042392e and 0x004239cb
   (`mov al,[esi+0x44]` leaves EAX's high bytes in the pushed argument),
   slot +0x68's result is dead. */
typedef u32 (CM_THISCALL *cm_vt40)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
typedef void (CM_THISCALL *cm_vt68)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5);
typedef u32 (CM_THISCALL *cm_vt3c)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

#define L_COUNT  0x000
#define L_OUT_A  0x004
#define L_OUT_B  0x008
#define L_RANK   0x00c
#define L_TMP    0x024
#define L_BUF1   0x028
#define L_BUF2   0x128

#define RD32(p)   (*(u32 volatile *)(p))
#define RD16(p)   (*(u16 volatile *)(p))
#define RD8(p)    (*(u8 volatile *)(p))
#define WR32(p, v) (*(u32 volatile *)(p) = (u32)(v))

void CM_THISCALL cm_fn_00423880(u8 *self)
{
  /* One SEH record, linked and unlinked in listing order (0x00423886-0x0042388e,
     0x00423b87).  `state` is the dword written by 0x00423ab2 / 0x00423af0. */
  volatile struct { void *prev; void *handler; u32 state; } frame;
  volatile u8 scratch[0x228];

  u16 self_count;      /* cx  : [esi+0x3e] */
  u16 other_count;     /* dx  : [ebp+0x3e] */
  u32 list2;           /* ebp : [[esi+0xc]] */
  s32 total;           /* edx / ecx counters */
  u16 bx;              /* ebx */
  u32 idx;
  u8 *addr;
  u8 byte44;
  u32 arr;
  u32 club;
  u32 vt;
  u32 ret40;
  s16 di;
  u16 cx_i;
  s16 dx_o;
  u32 vt3c;
  u32 sel;             /* edi : virtual slot +0x3c result */
  u32 alloc;
  unsigned long long alloc_regs; /* EDX:EAX boundary carrier */
  u32 out_a, out_b, year_bits, competition;
  u32 res;
  u32 rec;
  u32 now;
  char *str;

  frame.state = 0xffffffffu;                 /* push -1        */
  frame.handler = (void *)0x0095419eu;       /* push 0x95419e  */
  frame.prev = CM_FS(void *, 0);             /* push fs:[0]    */
  CM_FS(void *, 0) = (void *)(u32)&frame;    /* mov fs:[0],esp */

  self_count = RD16(self + 0x3e);                    /* mov cx,[esi+0x3e] */
  list2 = RD32((u32)RD32(self + 0xc));               /* mov eax,[esi+0xc]; mov ebp,[eax] */
  total = 0;                                         /* xor edx,edx */
  if ((s16)self_count > 0) {                         /* test cx,cx; jle */
    u8 *p = (u8 *)(RD32(self + 0xb1) + 0x37);
    s32 n = (s32)(s16)self_count;
    do {
      if (*p == 1) {                                 /* cmp byte ptr [eax],1 */
        total = total + 1;
      }
      p = p + 0x3b;
      n = n - 1;
    } while (n != 0);
  }
  bx = 0;                                            /* xor ebx,ebx */
  if ((s16)self_count > 0) {
    for (;;) {
      if (total >= 3) break;                         /* cmp edx,3; jge */
      idx = (u32)(s32)(s16)bx * 0x3b;
      arr = RD32(self + 0xb1);
      addr = (u8 *)(arr + idx + 0x37);
      if (*addr == 0xff) {                           /* cmp cl,0xff */
        *addr = 1;                                   /* mov byte ptr [eax],1 */
        byte44 = RD8(self + 0x44);                   /* mov al,[esi+0x44] */
        arr = RD32(self + 0xb1);
        total = total + 1;                           /* inc edx */
        club = RD32(arr + idx);                      /* mov eax,[ecx+edi] */
        WR32(scratch + L_COUNT, total);              /* mov [esp+0x1c],edx */
        vt = RD32(self);                             /* mov edx,[esi] */
        ret40 = ((cm_vt40)(*(u32 volatile *)(vt + 0x40)))
                    ((u32)self, club, 1,
                     ((u32)addr & 0xffffff00u) | (u32)byte44, 0, 0);
        byte44 = RD8(self + 0x44);                   /* mov al,[esi+0x44] */
        arr = RD32(self + 0xb1);
        vt = RD32(self);
        club = RD32(arr + idx);
        ((cm_vt68)(*(u32 volatile *)(vt + 0x68)))
                    ((u32)self, club,
                     (ret40 & 0xffffff00u) | (u32)byte44, 2, 2, 7);
        total = (s32)RD32(scratch + L_COUNT);        /* mov edx,[esp+0x10] */
      }
      bx = (u16)(bx + 1);
      if ((s16)bx >= (s16)RD16(self + 0x3e)) break;  /* cmp bx,[esi+0x3e]; jl */
    }
  }

  other_count = RD16(list2 + 0x3e);                  /* mov dx,[ebp+0x3e] */
  total = 0;                                         /* xor ecx,ecx */
  if ((s16)other_count > 0) {
    u8 *p = (u8 *)(RD32(list2 + 0xb1) + 0x37);
    s32 n = (s32)(s16)other_count;
    do {
      if (*p == 1) {
        total = total + 1;
      }
      p = p + 0x3b;
      n = n - 1;
    } while (n != 0);
  }
  bx = 0;
  if ((s16)other_count > 0) {
    for (;;) {
      if (total >= 3) break;                         /* cmp ecx,3; jge */
      idx = (u32)(s32)(s16)bx * 0x3b;
      arr = RD32(list2 + 0xb1);
      addr = (u8 *)(arr + idx + 0x37);
      if (*addr == 0xff) {
        total = total + 1;                           /* inc ecx */
        *addr = 1;
        byte44 = RD8(self + 0x44);
        vt = RD32(self);
        WR32(scratch + L_COUNT, total);              /* mov [esp+0x10],ecx */
        arr = RD32(list2 + 0xb1);
        club = RD32(arr + idx);
        ret40 = ((cm_vt40)(*(u32 volatile *)(vt + 0x40)))
                    ((u32)self, club, 1,
                     ((u32)addr & 0xffffff00u) | (u32)byte44, 0, 0);
        byte44 = RD8(self + 0x44);
        arr = RD32(list2 + 0xb1);
        vt = RD32(self);
        club = RD32(arr + idx);
        ((cm_vt68)(*(u32 volatile *)(vt + 0x68)))
                    ((u32)self, club,
                     (ret40 & 0xffffff00u) | (u32)byte44, 2, 2, 7);
        total = (s32)RD32(scratch + L_COUNT);
      }
      bx = (u16)(bx + 1);
      if ((s16)bx >= (s16)RD16(list2 + 0x3e)) break; /* cmp bx,[ebp+0x3e]; jl */
    }
  }

  /* Even slots of the rank array from the `self` list (0x004239e0-0x00423a23). */
  di = (s16)RD16(self + 0x3e);                       /* mov di,[esi+0x3e] */
  cx_i = 0;
  dx_o = 0;
  if (di > 0) {
    for (;;) {
      if (dx_o >= 6) break;                          /* cmp dx,6; jge */
      idx = (u32)(s32)(s16)cx_i * 0x3b;
      addr = (u8 *)(RD32(self + 0xb1) + idx);
      if (*(addr + 0x37) == 1) {                     /* cmp byte ptr [eax+0x37],1 */
        club = RD32(addr);                           /* mov eax,[eax] */
        {
          s32 k = (s32)(s16)dx_o;
          dx_o = (s16)(dx_o + 2);
          WR32(scratch + L_RANK + (u32)k * 4, club);
        }
      }
      cx_i = (u16)(cx_i + 1);
      if ((s16)cx_i >= di) break;                    /* cmp cx,di; jl */
    }
  }
  /* Odd slots of the rank array from the `[[self+0xc]]` list (0x00423a25-0x423a68). */
  cx_i = 0;
  dx_o = 5;
  if ((s16)RD16(list2 + 0x3e) > 0) {                 /* cmp word [ebp+0x3e],cx; jle */
    for (;;) {
      if (dx_o <= 0) break;                          /* test dx,dx; jle */
      idx = (u32)(s32)(s16)cx_i * 0x3b;
      addr = (u8 *)(RD32(list2 + 0xb1) + idx);
      if (*(addr + 0x37) == 1) {
        club = RD32(addr);
        {
          s32 k = (s32)(s16)dx_o;
          dx_o = (s16)(dx_o - 2);
          WR32(scratch + L_RANK + (u32)k * 4, club);
        }
      }
      cx_i = (u16)(cx_i + 1);
      if ((s16)cx_i >= (s16)RD16(list2 + 0x3e)) break;
    }
  }

  /* 0x00423a6a: read the competition record field that the original stores to
     its write-only local, then dispatch virtual slot +0x3c. */
  rec = RD32((u32)RD32(0x00adadfcu) + RD32(self + 0x1c) * 4);
  arr = RD32(rec + 0xb1);
  vt3c = RD32(self);
  club = RD32(arr + 0x375);
  WR32(scratch + L_TMP, club);                       /* mov [esp+0x38],edx */
  sel = ((cm_vt3c)(*(u32 volatile *)(vt3c + 0x3c)))
            ((u32)self, 8, (u32)(scratch + L_OUT_A), (u32)(scratch + L_OUT_B), 0);

  alloc_regs = cm_fn_00944cf1(0xb2);                /* 0x00423aa2: retain EDX:EAX */
  alloc = (u32)alloc_regs;
  WR32(scratch + L_COUNT, alloc);                    /* mov [esp+0x10],eax */
  frame.state = 0;                                   /* mov [esp+0x240],ebx (ebx=0) */
  if (alloc == 0) {
    res = 0;                                         /* xor eax,eax */
  }
  else {
    out_b = RD32(scratch + L_OUT_B);                 /* 0x00423abb */
    year_bits = ((u32)(alloc_regs >> 32) & 0xffff0000u)
                | RD16(self + 0x40);                /* 0x00423abf: mov dx */
    competition = RD32(self + 4);                  /* 0x00423acb */
    out_a = RD32(scratch + L_OUT_A);                 /* 0x00423ad3 */
    res = cm_fn_00522e00(alloc, (u32)self, 7, (u32)(scratch + L_RANK),
                         out_a,
                         competition,
                         sel,
                         year_bits,
                         8, 1,
                         out_b,
                         0x14, 1, 0, 0, 0);
  }
  rec = RD32(self + 0xc);                            /* mov edx,[esi+0xc] */
  frame.state = 0xffffffffu;                         /* mov [esp+0x240],0xffffffff */
  WR32(rec + 0x20, res);                             /* mov [edx+0x20],eax */
  now = RD32((u32)RD32(self + 0xc) + 0x20);          /* mov eax,[esi+0xc]; mov ecx,[eax+0x20] */
  if (now == 0) {                                    /* cmp ecx,ebx; jne */
    cm_fn_00944cff(0x00989d04u, 0, 0,
                   (void *)(u32)(scratch + L_BUF2), (void *)(u32)(scratch + L_BUF1));
    str = cm_fn_0090d130((u32)(scratch + L_BUF2), (u32)(scratch + L_BUF1), 0x611);
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)str,
                   (u32)(scratch + L_BUF2), (u32)(scratch + L_BUF1), 0x611);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(u32, 0x00b67a34) = 0;
  }
  else {
    cm_fn_0051c800(now, 0);                          /* ECX = [eax+0x20] */
    cm_fn_009452ca(sel);                             /* push edi */
    WR32(self + 0x30, 8);                            /* mov [esi+0x30],8 */
  }
  CM_FS(void *, 0) = frame.prev;                     /* mov ecx,[esp+0x238]; mov fs:[0],ecx */
}
