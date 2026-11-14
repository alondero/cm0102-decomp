/* fn_00539490 - plain C translation of the preferred-image listing
   (VA 0x00539490, image base 0x00400000, cm0102.exe 3.9.68).

   Stack mapping (C.md rule: base L is the address right after `sub esp,0x26c`,
   a local at [esp+d] with ESP = L - <live pushed bytes> sits at L + d - <live>):

     the callee is cdecl (the caller's `add esp,0x10` removes the four pushed
     arguments), so at 005394e1 ESP = L-32 and `movsx eax, byte ptr [esp+0x22]`
     reads L+0x22-0x20 = L+2 - the marker byte, the very slot the body reaches
     as [esp+0x12] once ESP = L-16 (after `add esp,0x10`).
     `lea eax,[esp+0xe]` (ESP = L-12) = L+2  -> argument 4 (marker).
     `lea ecx,[esp+0x2c]` (ESP = L-16) = L+0x1c -> argument 3 (destination).

   One scratch allocation holds every local at its original offset:
     L+0x01 best index byte      L+0x02 marker byte      L+0x03 round byte
     L+0x04 threshold dword      L+0x08 sum dword        L+0x0c count dword
     L+0x10 pointer dword        L+0x14 saved edx dword
     L+0x1c 63 three-byte elements (word value, byte flag), L+0x55 = element 19
     L+0xdc 100 dword flags                                       (frame 0x26c)

   The 63-element pattern (word 0xff81, byte 0x81) really occupies 189 bytes
   from L+0x1c; the draft's separate `local_24e[55]` buffer ran off the end of
   the frame, which is what produced the earlier 0x0203ffff fault. */
#include "cmre.h"

extern void CM_CDECL cm_fn_0053a8b0(u8 *source, u32 unused, u8 *destination, u8 *marker);
extern void CM_CDECL cm_fn_0053a980(void *dst, u32 arg2_unused, const void *src);

/* three-byte element: word value at +0, byte flag at +2 (packed, as in the
   original's word access at a three-byte stride) */
typedef struct __attribute__((packed)) {
  s16 v;
  u8  b;
} elem_t;

typedef struct {
  u8               b000;       /* L+0x00 */
  u8               best;       /* L+0x01 */
  volatile u8      marker;     /* L+0x02 */
  u8               round;      /* L+0x03 */
  u32              threshold;  /* L+0x04 */
  u32              sum;        /* L+0x08 */
  u32              cnt;        /* L+0x0c */
  u32              ptr;        /* L+0x10 */
  u32              saved_edx;  /* L+0x14 */
  u8               pad18[4];   /* L+0x18 */
  volatile elem_t  elem[63];   /* L+0x1c .. L+0xd8 */
  u8               pad_d9[3];  /* L+0xd9 .. L+0xdb */
  u32              flags[100]; /* L+0xdc .. L+0x26b */
} frame_t;

typedef char frame_size_check[(sizeof(frame_t) == 0x26c) ? 1 : -1];

#define ELEM(i) ((volatile elem_t *)((u8 *)&f.elem[0] + 3 * (i)))

void cm_fn_00539490(int param_1)
{
  volatile frame_t f;
  u32  edx;
  u8   cl;
  u8   bl;
  s32  esi;
  s32  edi;
  s16  bp;
  s16  axv;
  s32  idx;
  volatile elem_t *ptr;

  if (param_1 == 0)                                /* 0053949e/005394a0 */
    goto done;
  edx = *(volatile u32 *)((u8 *)param_1 + 0x69);   /* 005394a6 */
  f.saved_edx = edx;                              /* 005394ab */
  if (edx == 0)                                   /* 005394a9/005394af */
    goto done;

  {                                               /* 005394b5..005394cb */
    s32 k;
    for (k = 0; k < 0x3f; k++) {
      f.elem[k].v = (s16)(u16)0xff81;
      f.elem[k].b = 0x81;
    }
  }

  cm_fn_0053a8b0((u8 *)edx, (u32)param_1, (u8 *)&f.elem[0], (u8 *)&f.marker);

  idx = (s8)f.marker;                             /* 005394e1 */
  f.round = 0;                                    /* 005394e9 */
  ptr = (volatile elem_t *)((u8 *)&f.elem[0] + 3 * idx); /* 005394ee/005394f2 */
  f.ptr = (u32)ptr;                                /* 005394f5 */

outer:                                            /* 005394f9 */
  ptr = (volatile elem_t *)f.ptr;
  esi = 0;                                        /* 005394fd */
  f.cnt = 0;                                      /* 005394ff */
  f.sum = 0;                                      /* 00539503 */
  bp = ptr->v;                                    /* 00539507 */
  f.threshold = 0x28;                             /* 0053950a */

inner_start:                                      /* 00539512 */
  cl = f.marker;                                  /* 00539512 */
  edi = 0;                                        /* 00539516 */
  bl = 0;                                         /* 00539518 */
  f.best = 0xff;                                  /* 0053951e */
  if (bp == (s16)-0x7f)                           /* 0053951a/00539523 */
    goto inner_restart;

  axv = bp;                                       /* 00539525 (mov eax, ebp) */
scan:
  {
    s32 v  = (s16)axv;                            /* 00539527 movsx esi, ax */
    s32 a  = (v < 0) ? -v : v;                    /* 0053952c..0053952f abs */
    s32 th = (s16)f.threshold;                    /* 00539533 movsx */
    if (a > th) {                                 /* 00539538/0053953a jg */
      f.flags[(s8)cl] = 0;                        /* 00539550/00539556 */
      if (cl == 0x13) {                           /* 00539553 */
        if (f.best == 0xff) {                     /* 00539563/00539567 */
          f.best = 0x13;                          /* 0053957d */
        } else {
          s16 si = f.elem[19].v;                  /* 0053956b */
          s16 bv = ELEM((s8)f.best)->v;            /* 00539573/00539577 */
          if (si > bv)                            /* 0053957b jle */
            f.best = 0x13;                        /* 0053957d */
        }
      }
    } else {
      edi += v;                                   /* 0053953f add edi, esi */
      bl++;                                       /* 00539541 inc bl */
      f.flags[(s8)cl] = 1;                        /* 00539543 */
    }
    cl++;                                         /* 00539582 */
    axv = ELEM((s8)cl)->v;                        /* 00539584..0053958b */
    if (axv != (s16)-0x7f)                        /* 0053958f/00539593 */
      goto scan;
  }
  esi = (s32)f.cnt;                               /* 00539595 */
  if ((s8)bl >= 6)                                /* 00539599/0053959c */
    goto inner_done;
inner_restart:                                    /* 0053959e */
  f.threshold = f.threshold + 0xa;                /* 005395a2/005395a8 */
  if ((s8)bl < 6)                                 /* 005395a5/005395ac */
    goto inner_start;
inner_done:                                       /* 005395b2 */
  if ((s8)f.best > (s8)0xff)                      /* 005395b6/005395b8 jle */
    f.flags[(s8)f.best] = 1;                      /* 005395ba/005395bd */
  {
    s32 q = edi / (s32)(s8)bl;                    /* 005395c8..005395d3 idiv */
    bl = f.marker;                                /* 005395d5 */
    edi = -5 - q;                                 /* 005395d9 */
  }
  if (bp == (s16)-0x7f)                           /* 005395db/005395df */
    goto adjust;

element:                                          /* 005395e5 */
  if (bl == f.best)                               /* 005395e5/005395e9 */
    goto element_next;
  {
    s32 i = (s8)bl;                               /* 005395ef */
    if (f.flags[i] == 1) {                        /* 005395f2/005395fa */
      volatile elem_t *e = ELEM(i);               /* 005395fc */
      s16 dx;
      e->v = (s16)((u16)e->v + (u16)edi);         /* 00539600 add word, di */
      dx = e->v;                                  /* 00539607 */
      if (dx < (s16)-0x7d) {                      /* 0053960b/0053960f */
        e->v = (s16)(u16)0xff83;                  /* 00539611/00539616 */
      } else if (dx > (s16)0x7d) {                /* 0053961b/0053961f */
        e->v = (s16)0x7d;                         /* 00539621/00539626 */
      } else {
        e->v = dx;                                /* 0053962b/0053962e */
      }
    } else {
      cl = f.round;                               /* 00539633 */
      if (cl == 0) {                              /* 00539637/00539639 */
        volatile elem_t *e = ELEM(i);             /* 0053963b */
        s16 v = e->v;                             /* 00539649 */
        if (v > 0) {                              /* 0053963f/00539647 */
          esi++;                                  /* 00539650 */
          f.sum = f.sum + (u32)v;                 /* 0053964c..00539653 */
        } else {
          f.flags[i] = 1;                         /* 00539659 */
        }
      } else {
        f.flags[i] = 1;                           /* 00539659 */
      }
    }
    if (*(volatile u8 *)((u8 *)param_1 + 0x3d) == 0x0a ||   /* 0053966b */
        *(volatile u8 *)((u8 *)param_1 + 0x28) == 0x0a) {   /* 00539671 */
      if (bl == 0x13) {                           /* 00539677 */
        s16 a = f.elem[19].v;                     /* 00539699 */
        f.elem[19].v = (a > 0) ? a : (s16)0;      /* 0053969e..005396aa */
      } else {
        volatile elem_t *e = ELEM(i);             /* 0053967c */
        s16 v = e->v;                             /* 00539683 */
        if (v >= (s16)-0x50)                      /* 00539686/0053968d jl */
          v = (s16)-0x50;                         /* 0053968f */
        e->v = v;                                 /* 00539694 */
      }
    }
  }
element_next:                                     /* 005396af */
  bl++;                                           /* 005396af */
  if (ELEM((s8)bl)->v != (s16)-0x7f)              /* 005396b8/005396bd */
    goto element;

adjust:                                           /* 005396c3 */
  ptr = (volatile elem_t *)f.ptr;
  bl = f.marker;                                  /* 005396c7 */
  if (ptr->v == (s16)-0x7f)                       /* 005396cb/005396cf */
    goto round_next;
  if (esi == 0)                                   /* 005396d1/005396d3 */
    goto round_next;
  if ((s32)f.sum <= 0x1c)                         /* 005396d9/005396dc */
    goto round_next;
adjust_loop:
  if (esi == 0 || (s32)f.sum <= 0x1c)             /* 00539714 -> 005396d1 */
    goto round_next;
  {
    s32 i = (s8)bl;                               /* 005396de */
    if (f.flags[i] != 1) {                        /* 005396e1/005396e9 */
      volatile elem_t *e = ELEM(i);               /* 005396eb */
      s32 q = ((s32)f.sum - 0x1c) / esi;          /* 005396f2..005396f6 */
      e->v = (s16)((u16)e->v - (u16)q);           /* 005396f8 */
      if (e->v < 0)                               /* 005396fb/005396ff */
        e->v = 0;                                 /* 00539701 */
    }
    bl++;                                         /* 00539706 */
    if (ELEM((s8)bl)->v != (s16)-0x7f)            /* 0053970f/00539714 */
      goto adjust_loop;
  }
round_next:                                       /* 00539716 */
  f.round = (u8)(f.round + 1);                    /* 0053971a/0053971e */
  if ((s8)f.round < 2)                            /* 0053971c/00539722 */
    goto outer;

  cm_fn_0053a980((void *)f.saved_edx, (u32)param_1, (const void *)&f.elem[0]);

done:
  return;
}
