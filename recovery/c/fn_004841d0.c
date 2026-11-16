/* fn_004841d0 — club_screens.cpp, 1965 bytes, preferred image base 0x00400000.
   Reconstructed from the original instructions only. Callees are the brief's
   recovered C boundaries; the caller's pushes match those arguments
   (right-to-left) and callee stack cleanup matches the declared convention.

   Three callees expose register inputs at their boundary (EAX/EDX for
   cm_fn_005b7d20 and cm_fn_00945333, EDX for cm_fn_0068d0d0).  The listing
   never sets those registers for the calls, so the live values are carried
   forward explicitly here exactly as the caller leaves them. */
/* Pinned SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86:
   preferred-image VAs 0048426b-00484277 push three DWORD arguments and
   caller cleans 0xc for 0053d980; 0048427e-0048428b push five DWORD
   arguments and caller cleans 0x14 for 0053c190. Both declarations below
   therefore use cdecl DWORD parameters, resolving the earlier build concern. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0045cbe0(u32 player, u32 object, u32 third, u32 fourth);
extern u8 CM_THISCALL cm_fn_004cc130(u32 self, u32 person, u32 club);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_0053c190(u32 club, u32 person, u32 date, u32 detach, u32 players_only);
extern u32 CM_CDECL cm_fn_0053d980(u32 arg_1, u32 arg_2, u32 arg_3);
extern u32 CM_CDECL cm_fn_00540a50(u32 club, u32 direction, u32 require_flag);
extern u32 CM_CDECL cm_fn_005b7d20(const u8 *person);
extern void cm_fn_005e8290(const char * param_1,const char * param_2,int param_3);
extern u32 CM_CDECL cm_fn_00600c90(u32 club, u32 skip_status, u8 flag);
extern u32 CM_THISCALL cm_fn_0068d080(u32 self);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count);
extern u32 CM_STDCALL cm_fn_00775790(u32 input);
extern u8 CM_THISCALL cm_fn_008972c0(u32 self, u32 club, u32 person, u32 flag);
extern char * cm_fn_0090d130(void);
extern void CM_THISCALL cm_fn_0090d4f0(u32 self, u32 club);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00945333(u32 base_arg, u32 count_arg, u32 width_arg, u32 compare_arg);

/* Original globals (preferred-image VAs).  The ones the routine re-reads
   between calls stay volatile so every original reload is preserved. */
#define G_AD9DB0   CM_GLOBAL(int * volatile, 0x00ad9db0)
#define G_AD9DB4   CM_GLOBAL(u8, 0x00ad9db4)
#define G_AD9DB5   CM_GLOBAL(u8, 0x00ad9db5)
#define G_ADAB33   CM_GLOBAL(u8, 0x00adab33)
#define G_ADAB34   CM_GLOBAL(int, 0x00adab34)
#define G_ADAB38   CM_GLOBAL(int *, 0x00adab38)
#define G_ADAB3C   CM_GLOBAL(int * volatile, 0x00adab3c)
#define G_ADAB44   CM_GLOBAL(u32, 0x00adab44)
#define G_ADAB4A   CM_GLOBAL(u8, 0x00adab4a)
#define G_ADADF0   CM_GLOBAL(u32, 0x00adadf0)
#define G_AE2364   CM_GLOBAL(volatile int, 0x00ae2364)
#define G_AE24D0   CM_GLOBAL(u8, 0x00ae24d0)
#define G_B67A34   CM_GLOBAL(u32, 0x00b67a34)
#define G_B77C7A   CM_GLOBAL(volatile s16, 0x00b77c7a)
/* Receiver sources loaded into ECX before each call; reloaded every time. */
#define G_AE1050   CM_GLOBAL(volatile u32, 0x00ae1050)
#define G_DE1D08   CM_GLOBAL(volatile u32, 0x00de1d08)
#define G_DE2F70   CM_GLOBAL(volatile u32, 0x00de2f70)

/* Scratch area holding the original frame's contiguous locals:
   L+0x18 -> out (the second out-parameter of cm_fn_00540a50),
   L+0x1c -> the 50 collected pointers, L+0xe4 and L+0x1e4 -> the two 256-byte
   buffers filled by cm_fn_00944cff.  One allocation because the original
   addresses are adjacent. */
typedef struct {
  s32 out;
  int *items[50];
  u8 name[256];
  u8 ext[256];
} cm_4841d0_frame;

u32
cm_fn_004841d0(int *param_1, int param_2, u32 param_3, u8 param_4, u8 param_5,
               u8 param_6, u8 param_7, u32 param_8, int *param_9, int *param_10,
               u32 param_11, int param_12, int *param_13, int param_14)
{
  cm_4841d0_frame frame;
  s8 count;         /* L+0x0b  number of collected pointers */
  int *slot;        /* L+0x0c  cursor over param_13 */
  int rounds;       /* L+0x10  remaining iterations (0x32) */
  int first;        /* L+0x14  first cm_fn_00540a50 result */
  u32 table;        /* selected byte table address */
  u32 grp;          /* which of the three shared call tails is taken */
  u32 reg_eax;      /* live EAX carried into the dispatch tail */
  u32 reg_edx;      /* live EDX carried into the write loops */
  unsigned long long r64;
  int gate;
  u32 bits;
  int keep;
  int *person;
  u32 field;
  u8 r8;
  u32 r32;
  s32 i;

  /* 004841d7/004841e7: both the club and the stream object are required. */
  if (param_1 == (int *)0 || param_9 == (int *)0) {
    /* 00484909 */
    cm_fn_00944cff(0x0098e598u, 0u, 0u, (void *)frame.ext, (void *)frame.name);
    /* 0048422a->0048492a: cm_fn_0090d130 takes no arguments in its recovered
       signature, so the three earlier pushes belong to cm_fn_00944c9f. */
    cm_fn_00944c9f((u32)&G_AE24D0, 0x009870e8u, (u32)cm_fn_0090d130(),
                   (u32)(void *)frame.ext, (u32)(void *)frame.name, 0x503fu);
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)&G_AE24D0, 0);
    G_B67A34 = 0;
    return 0;
  }

  /* 004841f6 */
  first = (int)cm_fn_00540a50((u32)param_1, 0u, 1u);
  /* 00484203: ECX = stream object */
  cm_fn_0068d080((u32)param_9);
  /* 0048420e: ECX = [0x00de2f70] */
  cm_fn_0090d4f0(G_DE2F70, (u32)param_1);

  count = 0;
  /* 0048421a: param_13 defaults to club+0xd7 */
  if (param_13 == (int *)0) {
    param_13 = (int *)((u32)param_1 + 0xd7u);
  }
  slot = param_13;
  rounds = 0x32;
  do {
    person = (int *)*slot;                              /* 00484249 */
    if (person != (int *)0) {
      field = *(volatile u32 *)((u32)person + 0x61);     /* 00484253 */
      if (field != 0u) {
        /* 0048425e */
        if (cm_fn_0053b900((u32 *)param_1) != 0u) {
          if (cm_fn_0053d980((u32)person,
                             *(volatile u32 *)((u32)param_1 + 0x53), 1u) != 0u) {
            cm_fn_0053c190((u32)param_1, (u32)person, 0u, 1u, 0u);
          }
        }
        /* 0048428e: the gate re-reads cm_fn_0053b900; whichever branch
           reaches 0x004842c0 leaves its own value in EAX (0x004842ab-0x004842b4). */
        gate = 0;
        {
          u32 r53b = cm_fn_0053b900((u32 *)param_1);
          if (r53b != 0u) {
            gate = 1;                           /* 00484297 jne -> 004842c0 */
          }
          else {
            u32 r600 = cm_fn_00600c90((u32)param_1, 0u, 1);
            if (r600 != 0u) {
              gate = 1;                         /* 004842a9 jne -> 004842c0 */
            }
            else {
              /* 004842ab: EDX = *person, EAX = [0x00ae2364] — the only branch
                 that pins the registers cm_fn_005b7d20 reads. */
              u32 dv = *(volatile u32 *)person;
              s32 dcnt = G_AE2364;
              if ((s32)dv < dcnt) {
                gate = 1;                       /* 004842b4 jl -> 004842c0 */
              }
              else if ((*(volatile u8 *)((u32)person + 0x5e) & 2u) == 0u) {
                gate = 1;                       /* falls through to 004842c0 */
              }
            }
          }
        }
        if (gate != 0) {
          /* 004842c0: the (mask&1)&&(mask&0x20) shortcut skips the call and
             all of its bit tests. */
          if ((param_8 & 1u) == 0u || (param_8 & 0x20u) == 0u) {
            /* The recovered boundary reads EAX/EDX at entry; forward them where
               the listing pins them (004842ab-004842b4) and leave the branches
               that only carry another call's leftover undefined. */
            bits = cm_fn_005b7d20((const u8 *)field);
            if ((param_8 & 2u) != 0u && (bits & 1u) == 0u) goto next;
            if ((param_8 & 4u) != 0u && (bits & 6u) == 0u) goto next;
            if ((param_8 & 8u) != 0u && (bits & 0x38u) == 0u) goto next;
            if ((param_8 & 0x10u) != 0u && (bits & 0x40u) == 0u) goto next;
            if ((param_8 & 0x40u) != 0u && (bits & 0x80u) == 0u) goto next;
            if ((param_8 & 0x80u) != 0u && (bits & 0x200u) == 0u) goto next;
            if ((param_8 & 0x100u) != 0u && (bits & 0x800u) == 0u) goto next;
          }
          /* 0048433d */
          if ((param_8 & 0x400u) == 0u) {
            keep = 0;                                   /* 0048437e */
            if (cm_fn_00600c90((u32)param_1, 0u, 1) != 0u) {
              /* 00484357 */
              r8 = cm_fn_008972c0(G_DE1D08, (u32)param_1, (u32)person,
                                  (u32)param_2);
              /* 0048436c: xor ecx,ecx / cmp al,0xff / setg cl */
              keep = (r8 < 0x80u) ? 1 : 0;
            }
            else if (param_10 != (int *)0) {
              /* 00484384: secondary scan over param_10's records */
              i = 0;
              do {
                if (keep != 0) break;
                {
                  u32 sel = (*(volatile s32 *)param_1 !=
                             *(volatile s32 *)param_10) ? 1u : 0u;
                  s32 v = *(volatile s32 *)((u32)param_10 + sel * 1573u +
                                            (u32)i * 73u + 400u);
                  if (v == *(volatile s32 *)person) keep = 1;
                }
                i = i + 1;                              /* 004843d6 */
              } while ((s8)i < 0x14);
            }
            /* 004843dd */
            if (cm_fn_0053b900((u32 *)param_1) != 0u) {
              u8 f5e = *(volatile u8 *)((u32)person + 0x5e);
              if (param_2 == 0) {
                if ((f5e & 8u) == 0u) goto after_3dd;
              }
              else {
                if ((f5e & 4u) == 0u) goto after_3dd;
              }
              if (keep == 0) goto next;                 /* 00484402 */
            }
          after_3dd:
            /* 0048440a */
            if ((param_8 & 0x200u) != 0u) {
              if (cm_fn_0053b900((u32 *)param_1) == 0u) {
                if (cm_fn_00600c90((u32)param_1, 0u, 1) != 0u) {
                  r8 = cm_fn_004cc130(G_AE1050, (u32)person, (u32)param_1);
                  if (param_2 == 0) {
                    if (r8 == 2u) { if (keep == 0) goto next; }
                  }
                  else {
                    if (r8 == 1u) { if (keep == 0) goto next; }
                  }
                }
              }
            }
          }
          /* 0048445b */
          if ((param_8 & 0x800u) == 0u) {
            r32 = cm_fn_0045cbe0((u32)person, param_11, (u32)param_1,
                                 0x00ae2c90u);
            if ((param_8 & 0x1000u) != 0u && r32 == 0u) goto next;
            if ((param_8 & 0x2000u) != 0u && r32 != 0u) goto next;
          }
          /* 00484491 */
          if ((param_8 & 0x4000u) == 0u && (param_8 & 0x8000u) != 0u) {
            if (*(volatile u8 *)((u32)person + 0x6d) != 2u &&
                *(volatile s8 *)((u32)person + 0x18) > 0x13) {
              if (*(volatile s32 *)((u32)param_1 + 0x1db) != -1) goto next;
              if (first != 0) {
                if (*(volatile s32 *)((u32)first + 0x1db) != -1) goto next;
              }
            }
          }
          /* 004844c9 */
          if ((param_8 & 0x10000u) == 0u && (param_8 & 0x20000u) != 0u) {
            if (*(volatile s32 *)person >= G_AE2364) goto next;
          }
          if ((param_8 & 0x40000u) == 0u && (param_8 & 0x80000u) != 0u) {
            if (*(volatile u8 *)((u32)person + 0x6d) == 8u) goto next;
          }
          if ((param_8 & 0x100000u) == 0u && (param_8 & 0x200000u) != 0u) {
            if (*(volatile u32 *)((u32)person + 0x39) != (u32)param_1) {
              r32 = cm_fn_00540a50((u32)param_1, 0u, 1u);
              if (*(volatile u32 *)((u32)person + 0x39) != r32) goto next;
            }
            if (cm_fn_004cf5a0(G_AE1050, (u32)person) == 0u) goto next;
            r32 = cm_fn_004cf5a0(G_AE1050, (u32)person);
            if ((*(volatile u8 *)(r32 + 0x35) & 0x3fu) == 5u) goto next;
          }
          /* 00484549 */
          i = (s32)count;
          count = (s8)(count + 1);
          frame.items[i] = person;
        }
      }
    }
next:                                                   /* 0048455a */
    slot = slot + 1;
    rounds = rounds + -1;
  } while (rounds != 0);

  /* 00484574 */
  if (count == 0) {
    return 1;
  }

  G_AD9DB0 = param_1;
  /* 00484580: cm_fn_00540a50(club, &frame.out, 1) */
  r32 = cm_fn_00540a50((u32)param_1, (u32)(void *)&frame.out, 1u);
  G_ADAB3C = (int *)r32;
  if (frame.out != 0 || r32 == 0u) {
    G_ADAB3C = G_AD9DB0;
  }
  G_ADAB4A = param_5;
  G_ADAB33 = param_4;
  G_ADAB38 = param_10;
  G_AD9DB4 = param_7;
  G_ADAB34 = param_2;
  G_AD9DB5 = param_6;
  /* 004845e4: movsx eax, word[0xb77c7a]; lea edx,[eax+eax*2]; shl edx,8 */
  {
    s32 idx = (s32)G_B77C7A * 0x300;
    r32 = *(volatile u32 *)(0x00b74c26u + (u32)idx);
    reg_edx = (u32)idx;                 /* EDX left by 0x00484608 */
  }
  reg_eax = r32;                        /* EAX left by 0x00484618 */
  G_ADAB44 = param_11;
  G_ADADF0 = r32;                                       /* 00484627 */

  /* 00484624: label dispatch; every branch pushes its byte table address and
     the three shared tails differ only in which registers carry the live
     count and base: A -> EAX=base,EDX=count; B -> EDX=base; C -> EAX=count. */
  grp = 0;
  if ((param_3 & 1u) != 0u) {
    reg_eax = (u32)param_14;                            /* 0048462e */
    if (param_14 != 0) {
      table = 0x00485630u; grp = 2;                     /* 00484639 */
    }
    else {
      table = 0x00485470u; grp = 3;                     /* 00484643 */
    }
  }
  else if ((param_3 & 2u) != 0u) {
    table = 0x004856a0u; grp = 1;
  }
  else if ((param_3 & 4u) != 0u) {
    table = 0x004857a0u; grp = 2;
  }
  else if ((param_3 & 8u) != 0u) {
    table = 0x00485830u; grp = 3;
  }
  else if ((param_3 & 0x10u) != 0u) {
    /* 00484681: ECX = [0xb74318] is the callee's own receiver boundary */
    reg_eax = cm_fn_00775790((u32)param_1);
    if (reg_eax == 0u) {
      table = 0x00485a60u; grp = 1;                     /* 00484691 */
    }
    else {
      table = 0x00485960u; grp = 2;                     /* 0048469f */
    }
  }
  else if ((param_3 & 0x20u) != 0u) {
    table = 0x00485ba0u; grp = 3;
  }
  else if ((param_3 & 0x40u) != 0u) {
    table = 0x00485ed0u; grp = 1;
  }
  else if ((param_3 & 0x80u) != 0u) {
    table = 0x00485fd0u; grp = 2;
  }
  else if ((param_3 & 0x100u) != 0u) {
    table = 0x004860e0u; grp = 3;
  }
  else if ((param_3 & 0x200u) != 0u) {
    table = 0x004863c0u; grp = 1;
  }
  else if ((param_3 & 0x8000000u) != 0u) {
    table = 0x004861e0u; grp = 2;
  }
  else if ((param_3 & 0x4000000u) != 0u) {
    table = 0x004862c0u; grp = 3;
  }
  else if ((param_3 & 0x400u) != 0u) {
    table = 0x004864e0u; grp = 1;
  }
  else if ((param_3 & 0x800u) != 0u) {
    table = 0x00486620u; grp = 2;
  }
  else if ((param_3 & 0x1000u) != 0u) {
    table = 0x00486760u; grp = 3;
  }
  else if ((param_3 & 0x2000u) != 0u) {
    /* 0048474f */
    reg_eax = cm_fn_00775790((u32)param_1);
    if (reg_eax == 0u) {
      table = 0x00486760u; grp = 1;                     /* 0048475f */
    }
    else {
      table = 0x00485960u; grp = 2;                     /* 0048476d */
    }
  }
  else if ((param_3 & 0x4000u) != 0u) {
    table = 0x004868c0u; grp = 3;
  }
  else if ((param_3 & 0x8000u) != 0u) {
    table = 0x00486970u; grp = 1;
  }
  else if ((param_3 & 0x10000u) != 0u) {
    table = 0x00486a30u; grp = 2;
  }
  else if ((param_3 & 0x20000u) != 0u) {
    table = 0x00486af0u; grp = 3;
  }
  else if ((param_3 & 0x40000u) != 0u) {
    table = 0x00486c70u; grp = 1;
  }
  else if ((param_3 & 0x20000000u) != 0u) {
    table = 0x00486e60u; grp = 2;
  }
  else if ((param_3 & 0x80000u) != 0u) {
    table = 0x00486f80u; grp = 3;
  }
  else if ((param_3 & 0x10000000u) != 0u) {
    table = 0x00487030u; grp = 1;
  }
  else if ((param_3 & 0x100000u) != 0u) {
    table = 0x00487140u; grp = 2;
  }
  else if ((param_3 & 0x200000u) != 0u) {
    table = 0x00487250u; grp = 3;
  }
  else if ((param_3 & 0x400000u) != 0u) {
    table = 0x00487360u; grp = 1;
  }
  else if ((param_3 & 0x800000u) != 0u) {
    table = 0x00487470u; grp = 2;
  }
  else if ((param_3 & 0x2000000u) != 0u) {
    table = 0x00485d20u; grp = 3;
  }
  else {
    goto L_0048486d;
  }
  /* 00484820 / 0048483c / 00484858 -> 00484865 */
  if (grp == 1u) {
    r64 = cm_fn_00945333((u32)frame.items, (u32)(s32)count, 4u, table);
  }
  else if (grp == 2u) {
    r64 = cm_fn_00945333((u32)frame.items, (u32)(s32)count, 4u, table);
  }
  else {
    r64 = cm_fn_00945333((u32)frame.items, (u32)(s32)count, 4u, table);
  }
  reg_edx = (u32)(r64 >> 32);           /* EDX after the 64-bit return */

L_0048486d:
  /* 0048486d */
  if (param_12 != 0) {
    s32 n = (s32)count;
    if (!(n < param_12)) {
      n = param_12;
    }
    /* 00484878: EAX = n, EDX = base */
    r64 = cm_fn_00945333((u32)frame.items, (u32)n, 4u, 0x004857a0u);
    reg_edx = (u32)(r64 >> 32);
  }
  if ((param_3 & 0x1000000u) == 0u) {
    /* 004848d9: forward */
    if (count > 0) {
      int **p = frame.items;
      s32 n = (s32)count;
      do {
        cm_fn_0068d0d0((volatile u8 *)param_9, reg_edx, (const u8 *)p, 4);
        p = p + 1;
        n = n + -1;
      } while (n != 0);
    }
  }
  else {
    /* 004848a4: reverse */
    count = (s8)(count + -1);
    if (count >= 0) {
      int **p = frame.items + (s32)count;
      s32 n = (s32)count + 1;
      do {
        cm_fn_0068d0d0((volatile u8 *)param_9, reg_edx, (const u8 *)p, 4);
        p = p + -1;
        n = n + -1;
      } while (n != 0);
    }
  }
  return 1;
}
