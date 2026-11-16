/* fn_008be300 -- VA 0x008be300, image base 0x00400000, 1741 bytes, `ret 8`
   (thiscall: ECX receiver + 2 stack arguments), transfer_manager.cpp.

   Translated from the original instruction listing only. Every callee stays an
   explicit boundary; no engine logic, default or cleanup is invented.

    Instruction evidence below uses preferred-image VAs in CM0102 3.9.68,
    SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.

    UNRESOLVED acceptance blocker (fatal path, 0x008be3b0..0x008be446):
   the realloc-failure block ends with `push -1; call 0x00945934` and the
   original bytes then fall through to 0x008be44b (`mov edi,[ebx+4]` /
   `rep stosd`) *without* an `add esp,4`, so on that path the original's
   esp-relative homes are shifted by four bytes and its epilogue
   (`mov ecx,[esp+0x234]` / `add esp,0x230` / `ret 8`) would tear down a
   stack it no longer owns. That shifted transport is not expressible in
   plain C and no original evidence shows 0x00945934 returning. This source
   therefore (a) keeps the call as the explicit boundary the listing has,
   (b) keeps the fall-through statement stream exactly as listed instead of
   inventing a termination, a return or a cleanup, and (c) does not invent
   synthetic shifted homes: the statements after the call run against this
    frame's own homes. This is NOT equivalent if that boundary returns:
    EDX is also call-clobbered, and every subsequent ESP-relative access,
    saved-register pop and the return transport differs. Passing ordinary
    fixtures cannot establish the missing nonreturn contract. The other gaps
   (callee ABI / partial-DWORD argument widths at 0x008be8b6 and
   0x008be8b6/0x008be8ba) are taken at the widths the callee declarations
   evidence: the two pushed dwords are read in full but only their low
   byte is parameter-typed as s8/u8, which is all the original caller can
   guarantee.
*/
#include "cmre.h"

/* ---- original globals (preferred-image VAs) ---- */
#define G_AE23B4 CM_GLOBAL(u32, 0x00ae23b4) /* record index base */
#define G_AE23BC CM_GLOBAL(u32, 0x00ae23bc) /* record index base 2 */
#define G_AE1050 CM_GLOBAL(u32, 0x00ae1050) /* receiver of 0x004cf5a0 */
#define G_AE1080 CM_GLOBAL(u32, 0x00ae1080) /* cached lookup result */
#define G_B67A34 CM_GLOBAL(u32, 0x00b67a34)
#define G_B74448 CM_GLOBAL(u32, 0x00b74448) /* receiver of 0x007f75f0 */

/* ---- original data addresses used as values ---- */
#define A_00a8d514 ((u32)0x00a8d514) /* source path string */
#define A_00a8e940 ((u32)0x00a8e940) /* "TRANSFER_MANAGER::add_transfer_o..." */
#define A_009870e0 ((u32)0x009870e0) /* "Error" */
#define A_009870e8 ((u32)0x009870e8) /* format string */
#define A_009a7ff8 ((u32)0x009a7ff8) /* 0x009a7ff6 + 2 text */
#define A_009a8108 ((u32)0x009a8108) /* "Memory Allocation Error" */
#define A_009a8120 ((u32)0x009a8120) /* format string */
#define A_00ae24d0 ((u32)0x00ae24d0) /* message buffer */
#define A_00ae26c4 ((u32)0x00ae26c4) /* message buffer */
#define A_00ae28bc ((u32)0x00ae28bc) /* message buffer */

/* ---- explicit callee boundaries ---- */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00600a50(u32 club, u32 skip_status, u8 flag, u32 person, u32 force);
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in,
                                                    const u8 *src, s16 count);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern void CM_THISCALL cm_fn_0068d2f0(u32 self, u32 source);
extern u32 CM_THISCALL cm_fn_007f75f0(u32 self, u32 record, u32 owner);
extern u32 CM_THISCALL cm_fn_008c6220(u32 self, volatile u32 record_argument,
                                      volatile s8 day_argument,
                                      volatile u8 tick_argument);
extern void CM_THISCALL cm_fn_008cc5b0(u32 self, u32 a1, u8 *out_a, u8 *out_b);
extern u32 CM_THISCALL cm_fn_008d0f60(u32 self, u32 record);
extern void CM_THISCALL cm_fn_008d3090(u32 self, u32 offer);
extern void CM_THISCALL cm_fn_008e1780(u32 self, u32 offer, u32 related);
extern u32 CM_THISCALL cm_fn_008e5680(u32 self);
/* At 008be3e3 and 008be514 the existing pushes survive to 00944c9f.
   They are its trailing arguments, not arguments to this boundary. */
extern char *cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);
extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
extern void cm_fn_00945934(u32 param_1); /* fatal, see header comment */

typedef struct cm_seh_frame {
  void *prev;
  void *handler;
  u32 state;
} cm_seh_frame;

void CM_THISCALL cm_fn_008be300(u8 *self, u8 *rec, u32 arg2)
{
  /* SEH frame: `push -1` (state), `push 0x963a6a`, `push fs:[0]`, `fs:[0]=esp` */
  volatile cm_seh_frame seh;
  /* L = esp right after `sub esp,0x224` (0x008be315) */
  u32 scratch[0x89];
  u8 *const L = (u8 *)scratch;
  u32 *const l_230 = (u32 *)(L + 0x00); /* [esp+0x10] slot index        */
  u32 *const l_22c = (u32 *)(L + 0x04); /* [esp+0x14] second index     */
  u8 *const l_228 = L + 0x08;           /* [esp+0x18] byte home        */
  u32 *const l_224 = (u32 *)(L + 0x0c); /* [esp+0x1c] allocation temp  */
  u8 *const l_220 = L + 0x10;           /* [esp+0x20] byte home        */
  u32 *const l_21c = (u32 *)(L + 0x14); /* [esp+0x24] occupied count   */
  u32 *const l_218 = (u32 *)(L + 0x18); /* [esp+0x28] rec->f8 value    */
  u32 *const l_214 = (u32 *)(L + 0x1c); /* [esp+0x2c] table flag       */
  u8 *const bufA = L + 0x24;            /* [esp+0x34] 256-byte buffer  */
  u8 *const bufB = L + 0x124;           /* [esp+0x134] 256-byte buffer */
  u32 tab, esi, edi, recptr;
  u32 fc, f8v;

  seh.state = 0xffffffffu;              /* 0x008be300 push -1 */
  seh.handler = (void *)0x00963a6a;     /* 0x008be302 push 0x963a6a */
  seh.prev = CM_FS(void *, 0);          /* 0x008be307/30d */
  CM_FS(void *, 0) = (void *)(u32)&seh; /* 0x008be30e */

  /* 0x008be31d: ebp = [esp+0x23c] = first stack argument (rec) */

  fc = *(u32 *)(rec + 0x0c);           /* 0x008be328 */
  edi = *(u32 *)(self + 0x85e);        /* 0x008be32b */
  f8v = *(u32 *)(rec + 0x08);          /* 0x008be331 */
  *l_22c = 0xffffffffu;                /* 0x008be337 */
  *l_224 = fc;                         /* 0x008be33b */
  tab = *(u32 *)(self + 0x86e);        /* 0x008be33f */
  *(u32 *)(self + 0x85e) = 0xffffffffu;/* 0x008be345 */
  fc = *(u32 *)(rec + 0x0c);           /* 0x008be34b reload */
  *l_230 = edi;                        /* 0x008be34e */
  *l_218 = f8v;                        /* 0x008be352 */
  esi = 0;                             /* 0x008be356 */
  {
    u8 dl = *(u8 *)(tab + fc * 8 + 1); /* 0x008be358 */
    *l_214 = esi;                      /* 0x008be35c */
    if (dl == 0) *l_214 = 1;           /* 0x008be360..0x008be36a */
  }
  if (*(u32 *)(rec + 0x1b) == 0) /* 0x008be36c */
    *(u8 *)(rec + 0x40) = 0;     /* 0x008be371 */

  /* ---- grow the slot array (0x008be375..0x008be46d) ---- */
  if (*(u32 *)(self + 0x04) == *(u32 *)(self + 0x08)) {
    u32 base = *(u32 *)(self + 0x00);      /* 0x008be383 */
    u32 nc = *(u32 *)(self + 0x04) + 0x32; /* 0x008be385 */
    *(u32 *)(self + 0x04) = nc;            /* 0x008be38a */
    if (base == 0) {
      /* 0x008be460: fresh block */
      u32 nb = (u32)cm_fn_00945501(nc, 4);
      *(u32 *)(self + 0x00) = nb; /* 0x008be46b */
    } else {
      u32 nb = (u32)cm_fn_00945667(base, nc * 4); /* 0x008be393..0x008be3a1 */
      *(u32 *)(self + 0x00) = nb;                 /* 0x008be3a8 */
      if (nb == 0) {
        /* 0x008be3b0..0x008be446 (see header comment for the fall-through) */
        cm_fn_00944cff(A_00a8d514, 0, 0, bufB, bufA); /* 0x008be3bb..3c4 */
        {
          /* 0x008be3d7..3e3: four live trailing arguments for 00944c9f. */
          u32 msg = (u32)cm_fn_0090d130();
          /* 0x008be3e8..3f3: the 0x0090d130 pushes are still live here, so the
             variadic 0x00944c9f argument slots are (dest, fmt, msg, bufB,
             bufA, 0x4d2, source-string) -- one `add esp,0x1c` covers all seven. */
          cm_fn_00944c9f(A_00ae24d0, A_009a8120, msg, (u32)bufB, (u32)bufA,
                         0x4d2, A_00a8e940);
        }
        cm_fn_0066f4e0((int)A_00ae28bc, (char *)A_009a8108);      /* 3fb..405 */
        cm_fn_0066f4e0((int)A_00ae26c4, (char *)A_009a7ff8, A_00ae24d0);
        cm_fn_005e8290((const char *)A_00ae28bc, (const char *)A_00ae26c4, 0);
        G_B67A34 = 0;      /* 0x008be434 */
        cm_fn_005cc7d0();  /* 0x008be43a */
        cm_fn_005a8f60();  /* 0x008be43f */
        cm_fn_00945934(0xffffffffu); /* 0x008be444/446 */
      }
      /* 0x008be44b: zero the 0x32 freshly added dwords */
      edi = *(u32 *)(self + 0x04);
      {
        u32 *p = (u32 *)(nb + edi * 4 - 0xc8); /* lea edi,[edx+edi*4-0xc8] */
        s32 k = 0x32;
        while (k != 0) {
          *p = 0;
          p = p + 1;
          k = k + -1;
        }
      }
    }
    edi = *l_230; /* 0x008be46d */
  }

  /* ---- scan the slot array (0x008be471..0x008be4e1) ---- */
  {
    u32 n = *(u32 *)(self + 0x04); /* 0x008be471 */
    u32 *arr = (u32 *)*(u32 *)(self + 0x00);
    u32 i = 0; /* 0x008be476 */
    *l_21c = esi; /* 0x008be47a, esi == 0 */
    if ((s32)n > 0) { /* 0x008be47e */
      do {
        u32 entry = arr[i]; /* 0x008be484 */
        if (entry == 0) {
          if (edi == 0xffffffffu) { /* 0x008be4ba */
            edi = i;
            *l_230 = edi;
          }
          if (*l_21c == *(u32 *)(self + 0x08)) break; /* 0x008be4c5..cc */
          if (*l_214 != 0) break;                     /* 0x008be4ce..d2 */
        } else {
          *l_21c = *l_21c + 1; /* 0x008be48a..8f */
          if (*(u32 *)(entry + 0x0c) == *l_224) { /* 0x008be493..9a */
            if (*(u32 *)(entry + 0x08) == *l_218) {
              edi = i;
              *l_230 = edi;
            } else if (*l_22c == 0xffffffffu) {
              *l_22c = i; /* 0x008be4ad..b8 */
            }
          }
        }
        i = i + 1;                      /* 0x008be4d7 */
        n = *(u32 *)(self + 0x04);      /* 0x008be4d4 reload */
      } while ((s32)i < (s32)n);        /* 0x008be4db..dd */
    }
  }

  if (edi == 0xffffffffu) {
    /* 0x008be4e6: no free/usable slot -> report and return */
    cm_fn_00944cff(A_00a8d514, 0, 0, bufA, bufB); /* 0x008be4f1..4fa */
    {
      /* 0x008be50d..514: three trailing 00944c9f arguments stay live. */
      u32 msg = (u32)cm_fn_0090d130();
      /* 0x008be519..524: (dest, fmt, msg, bufA, bufB, 0x511) -- one
         `add esp,0x24` covers the three calls' nine pushes. */
      cm_fn_00944c9f(A_00ae24d0, A_009870e8, msg, (u32)bufA, (u32)bufB, 0x511);
    }
    cm_fn_005e8290((const char *)A_009870e0, (const char *)A_00ae24d0, 0);
    G_B67A34 = 0; /* 0x008be53c */
    goto epilogue; /* 0x008be542 */
  }

  /* 0x008be547: release the record occupying the chosen slot */
  {
    u32 old = ((u32 *)*(u32 *)(self + 0x00))[edi];
    if (old != 0) cm_fn_008d0f60((u32)self, old); /* 0x008be550..53 */
  }

  /* 0x008be558: allocate the 0x41-byte record */
  {
    u32 a = (u32)cm_fn_00944cf1(0x41);
    u32 newrec;
    *l_224 = a;   /* 0x008be562 */
    seh.state = 0; /* 0x008be568 */
    if (a == 0) {
      newrec = 0;
    } else {
      newrec = cm_fn_008e5680(a); /* 0x008be571..78 */
    }
    seh.state = 0xffffffffu; /* 0x008be582 */
    ((u32 *)*(u32 *)(self + 0x00))[*l_230] = newrec; /* 0x008be58d */
  }

  /* 0x008be590: count++, esi = the new record */
  {
    u32 cnt = *(u32 *)(self + 0x08);
    u32 *arr = (u32 *)*(u32 *)(self + 0x00);
    *(u32 *)(self + 0x08) = cnt + 1; /* 0x008be599..9a */
    esi = arr[*l_230];               /* 0x008be59d */
  }

  /* 0x008be5a0..0x008be660: copy the incoming record into it (esi = dest) */
  *(u32 *)(esi + 0x00) = *(u32 *)(rec + 0x00);
  *(u32 *)(esi + 0x04) = *(u32 *)(rec + 0x04);
  *(u32 *)(esi + 0x08) = *(u32 *)(rec + 0x08);
  *(u32 *)(esi + 0x0c) = *(u32 *)(rec + 0x0c);
  *(u8 *)(esi + 0x10) = *(u8 *)(rec + 0x10);
  {
    u8 t11 = *(u8 *)(rec + 0x11); /* 0x008be5bd */
    /* 0x008be5c7/5c8: thiscall receiver = esi+0x12, args (scratch, rec+0x12) */
    *(u8 *)(esi + 0x11) = t11;    /* 0x008be5cc before the call */
    cm_fn_00417c10((u32 *)(esi + 0x12), (u32 *)(L + 0x1c),
                   (u16 *)(rec + 0x12)); /* 0x008be5cf */
  }
  *(u8 *)(esi + 0x1a) = *(u8 *)(rec + 0x1a);
  *(u32 *)(esi + 0x1b) = *(u32 *)(rec + 0x1b);
  *(u8 *)(esi + 0x1f) = *(u8 *)(rec + 0x1f);
  *(u32 *)(esi + 0x20) = *(u32 *)(rec + 0x20);
  *(u32 *)(esi + 0x24) = *(u32 *)(rec + 0x24);
  *(u32 *)(esi + 0x28) = *(u32 *)(rec + 0x28);
  *(u8 *)(esi + 0x2c) = *(u8 *)(rec + 0x2c);
  *(u8 *)(esi + 0x2d) = *(u8 *)(rec + 0x2d);
  *(u8 *)(esi + 0x2e) = *(u8 *)(rec + 0x2e);
  *(u8 *)(esi + 0x2f) = *(u8 *)(rec + 0x2f);
  {
    /* 0x008be610: low nibble of rec->0x30 into dest->0x30 low nibble */
    u8 cl = *(u8 *)(rec + 0x30);                   /* 0x008be610 */
    u8 al = *(u8 *)(esi + 0x30);                   /* 0x008be613 */
    cl = (u8)(cl << 4);                           /* 0x008be616 */
    s32 sx = (s32)(s8)cl >> 4;                     /* 0x008be619 sar cl,4 */
    u8 r = (u8)(((u8)(sx ^ al)) & 0xf) ^ al;       /* 0x008be61c..21 */
    u32 old31 = *(u32 *)(esi + 0x31);              /* 0x008be623 */
    *(u8 *)(esi + 0x30) = r;                       /* 0x008be626 */
    {
      u32 d = ((*(u32 *)(rec + 0x31) ^ old31) & 0xf) ^ old31; /* 0x008be629..34 */
      *(u32 *)(esi + 0x31) = d;                             /* 0x008be639 */
    }
  }
  *(u32 *)(esi + 0x35) = *(u32 *)(rec + 0x35); /* 0x008be63c..3e */
  *(u8 *)(esi + 0x39) = *(u8 *)(rec + 0x39);   /* 0x008be643..46 */
  *(u32 *)(esi + 0x3a) = *(u32 *)(rec + 0x3a); /* 0x008be64c..4f */
  *(u8 *)(esi + 0x3e) = *(u8 *)(rec + 0x3e);   /* 0x008be651..54 */
  *(u8 *)(esi + 0x3f) = *(u8 *)(rec + 0x3f);   /* 0x008be657 */
  *(u8 *)(esi + 0x40) = *(u8 *)(rec + 0x40);   /* 0x008be65d */

  {
    u32 ix = *l_230; /* 0x008be665 */
    *(u32 *)(((u32 *)*(u32 *)(self + 0x00))[ix] + 0x20) = ix; /* 0x008be669..6c */
    *(u32 *)(rec + 0x20) = ix;                                /* 0x008be673 */
  }
  edi = 0xffffffffu; /* 0x008be678 */
  {
    u32 ix = *l_230;
    u32 nr = ((u32 *)*(u32 *)(self + 0x00))[ix];
    *(u32 *)(nr + 0x28) = edi;                                /* 0x008be67e */
    *(u8 *)(((u32 *)*(u32 *)(self + 0x00))[ix] + 0x2c) = 0;   /* 0x008be68a */
    *(u8 *)(((u32 *)*(u32 *)(self + 0x00))[ix] + 0x10) = 0;   /* 0x008be697 */
    nr = ((u32 *)*(u32 *)(self + 0x00))[ix];
    if (*(u8 *)(nr + 0x2d) == 2) *(u8 *)(nr + 0x2d) = 1; /* 0x008be6a4..aa */
    nr = ((u32 *)*(u32 *)(self + 0x00))[ix];                  /* 0x008be6ae */
    *(u32 *)(nr + 0x24) = *(u32 *)(self + 0x85a);             /* 0x008be6b7..bd */
    *(u32 *)(rec + 0x24) = *(u32 *)(nr + 0x24);               /* 0x008be6c9..cc */
    *(u32 *)(self + 0x85a) = *(u32 *)(self + 0x85a) + 1;      /* 0x008be6cf..d6 */
  }

  /* 0x008be6dc: optional 0xe-byte companion object */
  if (*(u32 *)(rec + 0x04) != 0) {
    u32 a = (u32)cm_fn_00944cf1(0xe);
    u32 o;
    *l_224 = a;    /* 0x008be6ed */
    seh.state = 1; /* 0x008be6f3 */
    if (a == 0) {
      o = 0;
    } else {
      o = cm_fn_0068d020(a); /* 0x008be700..707 */
    }
    seh.state = edi; /* 0x008be711 */
    {
      u32 nr = ((u32 *)*(u32 *)(self + 0x00))[*l_230];
      *(u32 *)(nr + 0x04) = o; /* 0x008be71b */
      cm_fn_0068d2f0(*(u32 *)(nr + 0x04), *(u32 *)(rec + 0x04)); /* 0x008be72e */
    }
  }

  /* 0x008be733: link with an already existing equivalent slot */
  if (*l_22c != edi) {
    u32 slot = ((u32 *)*(u32 *)(self + 0x00))[*l_22c]; /* 0x008be73f..41 */
    u32 f0 = *(u32 *)slot;                             /* 0x008be744 */
    if (f0 != 0) {
      /* 0x008be74a..751: ECX = slot->f0, EDX = slot */
      cm_fn_0068d0d0((volatile u8 *)f0, (u32)slot, (const u8 *)(L + 0x00), 4);
      /* 0x008be756..768: newrec->f0 = slot->f0 */
      {
        u32 base = *(u32 *)(self + 0x00);
        u32 other = *(u32 *)(base + *l_22c * 4);
        u32 dest = *(u32 *)(base + *l_230 * 4);
        u32 value = *(u32 *)other;
        *(u32 *)dest = value;
      }
    } else {
      u32 a = (u32)cm_fn_00944cf1(0xe);
      u32 o;
      *l_224 = a;    /* 0x008be776 */
      seh.state = 2; /* 0x008be77c */
      if (a == 0) {
        o = 0;
      } else {
        o = cm_fn_0068d020(a); /* 0x008be789..790 */
      }
      seh.state = edi; /* 0x008be79f */
      /* 0x008be796..7a6: ECX = o, EDX = &l_22c, src = &l_22c, count 4 */
      cm_fn_0068d0d0((volatile u8 *)o, (u32)(L + 0x04), (const u8 *)(L + 0x04), 4);
      /* 0x008be7ab..7b4: ECX = o, EDX = &l_22c, src = &l_230, count 4 */
      cm_fn_0068d0d0((volatile u8 *)o, (u32)(L + 0x04), (const u8 *)(L + 0x00), 4);
      *(u32 *)((u32 *)*(u32 *)(self + 0x00))[*l_22c] = o; /* 0x008be7b9..c2 */
      *(u32 *)((u32 *)*(u32 *)(self + 0x00))[*l_230] = o; /* 0x008be7c4..cd */
    }
  }

  /* 0x008be7cf: look up the club/person record and cache it */
  {
    u32 fcx = *(u32 *)(rec + 0x0c);
    u32 t = fcx + fcx * 4;        /* lea ecx,[eax+eax*4] */
    u32 t2 = fcx + t * 2;         /* 11 * fcx */
    u32 t3 = t2 + t2 * 4;         /* 55 * fcx */
    u32 base = G_AE23BC;
    u32 r = cm_fn_004cf5a0(G_AE1050, base + t3 * 2); /* base + 0x6e*fcx */
    esi = G_AE23B4;      /* 0x008be7ef */
    G_AE1080 = r;        /* 0x008be7f5 */
    if ((r == 0) || ((*(u8 *)(r + 0x35) & 0x3f) == 5)) {
      recptr = 0; /* 0x008be81c */
    } else {
      u32 k = *(u32 *)(r + 0x04);
      u32 t4 = (k + k * 8) << 4; /* 144 * k */
      t4 = t4 + k;               /* 145 * k */
      recptr = (esi + k) + t4 * 4; /* G_AE23B4 + 0x245 * k */
    }
  }

  /* 0x008be81e: decide the day/tick bytes */
  *l_220 = 0; /* 0x008be821 */
  if (*(u8 *)(rec + 0x2f) != 0) {
    u32 p = *l_218;                        /* 0x008be82a */
    u32 q = (esi + p) + (((p + p * 8) << 4) + p) * 4; /* G_AE23B4 + 0x245*p */
    u32 r = cm_fn_00600a50(q, 0, 1, 0, 0); /* 0x008be82e..44 */
    u32 ix = *l_230;                       /* 0x008be849 */
    *(u8 *)(((u32 *)*(u32 *)(self + 0x00))[ix] + 0x2c) = 7; /* 0x008be857 */
    if (r != 0) {
      *l_228 = 0x1c; /* 0x008be85d */
    } else {
      *l_228 = 0;    /* 0x008be864 */
      goto L_008be8ae;
    }
    goto L_008be8b6;
  } else {
    if (recptr != 0) { /* 0x008be86b */
      if (cm_fn_00600a50(recptr, 0, 1, 0, 0) != 0) { /* 0x008be86f..80 */
        *l_228 = 0x1c;   /* 0x008be884 */
        goto L_008be8b6;
      }
    }
    /* 0x008be88b: ask the manager for the day/tick bytes */
    cm_fn_008cc5b0((u32)self, ((u32 *)*(u32 *)(self + 0x00))[*l_230],
                   (u8 *)(L + 0x08), (u8 *)(L + 0x10));
    if (*(u8 *)(L + 0x08) != 0) goto L_008be8b6; /* 0x008be8a6..ac */
  }
L_008be8ae:
  if (*l_220 == 0) goto L_008be8d1; /* 0x008be8ae..b4 */
L_008be8b6:
  /* 0x008be8b6: the pushed dwords are only parameter-typed as s8/u8 */
  cm_fn_008c6220((u32)self, ((u32 *)*(u32 *)(self + 0x00))[*l_230],
                 (s8) * (u8 *)(L + 0x08), *(u8 *)(L + 0x10));
L_008be8d1:
  {
    u32 nr = ((u32 *)*(u32 *)(self + 0x00))[*l_230]; /* 0x008be8d1..d7 */
    if (*(u8 *)(nr + 0x1f) == 1) {
      cm_fn_008e1780((u32)self, nr, arg2); /* 0x008be8e0..eb */
    } else {
      cm_fn_008d3090((u32)self, nr); /* 0x008be8f2..f5 */
    }
  }

  /* 0x008be8fa: touch the per-record table entry */
  {
    u32 fcx = *(u32 *)(rec + 0x0c);
    u8 *e = (u8 *)(*(u32 *)(self + 0x86e) + fcx * 8);
    *(u8 *)e = (u8)(*(u8 *)e | 1);
  }
  {
    u32 fcx = *(u32 *)(rec + 0x0c);
    u8 *e = (u8 *)(*(u32 *)(self + 0x86e) + fcx * 8 + 1);
    *(u8 *)e = (u8)(*(u8 *)e + 1);
  }

  /* 0x008be923: walk the companion list and bump each entry's counter */
  if (*(u32 *)(rec + 0x04) != 0) {
    u32 f4 = *(u32 *)(rec + 0x04);
    u32 v = *(u32 *)(f4 + 2);   /* 0x008be92a */
    *(u32 *)(f4 + 0x0a) = v;    /* 0x008be92d */
    {
      u32 node = cm_fn_0068d1b0(f4); /* 0x008be933 */
      while (node != 0) {
        u8 *e = (u8 *)(*(u32 *)(self + 0x86e) + *(u32 *)node * 8 + 2);
        *(u8 *)e = (u8)(*(u8 *)e + 1); /* 0x008be948..4c */
        node = cm_fn_0068d1b0(*(u32 *)(rec + 0x04)); /* 0x008be951 */
      }
    }
  }

  /* 0x008be95a: notify the person record when it exists */
  {
    u32 p = *(u32 *)(rec + 0x08);
    u32 q = (G_AE23B4 + p) + (((p + p * 8) << 4) + p) * 4;
    if (cm_fn_00600a50(q, 0, 1, 0, 0) != 0) { /* 0x008be965..83 */
      u32 fcx = *(u32 *)(rec + 0x0c);
      u32 t = fcx + fcx * 4;
      u32 t2 = fcx + t * 2;
      u32 t3 = t2 + t2 * 4;
      u32 rp = G_AE23BC + t3 * 2;
      if (*(u32 *)(rp + 0x61) != 0) { /* 0x008be99c..a2 */
        cm_fn_007f75f0(G_B74448, rp, 0); /* 0x008be9a4..ad */
      }
    }
  }

epilogue:
  /* 0x008be9b2: mov ecx,[esp+0x234]; pop edi/esi/ebp/ebx; fs:[0]=ecx */
  CM_FS(void *, 0) = seh.prev;
  return;
}
