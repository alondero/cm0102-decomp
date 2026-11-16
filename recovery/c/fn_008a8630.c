/* fn_008a8630 -- tactics_screens.cpp, VA 0x008a8630 (preferred image base 0x00400000,
   pinned 3.9.68 primary executable).  Reconstructed from the original instructions.

   Entry sequence 0x008a8630 `mov eax,0x3a74; call 0x00945610` is MSVC's large-frame
   stack allocation: 0x00945610 is the __chkstk-style probe (Ghidra: "unable to track
   spacebase fully for stack") that lowers ESP by the size in EAX, and the matching
   epilogue is `add esp,0x3a74; ret 4` (0x008a8eac / 0x008a8ebb).  The pair is the
   function's own 0x3a74-byte scratch frame, so the faithful C form is one
   uninitialized 0x3a74-byte scratch allocation addressed at the original frame
   offsets: every `[esp+N]` below is `L + N - saved(16) - live pushes`, with
   L = entry_esp - 0x3a74.  The probe call is not re-issued: it only performs that
   same allocation (its net ESP change is the frame itself, which the C frame
   already owns), so calling the original here would allocate a second 0x3a74
   block on top of the C frame.

   Calling convention: ECX is read before it is written -> `this` (EBP holds it and
   all object fields go through it).  `ret 4` pops one stack argument, which is also
   the receiver (ECX) of the nested calls at 0x008a8647/0x008a87cf/0x008a8886/
   0x008a8a56/0x008a8d3e/0x008a8e9e: at those sites [esp+0x3a84|0x3a94|0x3a8c]
   resolves to entry_esp+4. */

#include "cmre.h"

extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004afa40(volatile u32 *self);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector);
extern void cm_fn_005e8290(const char * param_1, const char * param_2, int param_3);
/* Listing passes three stack dwords (0x008a868f-0x008a8694) and four at the
   0x008a8899-0x008a88ac site with a single `add esp` afterwards: dest, format,
   then variadic values. */
extern char * cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern u32 CM_CDECL cm_fn_006e1cf0(u32 first, u32 second, u32 club);
extern u8 CM_STDCALL cm_fn_0085a760(int *param_1, int *param_2);
extern u32 CM_STDCALL cm_fn_0085add0(u32 param_1, u32 *param_2);
extern u32 CM_THISCALL cm_fn_008aa9d0(u8 *object, s8 arg1, u8 *arg2, u32 arg3);
extern u32 CM_THISCALL cm_fn_008aaaa0(u32 self, s8 index, u32 mode);
extern u32 CM_THISCALL cm_fn_008ab480(u32 self, u32 index, u32 alternate);
extern u32 cm_fn_0090c520(u32 param_1, int param_2, short param_3, int param_4);
/* Three stack dwords at 0x008a8c47-0x008a8c4e. */
extern char * cm_fn_0090d130(char *param_1, char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

/* 32-bit wrapping address arithmetic for frame/object offsets. */
#define FA(off)      ((u32)(unsigned long)(unsigned long)frame + (u32)(off))
#define F8(off)      (*(volatile u8  *)(unsigned long)FA(off))
#define F32(off)     (*(volatile u32 *)(unsigned long)FA(off))

#define SA(off)      ((u32)(unsigned long)(unsigned long)self + (u32)(off))
#define S8(off)      (*(volatile u8  *)(unsigned long)SA(off))
#define S32(off)     (*(volatile u32 *)(unsigned long)SA(off))
#define RECA(rec, off) ((u32)(unsigned long)(unsigned long)self + (u32)(rec) + (u32)(off))
#define REC8(rec, off) (*(volatile u8  *)(unsigned long)RECA(rec, off))
#define REC32(rec, off) (*(volatile u32 *)(unsigned long)RECA(rec, off))

/* rep movs block: copies the NUL-terminated string including its terminator. */
static void cm_copy_cstr(u32 dst, u32 src)
{
  volatile u8 *d = (volatile u8 *)(unsigned long)dst;
  volatile const u8 *s = (volatile const u8 *)(unsigned long)src;
  while ((*d++ = *s++) != 0) {
  }
}

/* Inline strcat sites scan source first, then destination (repne scasb,
   dec edi), before copying DWORDs and the remaining bytes including NUL. */
static void cm_append_cstr(u32 dst, u32 src)
{
  u32 size = 0;
  u32 i;
  while (*(volatile const u8 *)(unsigned long)(src + size++) != 0) {
  }
  while (*(volatile const u8 *)(unsigned long)dst != 0) {
    dst++;
  }
  for (i = size >> 2; i != 0; --i) {
    *(volatile u32 *)(unsigned long)dst =
        *(volatile const u32 *)(unsigned long)src;
    dst += 4;
    src += 4;
  }
  for (i = size & 3; i != 0; --i) {
    *(volatile u8 *)(unsigned long)dst =
        *(volatile const u8 *)(unsigned long)src;
    dst++;
    src++;
  }
}

u32 CM_THISCALL cm_fn_008a8630(void *selfv, u32 arg1)
{
  u8 framebuf[0x3a74];
  volatile u8 *frame = framebuf;
  u8 *self = (u8 *)selfv;
  u32 B1, B2, B3, BA, BB, BM;
  s8 cnt;
  u8 bl;

  B1 = FA(0x0c);    /* [esp+0x1c/0x20] text scratch, capacity 0xc8 */
  B2 = FA(0xfc);    /* [esp+0x10c/0x110] second text scratch, capacity 0xc8 */
  B3 = FA(0xb94);   /* [esp+0xba4/0xbb0] composed row text */
  BA = FA(0x1c4);   /* [esp+0x1d4] message argument block */
  BB = FA(0x2c4);   /* [esp+0x2d4] message argument block */
  BM = FA(0x3c4);   /* [esp+0x3d4] accumulated text */

  cm_fn_004afa40((volatile u32 *)(unsigned long)arg1);   /* 0x008a8647, ECX = entry esp+4 */

  if (S32(4) == 0) {                                     /* 0x008a864c */
    return 0;
  }

  cnt = (s8)S8(0x4a1f);                                  /* 0x008a8657 */
  F8(0x1364) = 0;                                        /* 0x008a865d */

  if (cnt <= 3) {                                        /* 0x008a8667 jle */
    F8(0x04) = 0;                                        /* 0x008a8710 */
    if (cnt > 0) {                                       /* 0x008a8715 jle */
      for (;;) {
        u8 iv = F8(0x04);
        s32 a = (s32)(s8)S8(0x4a0d + (u32)(s32)(s8)iv);  /* 0x008a8723 */
        u32 rec = 0x2735 + 446u * (u32)a;                /* 0x008a873e */
        s32 a2;
        u32 rec2;
        u8 b, b2;

        if (S32(8) == 0 && cm_fn_0085add0(S32(0), 0) != 0) {   /* 0x008a8739-0x008a8753 */
          s32 t = (s32)(s8)cm_fn_0085a760((int *)(unsigned long)REC32(rec, 0x69),
                                          (int *)(unsigned long)S32(0));
          b = (u8)(t < 0 ? -t : t);                      /* 0x008a876f cdq/xor/sub */
        }
        else {
          b = (u8)(S8(0x4a0d + (u32)(s32)(s8)iv) + 1); /* 0x008a8779 reload; 0x008a877f inc al */
        }
        F8(0x03) = b;                                    /* 0x008a8781 */

        cm_fn_0053dba0(REC32(rec, 0x69), B1, 0xc8, 1);   /* 0x008a8795 */
        if (cm_fn_0053b900((u32 *)(unsigned long)S32(0)) != 0) {   /* 0x008a879e */
          cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7dc,
                         (int)REC32(rec, 0x69));         /* 0x008a87cf */
        }
        else {
          cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7d7,
                         (int)REC32(rec, 0x69));
        }

        a2 = (s32)(s8)S8(0x4a16 + (u32)(s32)(s8)iv);     /* 0x008a87d4 */
        rec2 = 0x2735 + 446u * (u32)a2;                  /* 0x008a87ee */

        if (S32(8) == 0 && cm_fn_0085add0(S32(0), 0) != 0) {   /* 0x008a87ec-0x008a8803 */
          s32 t = (s32)(s8)cm_fn_0085a760((int *)(unsigned long)REC32(rec2, 0x69),
                                          (int *)(unsigned long)S32(0));
          b2 = (u8)(t < 0 ? -t : t);                     /* 0x008a881f */
        }
        else {
          b2 = (u8)(S8(0x4a16 + (u32)(s32)(s8)iv) + 1); /* 0x008a882b reload; 0x008a8831 inc bl */
        }

        cm_fn_0053dba0(REC32(rec2, 0x69), B2, 0xc8, 1);  /* 0x008a8846 */
        if (cm_fn_0053b900((u32 *)(unsigned long)S32(0)) != 0) {   /* 0x008a884f */
          cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B2, 0x7dc,
                         (int)REC32(rec2, 0x69));        /* 0x008a8886 */
        }
        else {
          cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B2, 0x7d7,
                         (int)REC32(rec2, 0x69));
        }

        cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a8c64c,
                       (int)(s32)(s8)F8(0x03), (char *)(unsigned long)B1,
                       (int)(s32)(s8)b2, (char *)(unsigned long)B2);   /* 0x008a88ac */

        if (F8(0x1364) != 0) {                           /* 0x008a88bd */
          cm_append_cstr(FA(0x1364), 0x009ed918);
        }
        cm_append_cstr(FA(0x1364), 0x00de1f64);          /* 0x008a88f0 */

        {
          u8 n = (u8)(F8(0x04) + 1);                     /* 0x008a8923 */
          s8 bound = (s8)S8(0x4a1f);                    /* 0x008a8927 */
          F8(0x04) = n;
          if ((s8)n < bound) {                           /* 0x008a8933 jl */
            continue;
          }
        }
        break;
      }
    }
  }
  else {
    cm_fn_0090c520((u32)(s32)cnt, (int)B1, 0xc8, 1);     /* 0x008a867d */
    cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a8c6a8,
                   (char *)(unsigned long)B1);           /* 0x008a8694 */
    if (F8(0x1364) != 0) {                               /* 0x008a86a5 */
      cm_append_cstr(FA(0x1364), 0x009ed918);
    }
    cm_append_cstr(FA(0x1364), 0x00de1f64);              /* 0x008a86d8 */
  }

  /* 0x008a8939: locate the object key inside the 20 lookup slots. */
  bl = 0;
  F8(0x08) = 0;                                          /* byte store 0x008a893b */
  for (;;) {
    u32 idx = F32(0x08);                                 /* dword reload 0x008a893f */
    u32 r = cm_fn_008ab480((u32)(unsigned long)self, idx, 0);
    if (r != 0) {
      if (*(volatile u32 *)(unsigned long)r == S32(0x4a20)) {   /* 0x008a8951 */
        F8(0x08) = bl;                                   /* 0x008a8a38 */
        goto search_done;
      }
    }
    bl = (u8)(bl + 1);                                   /* 0x008a8961 */
    F8(0x08) = bl;
    if (!((s8)bl < 0x14)) {
      break;
    }
  }
  F8(0x08) = 0xff;                                       /* 0x008a896c */
search_done:
  {
    u32 found = F32(0x08);
    u32 h = cm_fn_008aa9d0(self, (s8)found, 0, 0);       /* 0x008a897c */
    if (h == 0) {
      bl = 0xff;                                         /* 0x008a898c or bl,0xff */
      if (S32(4) != 0) {                                 /* 0x008a8991 */
        u32 best = 0;                                    /* edi */
        u8 besti = 0xff;                                 /* bl */
        u32 off = 0x2735;
        F8(0x03) = 0;
        for (;;) {
          if ((s8)S8(off + 0x19) >= 0) {                 /* 0x008a89a7 jl */
            u32 v = cm_fn_006e1cf0(0, (u32)(unsigned long)SA(off), S32(0));  /* 0x008a89b2 */
            if (besti == 0xff || (s16)v > (s16)best) {   /* 0x008a89ba-0x008a89c2 */
              besti = F8(0x03);
              best = v;
            }
          }
          {
            u8 n = (u8)(F8(0x03) + 1);                   /* 0x008a89d4 */
            off += 0x1be;                                /* 0x008a89ce */
            F8(0x03) = n;
            if (!((s8)n < 0x14)) {                       /* 0x008a89dc */
              break;
            }
          }
        }
        if (!((s8)besti < 0)) {                          /* 0x008a89e1 jle */
          s32 a3 = (s32)(s8)besti;
          u32 rec3 = 0x2735 + 446u * (u32)a3;            /* 0x008a89ff */
          cm_fn_0053dba0(REC32(rec3, 0x69), B1, 0xc8, 1);      /* 0x008a8a13 */
          if (cm_fn_0053b900((u32 *)(unsigned long)S32(0)) != 0) {   /* 0x008a8a1c */
            cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7dc,
                           (int)REC32(rec3, 0x69));      /* 0x008a8a56 */
          }
          else {
            cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7d7,
                           (int)REC32(rec3, 0x69));
          }
          cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a16fec,
                         (char *)(unsigned long)B1);     /* 0x008a8a6a */
          if (F8(0x1364) != 0) {                         /* 0x008a8a7b */
            cm_append_cstr(FA(0x1364), 0x009ed918);
          }
          cm_append_cstr(FA(0x1364), 0x00de1f64);        /* 0x008a8aae */
        }
      }
    }
  }

  /* 0x008a8adf */
  bl = 0;
  F8(0x03) = 0;
  F8(0x04) = 0;
  for (;;) {
    s8 i = (s8)F8(0x04);                                 /* 0x008a8aeb */
    if (i >= 0 && i <= 0x13) {                           /* 0x008a8af5 jg */
      /* 0x008a8afb-0x008a8b13: [ebp+0xddf] is only read when [ebp+0xde1] != 0 */
      if (S32(0xde1) == 0 ||
          i < (s8)(u8)((S8(0xddf) & 0x0f) + 0x0b)) {
        u32 idx = F32(0x04);                             /* dword reload 0x008a8b19 */
        u32 v;
        if (cm_fn_008aa9d0(self, (s8)idx, 0, 0) == 0 &&          /* 0x008a8b24 */
            cm_fn_008aaaa0((u32)(unsigned long)self, (s8)idx, 0) == 0 &&   /* 0x008a8b31 */
            (v = cm_fn_008ab480((u32)(unsigned long)self, idx, 0)) != 0) { /* 0x008a8b3e */
          volatile u32 *p = (volatile u32 *)(unsigned long)SA(0x4a2c);     /* 0x008a8b47 */
          u32 outer = 5;                                 /* 0x008a8b4d */
          do {
            u32 k = 2;                                   /* 0x008a8b57 */
            do {
              if (*p == v) {                             /* 0x008a8b5c */
                u8 j = 0;
                if ((s8)bl > 0) {                        /* 0x008a8b62 */
                  do {
                    if (F32(0xd4 + 4 * (u32)(s32)(s8)j) == v) {   /* 0x008a8b69 */
                      break;
                    }
                    j = (u8)(j + 1);
                  } while ((s8)j < (s8)bl);              /* 0x008a8b76 */
                }
                if (j == bl) {                           /* 0x008a8b7a */
                  F32(0xd4 + 4 * (u32)(s32)(s8)bl) = v;  /* 0x008a8b81 */
                  bl = (u8)(bl + 1);
                }
              }
              p = p + 1;                                 /* 0x008a8b88 */
              k = k - 1;
            } while (k != 0);
            outer = outer - 1;                           /* 0x008a8b90 */
          } while (outer != 0);
        }
      }
    }
    else {
      /* 0x008a8c18 message path */
      u32 msg;
      cm_fn_00944cff(0x00a8b94c, 0, 0, (void *)(unsigned long)BB,
                     (void *)(unsigned long)BA);
      msg = (u32)(unsigned long)cm_fn_0090d130((char *)(unsigned long)BB,
                                               (char *)(unsigned long)BA, 0x1b8e);
      /* 0x008a8c47-0x008a8c5e: the earlier three pushes remain
         below msg/format/destination until add esp,0x24 at 0x008a8c74. */
      cm_fn_00944c9f(0x00ae24d0, 0x009870e8, msg, BB, BA, 0x1b8e);
      cm_fn_005e8290((const char *)(unsigned long)0x009870e0,
                     (const char *)(unsigned long)0x00ae24d0, 0);
      DAT_00b67a34 = 0;                                  /* 0x008a8c77 */
    }
    {
      u8 n = (u8)(F8(0x04) + 1);                         /* 0x008a8b9f */
      F8(0x04) = n;
      if (!((s8)n < 0x14)) {                             /* 0x008a8ba7 */
        break;
      }
    }
  }

  F8(0x03) = bl;                                         /* 0x008a8baf */
  if ((s8)bl > 0) {                                      /* 0x008a8bb3 jle */
    F8(0x04) = 0;                                        /* 0x008a8bb9 */
    for (;;) {
      s32 idx = (s32)(s8)F8(0x04);                       /* 0x008a8bbe movsx */
      u32 person = F32(0xd4 + 4 * (u32)idx);             /* 0x008a8bc9 */
      cm_fn_0053dba0(person, B1, 0xc8, 1);               /* 0x008a8bd7 */
      if (F8(0x04) != 0) {                               /* 0x008a8be3 */
        u8 c = F8(0x03);                                 /* 0x008a8c86 */
        s32 last = (s32)(s8)c - 1;
        u32 fmt;
        if (idx == last) {
          fmt = (c == 2) ? 0x00a17b40 : 0x00a17b18;      /* 0x008a8ca4 / 0x008a8cbc */
        }
        else {
          fmt = 0x00a17af4;                              /* 0x008a8cd0 */
        }
        cm_fn_0066f4e0((int)B3, (char *)(unsigned long)fmt,
                       (char *)(unsigned long)BM, (char *)(unsigned long)B1);  /* 0x008a8cdd */
        cm_copy_cstr(FA(0x3c4), FA(0xb94));              /* 0x008a8ce5 */
      }
      else {
        cm_copy_cstr(FA(0xb94), FA(0x0c));               /* 0x008a8bfb */
        cm_copy_cstr(FA(0x3c4), FA(0xb94));              /* 0x008a8ce5 */
      }
      if (cm_fn_0053b900((u32 *)(unsigned long)S32(0)) != 0) {   /* 0x008a8d14 */
        cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7dc,
                       (int)person);                     /* 0x008a8d3e */
      }
      else {
        cm_fn_004ae8a0((int)arg1, (u8 *)(unsigned long)B1, 0x7d7, (int)person);
      }
      {
        u8 n = (u8)(F8(0x04) + 1);                       /* 0x008a8d4b */
        u8 c2 = F8(0x03);
        F8(0x04) = n;
        if (!((s8)n < (s8)c2)) {                         /* 0x008a8d53 */
          break;
        }
      }
    }
    bl = F8(0x03);                                       /* 0x008a8d59 mov bl,cl */
    if ((s8)bl > 1) {                                    /* 0x008a8d5e jle */
      cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a8c5ec,
                     (char *)(unsigned long)BM);         /* 0x008a8d81 */
    }
    else {
      cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a8c594,
                     (char *)(unsigned long)BM);
    }
    if (F8(0x1364) != 0) {                               /* 0x008a8d92 */
      cm_append_cstr(FA(0x1364), 0x009ed918);
    }
    cm_append_cstr(FA(0x1364), 0x00de1f64);              /* 0x008a8dc5 */
  }

  /* 0x008a8df6 */
  if (F8(0x1364) == 0) {
    return 0;
  }
  if (S32(0x0c) == 0) {                                  /* 0x008a8e05 */
    return 0;
  }
  cm_fn_0066f4e0(0x00de1f64, (char *)(unsigned long)0x00a8c57c);   /* 0x008a8e1a */
  if (F8(0x1364) != 0) {                                 /* 0x008a8e2b */
    cm_append_cstr(FA(0x1364), 0x009ed918);
  }
  cm_append_cstr(FA(0x1364), 0x00de1f64);                /* 0x008a8e5e */
  cm_fn_004ae660((void *)(unsigned long)arg1, FA(0x1364));   /* 0x008a8e9e */
  return 1;                                              /* 0x008a8ea6 */
}
