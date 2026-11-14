/* fn_004cd230: translation of the routine at preferred-image VA 0x004cd230
   (cm0102.exe 3.9.68, image base 0x00400000).

   ABI from the listing: thiscall, ECX = receiver, two stack arguments
   (home +4 and home +8), `ret 8`.  EAX at return = receiver->f1c + 1. */

#include "cmre.h"

/* ---- original globals ---- */
#define G_HANDLER CM_GLOBAL(u32, 0x00956d50)
#define G_MGR     CM_GLOBAL(u32, 0x00de1f54)
#define G_LIMIT   CM_GLOBAL(int, 0x00ae2364)
#define G_TABLE   CM_GLOBAL(int, 0x00ae23bc)
#define G_BASE    CM_GLOBAL(u8,  0x00ae2c88)
#define G_DLO     CM_GLOBAL(u32, 0x00ae2c90)
#define G_DHI     CM_GLOBAL(u32, 0x00ae2c94)
#define G_PEOPLE  CM_GLOBAL(int, 0x00ae2d0c)

/* ---- original callees, explicit boundaries ---- */
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 *param_1, short *param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_0054c8e0(u32 *param_1, short *param_2, u32 param_3);
extern int  CM_THISCALL cm_fn_0054cdb0(u32 *param_1, u32 param_2, u32 param_3);
extern u32  CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u32 param_3);
extern u32  CM_THISCALL cm_fn_008c61e0(void *param_1, u32 param_2);
extern void CM_THISCALL cm_fn_008cc5b0(int param_1, int param_2, char *param_3, char *param_4);
extern u32  CM_THISCALL cm_fn_004cd6d0(u32 param_1, volatile u32 param_2, volatile u32 param_3, volatile u8 param_4, volatile u8 param_5);
/* Native boundary at 0x0090cfc0: cdecl, one 32-bit stack argument (the bound)
   read from the callee's entry ESP+4, 32-bit result in EAX (see
   recovery/c/contracts/fn_0090cfc0.json).  The accepted regparm(2) C
   definition in recovery/c/fn_0090cfc0.c is a different physical transport
   and is not linked here: the harness binds this symbol to the original VA,
   where EAX/EDX at entry are not inputs (EAX is written at 0x0090cfd5 and
   EDX is killed by the CDQ at 0x0090d01f).  The original pushes exactly one
   argument at each of the three call sites (0x004cd30e, 0x004cd47d,
   0x004cd69f) and removes it with `add esp, 4`. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

/* ---- receiver object: dword fields at +0x00 .. +0x1c ---- */
typedef struct cm_obj_s {
  u32 f00;
  u32 f04;
  u32 f08;
  u32 f0c;   /* base of the 14-byte record array */
  u32 f10;
  u32 f14;   /* record count */
  u32 f18;   /* current index */
  u32 f1c;   /* counter, incremented at the end */
} cm_obj_s;

/* ---- one 14-byte record ---- */
typedef struct cm_rec_s {
  u32 id;    /* +0x00 */
  u16 w4;    /* +0x04 */
  u16 w6;    /* +0x06 */
  u32 f8;    /* +0x08 */
  u8  c0c;   /* +0x0c */
  u8  c0d;   /* +0x0d */
} cm_rec_s;

/* ---- SEH frame (ADR 0003) ---- */
typedef struct cm_seh_s {
  void *prev;
  void *handler;
  volatile u32 state;
} cm_seh_s;

/* ---- scratch frame: the original stack slots, in original order ---- */
typedef struct cm_scratch_s {
  u32 slot34;      /* +0x00 : receiver copy, later {G_DLO,G_DHI} pair */
  u32 slot30;      /* +0x04 : high dword of that pair */
  u16 bufA0;       /* +0x08 */
  u16 bufA1;       /* +0x0a */
  u32 pad0c;       /* +0x0c */
  u32 bufB;        /* +0x10 */
  u32 pad14;       /* +0x14 */
  u32 bufC;        /* +0x18 */
  u32 pad18;       /* +0x1c */
  u32 bufE;        /* +0x20 */
  u32 bufF;        /* +0x24 : the original never writes this slot (S+0x34) */
  cm_seh_s seh;    /* +0x28 */
} cm_scratch_s;

u32 CM_THISCALL cm_fn_004cd230(u32 *param_1, u32 param_2, u32 param_3)
{
  volatile cm_obj_s *this = (volatile cm_obj_s *)param_1;
  cm_scratch_s sc;
  u32 homes[8];                /* argument homes: [0] = arg1, [1] = arg2 */
  u32 esi, edi, ebp_v, cnt, last, base, ptr;
  u32 eax_v = 0;
  u32 rec_ptr;                /* EDI: the record the tail works on */
  volatile cm_rec_s *rec;

  homes[0] = param_2;
  homes[1] = param_3;

  /* 0x004cd230..0x004cd23e : SEH frame */
  sc.seh.state   = 0xffffffffu;
  sc.seh.handler = (void *)0x00956d50u;   /* 0x004cd232 `push 0x956d50`: the
                                             immediate handler VA, not a load */
  sc.seh.prev    = CM_FS(void *, 0);
  CM_FS(void *, 0) = &sc.seh;

  /* 0x004cd24c..0x004cd26f */
  sc.slot34 = (u32)param_1;
  esi = this->f18;
  eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufA0, 0xe);
  edi = this->f14;
  ebp_v = param_2;
  sc.seh.state = 0;

  /* 0x004cd279..0x004cd2ce : first scan */
  if ((int)esi < (int)edi) {
    base = this->f0c;
    eax_v = esi * 8u;
    eax_v = eax_v - esi;
    ptr = base + eax_v * 2u;
    eax_v = ptr + 6u;
    for (;;) {
      rec = (volatile cm_rec_s *)ptr;
      if (sc.bufA1 < rec->w6) {            /* 0x004cd296 (unsigned) */
        break;
      }
      if (sc.bufA1 == rec->w6) {
        /* 0x004cd29a..0x004cd2aa: the signed low-word test.  It runs only on
           equality; the unsigned `ja` at 0x004cd298 jumps straight to the
           id-check block at 0x004cd2b1. */
        if ((s16)sc.bufA0 < (s16)rec->w4) {   /* 0x004cd2aa (signed) */
          break;
        }
      }
      /* 0x004cd2b1: reached when bufA1 > rec->w6 (ja) and when the equal
         low-word test passed; the id check is not nested under equality. */
      if (rec->c0d == 3 && rec->id == ebp_v) {   /* 0x004cd2b1, 0x004cd2b7 */
        rec->id = 0xffffffffu;                  /* 0x004cd2c8 */
        break;
      }
      esi++;                               /* 0x004cd2bb */
      ptr += 14u;                          /* 0x004cd2bc */
      eax_v = ptr + 6u;                    /* 0x004cd2bf */
      if ((int)esi >= (int)edi) {          /* 0x004cd2c4 */
        break;
      }
    }
  }

  /* 0x004cd2d2..0x004cd2e4 */
  cnt = this->f14;
  last = this->f1c;
  eax_v = cnt - last;                     /* 0x004cd2d8, 0x004cd2da */
  if ((int)esi < (int)eax_v) {            /* 0x004cd2de */
    esi = eax_v;                          /* 0x004cd2e0 */
  }

  /* 0x004cd2e6..0x004cd30c : second scan */
  rec_ptr = 0;
  if ((int)esi < (int)cnt) {
    base = this->f0c;
    eax_v = esi * 8u;
    eax_v = eax_v - esi;
    ptr = base + eax_v * 2u;
    for (;;) {
      rec = (volatile cm_rec_s *)ptr;
      if (rec->c0d == 3 && rec->id == ebp_v) {   /* 0x004cd2f5, 0x004cd2fb */
        if (esi != 0xffffffffu) {                /* 0x004cd309 */
          rec_ptr = ptr;
        }
        break;
      }
      esi++;                                     /* 0x004cd2ff */
      ptr += 14u;                                /* 0x004cd300 */
      if ((int)esi < (int)cnt) {                 /* 0x004cd303 */
        continue;
      }
      break;                                     /* 0x004cd307 */
    }
  }

  if (rec_ptr == 0) {
    /* 0x004cd30e..0x004cd367 : not found */
    eax_v = cm_fn_0090cfc0(5);               /* 0x004cd30e, 0x004cd310 */
    eax_v = eax_v + 1u;                     /* 0x004cd318 */
    eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufB, eax_v);
    {
      u32 person = (u32)G_TABLE + ebp_v * 110u;   /* 0x004cd329..0x004cd348 */
      sc.seh.state = (sc.seh.state & 0xffffff00u) | 1u;   /* 0x004cd340 */
      eax_v = cm_fn_004cd6d0((u32)param_1, person, (u32)(unsigned long)&sc.bufB, 2, 3);
    }
    cnt = this->f14;                        /* 0x004cd353 */
    base = this->f0c;                       /* 0x004cd356 */
    sc.seh.state = sc.seh.state & 0xffffff00u;      /* 0x004cd359 */
    rec_ptr = base + cnt * 14u - 14u;       /* 0x004cd367 */
  }

  if (param_3 != 0xffffffffu) {
    /* 0x004cd378..0x004cd4a2 : the caller-supplied person path */
    u32 person_ptr = cm_fn_008c61e0((void *)G_MGR, param_3);    /* 0x004cd37f */
    cm_fn_008cc5b0((int)G_MGR, (int)(*(u32 *)person_ptr),
                   (char *)&homes[0], (char *)&homes[1]);        /* 0x004cd39a */
    {
      u8 al = (u8)homes[0];                 /* 0x004cd39f */
      u8 cl = (u8)homes[1];                 /* 0x004cd3a7 */
      u8 bl = (u8)((u8)(al * 3u) + cl);     /* 0x004cd3a5, 0x004cd3bb */
      u8 aval;
      u8 cl2;
      s32 ebx;
      rec = (volatile cm_rec_s *)rec_ptr;
      eax_v = (u32)cm_fn_0054cdb0((u32 *)(rec_ptr + 4u), G_DLO, G_DHI);   /* 0x004cd3c1 */
      cl2 = (u8)(rec->c0c - G_BASE);        /* 0x004cd3c6, 0x004cd3cf */
      aval = (u8)eax_v;
      if ((s8)cl2 < 0) {                    /* 0x004cd3d1 */
        cl2 = (u8)(cl2 + 3u);               /* 0x004cd3d3 */
        aval = (u8)(aval - 1u);             /* 0x004cd3d6 */
      }
      aval = (u8)(aval * 3u);                /* 0x004cd3da */
      aval = (u8)(aval + cl2);               /* 0x004cd3dc */
      if ((s8)aval >= (s8)bl) {             /* 0x004cd3e0 */
        aval = ((s8)bl < 1) ? 1u : bl;      /* 0x004cd3ea..0x004cd3f1 */
      } else {
        if ((s8)aval < 1) {                 /* 0x004cd3e2 */
          aval = 1u;                        /* 0x004cd3e6 */
        }
      }
      ebx = (s8)aval;                       /* 0x004cd3f3 */
      {
        u32 count = (u32)((s32)ebx / 3);    /* 0x004cd3f6..0x004cd406 */
        u8 rem = (u8)((u8)((s32)ebx % 3) + G_BASE);   /* 0x004cd42b */
        eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufC, count);  /* 0x004cd40f */
        eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufE, eax_v); /* 0x004cd41c */
        rec->c0c = rem;                     /* 0x004cd434 */
        if ((s8)rem > 2) {                  /* 0x004cd437 */
          u8 rem2 = (u8)(rem - 3u);         /* 0x004cd439 */
          rec->c0c = rem2;                  /* 0x004cd43e */
          eax_v = cm_fn_0054c5b0((u32 *)(rec_ptr + 4u), (short *)&sc.bufE, 1);  /* 0x004cd448 */
          eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v); /* 0x004cd455 */
        }
      }
      {
        u32 pobj = *(u32 *)person_ptr;      /* 0x004cd45a */
        if (*(volatile u8 *)(pobj + 0x2fu) != 0) {   /* 0x004cd45d */
          eax_v = (u32)cm_fn_0054cdb0((u32 *)(rec_ptr + 4u), G_DLO, G_DHI);  /* 0x004cd473 */
          if ((s32)eax_v < 4) {             /* 0x004cd478 */
            eax_v = cm_fn_0090cfc0(2) + 4u;  /* 0x004cd47d, 0x004cd487 */
            eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufE, eax_v); /* 0x004cd495 */
            eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v); /* 0x004cd4a2 */
          }
        }
      }
    }
  } else {
    /* 0x004cd524..0x004cd5bb : the -1 path */
    sc.slot34 = G_DLO;                             /* 0x004cd52f */
    sc.slot30 = G_DHI;                             /* 0x004cd53f */
    rec = (volatile cm_rec_s *)rec_ptr;
    {
      u16 cx = (u16)(sc.slot34 >> 16);             /* 0x004cd533 */
      u16 ax = (u16)sc.slot34;                     /* low word of G_DLO */
      u16 w6 = rec->w6;                            /* 0x004cd538 */
      u16 w4 = rec->w4;                            /* 0x004cd545 */
      int go = 0;
      if (cx == w6 && ax == w4) {                  /* 0x004cd543, 0x004cd548 */
        go = 1;
      } else if (cx > w6) {                        /* 0x004cd564 */
        go = 1;
      } else if (cx == w6 && (s16)ax > (s16)w4) { /* 0x004cd56d */
        go = 1;
      }
      if (go) {
        /* 0x004cd574..0x004cd5bb */
        u8 c0c;
        eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufE, 0x00ae2c90u); /* 0x004cd580 */
        c0c = rec->c0c;                            /* 0x004cd585 */
        if (!((s8)c0c > (s32)G_BASE)) {            /* 0x004cd593, 0x004cd595 */
          u8 a2 = (u8)(c0c + 1u);                  /* 0x004cd597 */
          rec->c0c = a2;                           /* 0x004cd59b */
          if ((s8)a2 > 2) {                        /* 0x004cd59e */
            u8 a3 = (u8)(a2 - 3u);                 /* 0x004cd5a0 */
            rec->c0c = a3;                         /* 0x004cd5a4 */
            /* 0x004cd5a7 `lea eax, [esp + 0x34]` at ESP = S-4 resolves to
               S+0x30, the same slot 0x004cd441 uses for its 0x54c5b0 call. */
            eax_v = cm_fn_0054c5b0((u32 *)(rec_ptr + 4u), (short *)&sc.bufE, 1);      /* 0x004cd5ae */
            eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v);   /* 0x004cd5bb */
          }
        }
      }
    }
    /* 0x004cd5c0..0x004cd6c9 : person table walk */
    {
      u32 tbl = (u32)G_TABLE + ebp_v * 110u;       /* 0x004cd5c0..0x004cd5d2 */
      if (tbl != 0) {                              /* 0x004cd5d6 */
        u32 pi = *(u32 *)tbl;                      /* 0x004cd5dc */
        if ((s32)pi < G_LIMIT) {                  /* 0x004cd5e6 */
          u32 pbase = (u32)G_PEOPLE;               /* 0x004cd5ec */
          u32 pv = *(u32 *)(pbase + pi * 79u);     /* 0x004cd5fa */
          if ((s32)pv >= 0) {                     /* 0x004cd5ff */
            u32 person_ptr = this->f00 + pv * 80u;  /* 0x004cd605..0x004cd60d */
            homes[0] = person_ptr;                 /* 0x004cd60f */
            if (person_ptr != 0) {                 /* 0x004cd613 */
              u32 p2 = 0;
              int p2_ok = 0;                       /* ECX = 0 on every failure path */
              u32 tbl2;
              eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufE, 0xb4); /* 0x004cd628 */
              tbl2 = (u32)G_TABLE + ebp_v * 110u;  /* 0x004cd62d, 0x004cd633 */
              if (tbl2 != 0) {                     /* 0x004cd638 */
                u32 pi2 = *(u32 *)tbl2;            /* 0x004cd63a */
                if ((s32)pi2 < G_LIMIT) {          /* 0x004cd644 */
                  u32 pv2 = *(u32 *)((u32)G_PEOPLE + pi2 * 79u);  /* 0x004cd652, 0x004cd658 */
                  if ((s32)pv2 >= 0) {             /* 0x004cd65d */
                    p2 = pv2;                      /* 0x004cd663 */
                    p2_ok = 1;
                  }
                }
              }
              {
                /* 0x004cd663..0x004cd66b computes the second person pointer
                   only on the success path; every failure path leaves ECX = 0,
                   so 0x004cd67f / 0x004cd690 read the absolute addresses
                   0x2f and 0x2d instead of this->f00 + 0x2f / 0x2d. */
                u32 person2 = p2_ok ? (this->f00 + p2 * 80u) : 0u;   /* 0x004cd663..0x004cd66b */
                u16 di;
                u16 axw;
                u16 dxw;
                /* 0x004cd66d: EAX here is the value the 0x54c5b0 call left
                   (a pointer to its written date); the slot takes [eax]. */
                sc.slot34 = *(u32 *)eax_v;                  /* 0x004cd66d, 0x004cd672 */
                sc.slot30 = *(u32 *)(eax_v + 4u);           /* 0x004cd66f, 0x004cd67b */
                di = (u16)(sc.slot34 >> 16);             /* 0x004cd676 */
                axw = *(volatile u16 *)(person2 + 0x2fu); /* 0x004cd67f */
                dxw = (u16)sc.slot34;
                if (di > axw) {                          /* 0x004cd686 */
                  /* 0x004cd67f leaves EAX = (slot30 & 0xffff0000) | axw. */
                  eax_v = (sc.slot30 & 0xffff0000u) | axw;
                  eax_v = cm_fn_0090cfc0(4) + 4u;          /* 0x004cd69f, 0x004cd6a9 */
                  eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufE, eax_v); /* 0x004cd6b7 */
                  eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v); /* 0x004cd6c4 */
                } else if (di == axw && (s16)dxw > (s16)(*(volatile u16 *)(person2 + 0x2du))) {
                  /* 0x004cd68e clears EAX and the SETG at 0x004cd694 leaves
                     AL = 1, so EAX = 1 at the call. */
                  eax_v = 1u;
                  eax_v = cm_fn_0090cfc0(4) + 4u;          /* 0x004cd69f, 0x004cd6a9 */
                  eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufE, eax_v); /* 0x004cd6b7 */
                  eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v); /* 0x004cd6c4 */
                }
              }
            }
          }
        }
      }
    }
  }

  /* 0x004cd4ab..0x004cd503 : the common tail */
  {
    u32 dlo = G_DLO;
    u32 dhi = G_DHI;
    u16 di = (u16)dlo;
    u16 dx = (u16)(dlo >> 16);
    u32 date_ptr;
    int again;
    sc.slot34 = dlo;                                   /* 0x004cd4bf */
    sc.slot30 = dhi;                                   /* 0x004cd4c3 */
    date_ptr = cm_fn_0054c8e0((u32 *)(rec_ptr + 4u), (short *)&sc.bufE, 0xe);  /* 0x004cd4c7 */
    again = 0;
    if (dx < *(volatile u16 *)(date_ptr + 2u)) {       /* 0x004cd4d8 */
      again = 1;
    } else if (dx == *(volatile u16 *)(date_ptr + 2u)) {
      if ((s16)di < (s16)(*(volatile u16 *)date_ptr)) {  /* 0x004cd4e1 */
        again = 1;
      }
    }
    if (again) {                                        /* 0x004cd4ea */
      eax_v = cm_fn_0054c5b0((u32 *)0x00ae2c90u, (short *)&sc.bufE, 0xe);     /* 0x004cd4f6 */
      eax_v = cm_fn_00417c10((u32 *)(rec_ptr + 4u), (u32 *)&sc.bufC, eax_v); /* 0x004cd503 */
    }
  }

  /* 0x004cd508..0x004cd521 */
  {
    u32 next = this->f1c + 1u;                         /* 0x004cd508, 0x004cd50f */
    this->f1c = next;                                  /* 0x004cd512 */
    CM_FS(void *, 0) = sc.seh.prev;                    /* 0x004cd517 */
    return next;
  }
}
