/* fn_00568560 -- module display.cpp / network.h, preferred-image VA 0x00568560,
   image base 0x00400000, SHA-256 0c78e7da...f86 (3.9.68 primary executable).
   Translated from the original instruction listing only.

   Frame: `sub esp,0x384` gives the listing base L.  ebp/esi are pushed, then
   ebx/edi for the body, so `[esp+N]` in the listing is `L + N - live_saved`.
   One scratch block keeps every original L-relative address at its listed
   offset (offsets are source facts, not compiler frame addresses):
     L+0x02..0x70  output slots of cm_fn_005ee4a0 and the derived copies
     L+0x64        saved ECX receiver, L+0x70 saved record base
     L+0x74/0x174  the two 0x100-byte format buffers (0x944cff/0x90d130)
     L+0x280       spill of record+0xba9ca
     L+0x284       0x100-byte string copy destination (rep movs at 0x568771/0x569280)
   L+0x68 of the second record list, return address at L+0x384+0.

   Third stack argument of cm_fn_0077ac30 (0x5693ce..0x5693ea):
     mov cx,[0xae2914] only writes the low half of ECX; the upper half is
      whatever the preceding original helper (0x5601e0, 0x5694a0 or 0x567b40)
      left behind.  UNRESOLVED FIDELITY BLOCKER: the cast below zero-extends,
      rather than transporting that post-call ECX.  It is not an unspecified
      C conversion.  See the source note at the call; gate success does not
      resolve this blocker. */
#include "cmre.h"

struct cm_net_buffer;

extern void CM_THISCALL cm_fn_00560140(u8 *self, u8 value);
extern void CM_THISCALL cm_fn_005601e0(int *param_1, u16 param_2);
extern void CM_THISCALL cm_fn_00560290(int *param_1, u16 param_2);
extern void CM_THISCALL cm_fn_00560340(int *param_1, u32 param_2);
extern void CM_THISCALL cm_fn_00567b40(struct cm_net_buffer *self, u32 value);
extern void CM_THISCALL cm_fn_005694a0(int *param_1, u32 param_2);
extern void CM_THISCALL cm_fn_00569540(int *param_1, u32 *param_2, short param_3);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern void CM_THISCALL cm_fn_005ee4a0(volatile u8 *self, volatile u32 *out_0c, volatile u32 *out_10, volatile u32 *out_14, volatile u32 *out_18, volatile u32 *out_1c, volatile u32 *out_20, volatile u32 *out_24, volatile u32 *out_38, volatile u16 *out_72, volatile u16 *out_74, volatile u32 *out_3c, volatile u16 *out_76, volatile u16 *out_78, volatile u32 *out_80, volatile u32 *out_40, volatile u32 *out_08, volatile u32 *out_48, volatile u16 *out_7a, volatile u32 *out_54);
extern char *cm_fn_0066f4e0(int param_1, char *param_2, ...);
/* `test eax,eax` at 0x5693f1 inspects all 32 bits, so the result is declared
   full width (the callee's EAX is 0 or 1). */
extern u32 CM_THISCALL cm_fn_0077ac30(int param_1, u32 param_2, int param_3, u32 param_4);
extern char *cm_fn_0090d130(u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define NET_BUF   CM_GLOBAL(u8 *, 0x00ae28f8)
#define NET_LEN   CM_GLOBAL(int, 0x00ae28fc)
#define NET_CAP   CM_GLOBAL(u32, 0x00ae2900)
#define NET_FLAG  CM_GLOBAL(u16, 0x00ae2914)
#define NET_DIAG  CM_GLOBAL(u32, 0x00b67a34)
#define NET_SELF  ((void *)(u32)0x00ae28f8)  /* ECX=0xae28f8 receiver of the buffer helpers */

#define FMT_NET   0x009c3734u  /* "...network.h" */
#define FMT_DISP  0x009c36ecu  /* "...display.cpp" */
#define FMT_MSG   0x009870e8u
#define MSG_ERR   0x009870e0u  /* "Error" */
#define MSG_STACK 0x00ae24d0u

/* Guarded-append failure: format a source/line tag into the two scratch
   buffers, build the dialog text, show it, clear the diag flag. */
#define APPEND_ERR(code)                                                      \
  do {                                                                        \
    cm_fn_00944cff(FMT_NET, 0, 0, (void *)(b + 0x174), (void *)(b + 0x74));    \
    uTmp = (u32)cm_fn_0090d130((u32)(b + 0x174), (u32)(b + 0x74), (code));     \
    cm_fn_00944c9f(MSG_STACK, FMT_MSG, uTmp, (u32)(b + 0x174),                \
                   (u32)(b + 0x74), (code));                                  \
    cm_fn_005e8290((const char *)MSG_ERR, (const char *)MSG_STACK, 0);        \
    NET_DIAG = 0;                                                             \
  } while (0)

u32 CM_THISCALL cm_fn_00568560(int param_1, u32 param_2, short param_3)
{
  u32 frame[0x0e1];             /* L+0x000 .. L+0x383 */
  u8 *b = (u8 *)frame;
  u32 uTmp;
  u32 len;
  u32 si;
  u32 rec;
  u32 idx;
  u32 uVar;
  u32 uVar1;
  u32 uVar2;

  NET_LEN = 0;                                /* 0x568578 */
  *(u32 *)(b + 0x64) = (u32)param_1;          /* 0x568574: saved receiver */
  if (NET_CAP < 1u) {                         /* 0x568571 cmp eax,1 / jae */
    /* 0x568580: first failure path keeps the eax/edx argument order. */
    cm_fn_00944cff(FMT_NET, 0, 0, (void *)(b + 0x74), (void *)(b + 0x174));
    uTmp = (u32)cm_fn_0090d130((u32)(b + 0x74), (u32)(b + 0x174), 0x85);
    cm_fn_00944c9f(MSG_STACK, FMT_MSG, uTmp, (u32)(b + 0x74),
                   (u32)(b + 0x174), 0x85);
    cm_fn_005e8290((const char *)MSG_ERR, (const char *)MSG_STACK, 0);
    NET_DIAG = 0;
  } else {
    *(u8 *)NET_BUF = 6;                       /* 0x5685de */
    NET_LEN = NET_LEN + 1;
  }

  /* 0x5685f2: append the 16-bit index. */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = (u16)param_3;
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }

  /* 0x568681: esi = receiver + sign-extended index * 0x18c. */
  idx = (u32)(int)param_3;
  rec = (u32)param_1 + idx * 396u;            /* *0x18c, 32-bit wrap like shl/add/lea */
  *(u32 *)(b + 0x70) = rec;                   /* 0x56870c */

  cm_fn_005ee4a0((volatile u8 *)(rec + 0xba95eu),
                 (volatile u32 *)(b + 0x54),  /* out_0c */
                 (volatile u32 *)(b + 0x44),  /* out_10 */
                 (volatile u32 *)(b + 0x0c),  /* out_14 */
                 (volatile u32 *)(b + 0x60),  /* out_18 */
                 (volatile u32 *)(b + 0x14),  /* out_1c */
                 (volatile u32 *)(b + 0x4c),  /* out_20 */
                 (volatile u32 *)(b + 0x1c),  /* out_24 */
                 (volatile u32 *)(b + 0x3c),  /* out_38 */
                 (volatile u16 *)(b + 0x58),  /* out_72 */
                 (volatile u16 *)(b + 0x04),  /* out_74 */
                 (volatile u32 *)(b + 0x08),  /* out_3c */
                 (volatile u16 *)(b + 0x02),  /* out_76 */
                 (volatile u16 *)(b + 0x18),  /* out_78 */
                 (volatile u32 *)(b + 0x24),  /* out_80 */
                 (volatile u32 *)(b + 0x5c),  /* out_40 */
                 (volatile u32 *)(b + 0x28),  /* out_08 */
                 (volatile u32 *)(b + 0x30),  /* out_48 */
                 (volatile u16 *)(b + 0x38),  /* out_7a */
                 (volatile u32 *)(b + 0x68)); /* out_54 */

  /* 0x568718..0x568760: the listing's copy-down of the out slots. */
  *(u32 *)(b + 0x6c) = *(u32 *)(b + 0x0c);    /* out_14 -> 0x6c */
  *(u32 *)(b + 0x2c) = *(u32 *)(b + 0x60);    /* out_18 -> 0x2c */
  *(u32 *)(b + 0x48) = *(u32 *)(b + 0x14);    /* out_1c -> 0x48 */
  *(u32 *)(b + 0x50) = *(u32 *)(b + 0x1c);    /* out_24 -> 0x50 */
  *(u32 *)(b + 0x20) = *(u32 *)(b + 0x5c);    /* out_40 -> 0x20 */
  *(u32 *)(b + 0x40) = *(u32 *)(rec + 0xba9b6u);
  *(u8  *)(b + 0x34) = *(u8  *)(b + 0x4c);    /* out_20 low byte -> 0x34 */
  *(u8  *)(b + 0x10) = *(u8  *)(b + 0x02);    /* out_76 low byte -> 0x10 */

  /* 0x568764..0x56878c: copy out_80 up to and including its NUL into L+0x284. */
  uVar = *(u32 *)(b + 0x24);
  if (uVar == 0u) {
    *(u8 *)(b + 0x284) = 0;
  } else {
    const u8 *p = (const u8 *)(void *)uVar;
    u8 *q = b + 0x284;
    while (*p != 0) { *q = *p; q = q + 1; p = p + 1; }
    *q = 0;
  }

  /* 0x568794: 4-byte out_0c, line 0x8d */
  if ((u32)NET_LEN + 4u <= NET_CAP) {
    *(u32 *)(NET_BUF + NET_LEN) = *(u32 *)(b + 0x54);
    NET_LEN = NET_LEN + 4;
  } else {
    APPEND_ERR(0x8d);
  }
  /* 0x56882d: 2-byte out_10, line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x44);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x5688c3: 2-byte out_14 (copy at 0x6c), line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x6c);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x568954: 2-byte out_18 (copy at 0x2c), line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x2c);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x5689e5: 2-byte out_1c (copy at 0x48), line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x48);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x568a76: 1-byte out_20 (copy at 0x34), line 0x85 */
  if ((u32)NET_LEN + 1u <= NET_CAP) {
    *(u8 *)(NET_BUF + NET_LEN) = *(u8 *)(b + 0x34);
    NET_LEN = NET_LEN + 1;
  } else {
    APPEND_ERR(0x85);
  }
  /* 0x568b04: 2-byte out_24 (copy at 0x50), line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x50);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x568b95: 4-byte out_38, line 0x8d */
  if ((u32)NET_LEN + 4u <= NET_CAP) {
    *(u32 *)(NET_BUF + NET_LEN) = *(u32 *)(b + 0x3c);
    NET_LEN = NET_LEN + 4;
  } else {
    APPEND_ERR(0x8d);
  }
  /* 0x568c24: 2-byte out_72, line 0x8b */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x58);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x8b);
  }
  /* 0x568cb5: 2-byte out_74, line 0x8b */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x04);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x8b);
  }
  /* 0x568d46: 4-byte out_3c, line 0x8d */
  if ((u32)NET_LEN + 4u <= NET_CAP) {
    *(u32 *)(NET_BUF + NET_LEN) = *(u32 *)(b + 0x08);
    NET_LEN = NET_LEN + 4;
  } else {
    APPEND_ERR(0x8d);
  }
  /* 0x568dd5: 1-byte out_76 (copy at 0x10), line 0x85 */
  if ((u32)NET_LEN + 1u <= NET_CAP) {
    *(u8 *)(NET_BUF + NET_LEN) = *(u8 *)(b + 0x10);
    NET_LEN = NET_LEN + 1;
  } else {
    APPEND_ERR(0x85);
  }
  /* 0x568e63: 2-byte out_78, line 0x8b */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x18);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x8b);
  }

  /* 0x568ef4..0x568f49: length-prefixed string at L+0x284. */
  {
    u32 n;
    len = 0;
    while (*(u8 *)(b + 0x284 + len) != 0) len = len + 1;
    len = len + 1;                            /* inc esi */
    cm_fn_005601e0(NET_SELF, (u16)len);       /* 0x568f0e */
    n = (u32)NET_LEN;                         /* reload after the helper */
    uVar1 = n + (u32)(int)(short)len;         /* movsx eax,si */
    {
      u8 *q = NET_BUF + n;
      const u8 *p = b + 0x284;
      u32 k = len;
      while (k != 0u) { *q = *p; q = q + 1; p = p + 1; k = k - 1; }
    }
    NET_LEN = (int)uVar1;
  }

  /* 0x568fba: 2-byte out_40, line 0x86 */
  if ((u32)NET_LEN + 2u <= NET_CAP) {
    *(u16 *)(NET_BUF + NET_LEN) = *(u16 *)(b + 0x20);
    NET_LEN = NET_LEN + 2;
  } else {
    APPEND_ERR(0x86);
  }
  /* 0x568fd6: 4-byte out_08, line 0x88 */
  if ((u32)NET_LEN + 4u <= NET_CAP) {
    *(u32 *)(NET_BUF + NET_LEN) = *(u32 *)(b + 0x28);
    NET_LEN = NET_LEN + 4;
  } else {
    APPEND_ERR(0x88);
  }
  /* 0x569066: 4-byte out_48, line 0x89 */
  if ((u32)NET_LEN + 4u <= NET_CAP) {
    *(u32 *)(NET_BUF + NET_LEN) = *(u32 *)(b + 0x30);
    NET_LEN = NET_LEN + 4;
  } else {
    APPEND_ERR(0x89);
  }

  /* 0x5690f6 / 0x569105: out_7a then record+0xba9b6 via the calling helpers. */
  cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x38));
  cm_fn_00567b40((struct cm_net_buffer *)NET_SELF, *(u32 *)(b + 0x40));

  /* 0x569114: the record's 16-bit link index is appended before the test. */
  rec = *(u32 *)(b + 0x70);
  si = (u32)(*(u16 *)(rec + 0xba9dcu));
  cm_fn_005601e0(NET_SELF, (u16)si);
  if (si != 0xffffu) {
    uVar = *(u32 *)(rec + 0xba9beu);
    uVar1 = *(u32 *)(rec + 0xba9c2u);
    uVar2 = *(u32 *)(rec + 0xba9c6u);
    *(u32 *)(b + 0x280) = *(u32 *)(rec + 0xba9cau);
    cm_fn_005601e0(NET_SELF, 0x10);           /* 0x569158 */
    cm_fn_005694a0(NET_SELF, uVar);           /* 0x569163 */
    cm_fn_005694a0(NET_SELF, uVar1);          /* 0x56916e */
    cm_fn_005694a0(NET_SELF, uVar2);          /* 0x569179 */
    cm_fn_005694a0(NET_SELF, *(u32 *)(b + 0x280)); /* 0x56918b reload */

    if (si != (u32)(u16)param_3) {            /* 0x569190 cmp si, word[esp+0x39c] */
      u32 rec2 = *(u32 *)(b + 0x64) + (u32)(int)(s16)si * 396u; /* 0x5691b4 movsx */

      cm_fn_005ee4a0((volatile u8 *)(rec2 + 0xba95eu),
                     (volatile u32 *)(b + 0x54),
                     (volatile u32 *)(b + 0x44),
                     (volatile u32 *)(b + 0x0c),
                     (volatile u32 *)(b + 0x60),
                     (volatile u32 *)(b + 0x14),
                     (volatile u32 *)(b + 0x4c),
                     (volatile u32 *)(b + 0x1c),
                     (volatile u32 *)(b + 0x3c),
                     (volatile u16 *)(b + 0x58),
                     (volatile u16 *)(b + 0x04),
                     (volatile u32 *)(b + 0x08),
                     (volatile u16 *)(b + 0x02),
                     (volatile u16 *)(b + 0x18),
                     (volatile u32 *)(b + 0x24),
                     (volatile u32 *)(b + 0x5c),
                     (volatile u32 *)(b + 0x28),
                     (volatile u32 *)(b + 0x30),
                     (volatile u16 *)(b + 0x38),
                     (volatile u32 *)(b + 0x68));

      /* 0x56922b..0x56926f (this pass does not refresh 0x6c). */
      *(u32 *)(b + 0x2c) = *(u32 *)(b + 0x60);
      *(u32 *)(b + 0x48) = *(u32 *)(b + 0x14);
      *(u8  *)(b + 0x34) = *(u8  *)(b + 0x4c);
      *(u32 *)(b + 0x50) = *(u32 *)(b + 0x1c);
      *(u32 *)(b + 0x20) = *(u32 *)(b + 0x5c);
      *(u32 *)(b + 0x40) = *(u32 *)(rec2 + 0xba9b6u);
      *(u8  *)(b + 0x10) = *(u8  *)(b + 0x02);

      /* 0x569273..0x56929b: string copy for the second record. */
      uVar = *(u32 *)(b + 0x24);
      if (uVar == 0u) {
        *(u8 *)(b + 0x284) = 0;
      } else {
        const u8 *p = (const u8 *)(void *)uVar;
        u8 *q = b + 0x284;
        while (*p != 0) { *q = *p; q = q + 1; p = p + 1; }
        *q = 0;
      }

      /* 0x5692a3..0x5693c7: same 19 fields through the calling helpers. */
      cm_fn_00560340(NET_SELF, *(u32 *)(b + 0x54));  /* out_0c */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x44));  /* out_10 */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x0c));  /* out_14 */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x2c));  /* out_18 */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x48));  /* out_1c */
      cm_fn_00560140(NET_SELF, *(u8  *)(b + 0x34));  /* out_20 */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x50));  /* out_24 */
      cm_fn_00560340(NET_SELF, *(u32 *)(b + 0x3c));  /* out_38 */
      cm_fn_00560290(NET_SELF, *(u16 *)(b + 0x58));  /* out_72 */
      cm_fn_00560290(NET_SELF, *(u16 *)(b + 0x04));  /* out_74 */
      cm_fn_00560340(NET_SELF, *(u32 *)(b + 0x08));  /* out_3c */
      cm_fn_00560140(NET_SELF, *(u8  *)(b + 0x10));  /* out_76 */
      cm_fn_00560290(NET_SELF, *(u16 *)(b + 0x18));  /* out_78 */
      len = 0;
      while (*(u8 *)(b + 0x284 + len) != 0) len = len + 1;
      len = len + 1;
      cm_fn_00569540(NET_SELF, (u32 *)(b + 0x284), (short)len); /* 0x56937c */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x20));  /* out_40 */
      cm_fn_005694a0(NET_SELF, *(u32 *)(b + 0x28));  /* out_08 */
      cm_fn_00567b40((struct cm_net_buffer *)NET_SELF, *(u32 *)(b + 0x30)); /* out_48 */
      cm_fn_005601e0(NET_SELF, *(u16 *)(b + 0x38));  /* out_7a */
      cm_fn_00567b40((struct cm_net_buffer *)NET_SELF, *(u32 *)(b + 0x40));
    }
  }

   /* UNRESOLVED: 0x5693ce changes CX only; 0x5693e0 pushes all of ECX.
      Predecessor calls are 0x569128 (link -1), 0x56918b (same record,
      branch 0x569198), and 0x5693c7 (second record).
      The expression below incorrectly supplies zero for ECX[31:16].
      Deadness is NOT established: in the original 0x77ac30, 0x77acd7
      loads this argument as a DWORD and, on the nonzero-selector send-failure
      path, 0x77ae51/0x77ae58/0x77ae59 forwards the entire original DWORD
      to an indirect callback.  Fix requires actual post-call ECX transport
      or further original-instruction proof about every reachable callback;
      neither an uninitialized C local nor a guessed receiver high half is
      a faithful replacement. */
  uVar = cm_fn_0077ac30((int)param_2, (u32)NET_BUF, NET_LEN, (u32)NET_FLAG);
  if (uVar != 0u) {
    *(u32 *)((u32)(*(u32 *)(b + 0x64)) + 0x12f551u) = 0;  /* 0x56948f */
    return 1;
  }

  /* 0x5693f9: display.cpp failure path. */
  cm_fn_00944cff(FMT_DISP, 0, 0, (void *)(b + 0x174), (void *)(b + 0x74));
  uTmp = (u32)cm_fn_0090d130((u32)(b + 0x174), (u32)(b + 0x74), 0xcfa);
  cm_fn_00944c9f(MSG_STACK, FMT_MSG, uTmp, (u32)(b + 0x174),
                 (u32)(b + 0x74), 0xcfa);
  cm_fn_0066f4e0((int)(u32)0x00ae28bc, (char *)0x009c3814);
  cm_fn_0066f4e0((int)(u32)0x00ae26c4, (char *)0x009c37b0,
                 (char *)(void *)MSG_STACK);
  cm_fn_005e8290((const char *)(void *)0x00ae28bc,
                 (const char *)(void *)0x00ae26c4, 0);
  return 0;
}
