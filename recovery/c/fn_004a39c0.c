/* cm_fn_004a39c0, preferred image VA 0x004a39c0, image base 0x00400000.

   Translated from the original instructions only (listing in the farm bundle).

   HELD concern resolution: the four small stack strings at L+0x00, L+0x02,
   L+0x04 and L+0x06 are destinations of cm_fn_0066f4e0; the original never
   initializes those bytes (the draft's four separate arrays would not preserve
   their adjacency either). This source keeps every untouched original byte
   uninitialized inside one scratch allocation with the listing's exact offsets
   (L+0x00..L+0x07 strings, L+0x08 loop bound, L+0x0c and L+0x10c the two 256-byte
   buffers), performs the four formatter calls in listing order, then re-reads
   and scans the same bytes in the listing's order. No initialization or
   narrowing is invented.

   Entry convention: `ret 0x14` with five stack arguments and no ECX receiver,
   so CM_STDCALL. The error paths' final message call keeps the three earlier
   0x0090d130 pushes live as its own 4th..6th arguments; see the comment at
   `tail` below. */
#include "cmre.h"

/* Callees: original code, cdecl (each call's argument bytes are reclaimed by
   the caller's `add esp`; 0x4a3e7c reclaims the three calls' 0x24 bytes at once). */
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0066f4e0(char *destination, char *format, u32 arg3);
extern u32 cm_fn_0090d130(void *arg1, void *arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

#define A_009870E0 ((const char *)0x009870e0u) /* message global */
#define A_009870E8 0x009870e8u                 /* format "v=%s..." */
#define A_0098806C 0x0098806cu                 /* format, one integer */
#define A_0098C11C 0x0098c11cu                 /* format, one integer (quotient) */
#define A_0099CA94 0x0099ca94u                 /* source path format */
#define A_0099CC4C ((const char *)0x0099cc4cu)
#define A_0099CC54 ((const char *)0x0099cc54u)
#define A_0099CC58 0x0099cc58u                 /* format "%d(%d)" */
#define A_0099CC60 ((char *)0x0099cc60u)       /* "L..." format */
#define A_0099CC78 ((char *)0x0099cc78u)       /* "D..." format */
#define A_0099CC90 ((char *)0x0099cc90u)       /* "X..." format */
#define A_0099CCB0 ((char *)0x0099ccb0u)       /* "W..." format */
#define A_00AD9C64 0x00ad9c64u                 /* formatter argument 3 */
#define A_00AE24D0 0x00ae24d0u                 /* scratch message buffer */

/* CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preferred-image VAs 004a3a0e-004a3a29: scan before writing, then
   forward DWORD transfers followed by the saved count's remaining bytes.
   Volatile accesses retain scan/copy order and overlapping-transfer widths. */
static u32 scan_count_004a39c0(u32 source)
{
  u32 remaining = 0xffffffffu;
  while (remaining != 0) {
    u8 b = CM_GLOBAL(volatile u8, source);
    source++;
    remaining--;
    if (b == 0)
      break;
  }
  return ~remaining;
}

static void copy_count_004a39c0(u32 destination, u32 source, u32 count)
{
  u32 n;
  for (n = count >> 2; n != 0; n--) {
    u32 word = CM_GLOBAL(volatile u32, source);
    CM_GLOBAL(volatile u32, destination) = word;
    source += 4;
    destination += 4;
  }
  for (n = count & 3u; n != 0; n--) {
    u8 byte = CM_GLOBAL(volatile u8, source);
    CM_GLOBAL(volatile u8, destination) = byte;
    source++;
    destination++;
  }
}

/* Same pinned image: preferred-image VAs 004a3b55-004a3b78,
   004a3c2c-004a3c4f and 004a3d03-004a3d26 save the SOURCE
   count before scanning the destination; never re-test copied bytes for NUL. */
static void append_004a39c0(char *destination, char *source)
{
  u32 count = scan_count_004a39c0((u32)source);
  u32 end = (u32)destination + scan_count_004a39c0((u32)destination) - 1u;
  copy_count_004a39c0(end, (u32)source, count);
}

/* `ret 0x14` with five stack arguments and no incoming ECX receiver: stdcall. */
u32 CM_STDCALL cm_fn_004a39c0(u32 arg1, s8 arg2, char *arg3, int arg4, int arg5)
{
  u8 L[0x20c];
  u8 *rec;
  char *dest;
  void *p1;
  void *p2;
  u32 errcode;
  u32 val;

  if (arg3 == 0 || arg4 < 0x33) {
    /* 004a3e29: (fmt, 0, 0, L+0x0c, L+0x10c) */
    cm_fn_00944cff(A_0099CA94, 0, 0, (void *)(L + 0x0c), (void *)(L + 0x10c));
    p1 = L + 0x0c;
    p2 = L + 0x10c;
    errcode = 0xac4;
    goto tail;
  }

  /* 004a39e9-004a3a29: pre-scanned count includes the NUL. */
  {
    const char *s;
    u32 count;
    if (arg2 == 3 || arg2 == 4 || arg2 == 5 || arg2 == 2) {
      s = A_0099CC4C;
    }
    else {
      s = A_0099CC54;
    }
    count = scan_count_004a39c0((u32)s);
    copy_count_004a39c0((u32)arg3, (u32)s, count);
  }

  rec = (u8 *)arg1;
  if (rec == 0) {
    /* 004a3a36: (fmt, 0, 0, L+0x10c, L+0x0c), then 0090d130(L+0x10c, L+0x0c, 0xacf) */
    cm_fn_00944cff(A_0099CA94, 0, 0, (void *)(L + 0x10c), (void *)(L + 0x0c));
    p1 = L + 0x10c;
    p2 = L + 0x0c;
    errcode = 0xacf;
    goto tail;
  }

  dest = arg3;
  switch (arg2) {
  case 1:
    val = (u32)*(u8 *)(rec + 4);
    break;
  case 2: {
    u8 n = *(u8 *)(rec + 5);
    if (n == 0) {
      return 1;
    }
    /* 004a3a88: signed DWORD at +6 divided by zero-extended BYTE at +5 */
    cm_fn_00944c9f((u32)(void *)dest, A_0098C11C,
                   (u32)(*(s32 *)(rec + 6) / (s32)(u32)n));
    return 1;
  }
  case 3: {
    u8 *p;
    u32 *bound;
    /* 004a3ac0: formatter destinations L+2, L+6, L+4, L+0 in this order */
    cm_fn_0066f4e0((char *)(L + 2), A_0099CCB0, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 6), A_0099CC90, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 4), A_0099CC78, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 0), A_0099CC60, A_00AD9C64);
    *dest = 0;
    p = rec + 0xe;
    bound = (u32 *)(L + 8);
    *bound = 0xfffffff6u - arg1; /* -10 - record */
    for (;;) {
      u8 b = *p;
      char *s = (char *)0;
      if (b == 3) {
        s = (char *)(L + 2);
      }
      else if (b == 2) {
        s = (char *)(L + 6);
      }
      else if (b == 1) {
        s = (char *)(L + 4);
      }
      else if (b == 0xfd) {
        s = (char *)(L + 0);
      }
      if (s != (char *)0) {
        append_004a39c0(dest, s);
      }
      p--;
      if ((s32)(*bound + (u32)p) < 0) {
        break;
      }
    }
    return 1;
  }
  case 4: {
    u8 *p;
    u32 *bound;
    /* 004a3b97: same destinations, record scan starts at +0x13 */
    cm_fn_0066f4e0((char *)(L + 2), A_0099CCB0, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 6), A_0099CC90, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 4), A_0099CC78, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 0), A_0099CC60, A_00AD9C64);
    *dest = 0;
    p = rec + 0x13;
    bound = (u32 *)(L + 8);
    *bound = 0xfffffff1u - arg1; /* -15 - record */
    for (;;) {
      u8 b = *p;
      char *s = (char *)0;
      if (b == 3) {
        s = (char *)(L + 2);
      }
      else if (b == 2) {
        s = (char *)(L + 6);
      }
      else if (b == 1) {
        s = (char *)(L + 4);
      }
      else if (b == 0xfd) {
        s = (char *)(L + 0);
      }
      if (s != (char *)0) {
        append_004a39c0(dest, s);
      }
      p--;
      if ((s32)(*bound + (u32)p) < 0) {
        break;
      }
    }
    return 1;
  }
  case 5: {
    u8 *p;
    u32 *bound;
    /* 004a3c6e: same destinations, record scan starts at +0x18 */
    cm_fn_0066f4e0((char *)(L + 2), A_0099CCB0, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 6), A_0099CC90, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 4), A_0099CC78, A_00AD9C64);
    cm_fn_0066f4e0((char *)(L + 0), A_0099CC60, A_00AD9C64);
    *dest = 0;
    p = rec + 0x18;
    bound = (u32 *)(L + 8);
    *bound = 0xffffffecu - arg1; /* -20 - record */
    for (;;) {
      u8 b = *p;
      char *s = (char *)0;
      if (b == 3) {
        s = (char *)(L + 2);
      }
      else if (b == 2) {
        s = (char *)(L + 6);
      }
      else if (b == 1) {
        s = (char *)(L + 4);
      }
      else if (b == 0xfd) {
        s = (char *)(L + 0);
      }
      if (s != (char *)0) {
        append_004a39c0(dest, s);
      }
      p--;
      if ((s32)(*bound + (u32)p) < 0) {
        break;
      }
    }
    return 1;
  }
  case 6:
    val = *(u32 *)(rec + 0x19);
    break;
  case 7:
    val = *(u32 *)(rec + 0x2d);
    break;
  case 8:
    val = *(u32 *)(rec + 0x41);
    break;
  case 9:
    val = *(u32 *)(rec + 0x55);
    break;
  case 10:
    val = (u32)*(u8 *)(rec + 0x69);
    break;
  case 11:
    val = (u32)*(u8 *)(rec + 0x6a);
    break;
  case 12: {
    u8 b = *(u8 *)(rec + 0x6b);
    if (b == 0 || arg5 == 0) {
      val = (u32)b;
      break;
    }
    cm_fn_00944c9f((u32)(void *)dest, A_0099CC58, (u32)b, (u32)*(u8 *)(rec + 0x6c));
    return 1;
  }
  case 13:
    val = (u32)*(u8 *)(rec + 0x6d);
    break;
  case 14:
    val = (u32)*(u8 *)(rec + 0x6e);
    break;
  case 15:
    val = (u32)*(u8 *)(rec + 0x6f);
    break;
  case 16:
    val = (u32)*(u8 *)(rec + 0x70);
    break;
  case 17:
    val = (u32)*(u8 *)(rec + 0x71);
    break;
  default:
    /* 004a3df9: (fmt, 0, 0, L+0x0c, L+0x10c), 0090d130(L+0x0c, L+0x10c, 0xb96) */
    cm_fn_00944cff(A_0099CA94, 0, 0, (void *)(L + 0x0c), (void *)(L + 0x10c));
    p1 = L + 0x0c;
    p2 = L + 0x10c;
    errcode = 0xb96;
    goto tail;
  }
  cm_fn_00944c9f((u32)(void *)dest, A_0098806C, val);
  return 1;

tail:
  {
    /* The three argument slots pushed for 0x0090d130 (`p1`, `p2`, line code) are
       never reclaimed before the 0x00944c9f call below; they live at [esp+0x10],
       [esp+0x14] and [esp+0x18] of that callee, whose format at 0x009870e8
       ("v=%s,%s:%s:%d") consumes four conversions. Reproduce them as the
       callee's 4th..6th arguments (C.md: pushes before call A that belong to
       the next call). */
    u32 r = cm_fn_0090d130(p1, p2, errcode);
    cm_fn_00944c9f(A_00AE24D0, A_009870E8, r, p1, p2, errcode);
  }
  cm_fn_005e8290(A_009870E0, (const char *)0x00ae24d0u, 0);
  CM_GLOBAL(u32, 0x00b67a34) = 0;
  return 0;
}
