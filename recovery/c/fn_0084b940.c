/* fn_0084b940 -- CM 01/02 3.9.68, preferred image base 0x00400000,
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Plain C reconstruction of the instruction stream at
   0x0084b940 (simulated_stats.cpp). Original callees, imports and globals stay
   explicit boundaries; nothing is inferred beyond the listing.

   Frame: `sub esp, 0x4fc` with four saved registers (ebx, ebp, esi, edi), so
   the listing's [esp + d] slots are frame offsets d - 0x10:
     [esp+0x14] -> +0x04  this            (reloaded after the filter loop)
     [esp+0x10] -> +0x00  loop/row index  [esp+0x1c] -> +0x0c  filtered count
     [esp+0x20] -> +0x10  selection array [esp+0x28] -> +0x18  count % 16
     [esp+0x2c] -> +0x1c  text buffer A   [esp+0x12c] -> +0x11c text buffer B
     [esp+0x22c] -> +0x21c GetLogicalDriveStringsA buffer (200 bytes)
     [esp+0x320] -> +0x2e4 formatted CD path (260 bytes)
     [esp+0x3f8] -> +0x3e8 cm_fn_004c0aa0 record (276 bytes)
   The buffers are one scratch allocation so their original adjacency is kept.

    HELD boundary: source preserves these operations; stack-output transcript
    and compiler-dependent import dispatch still require harness evidence.
    (1) Stack output. The listing hands original code pointers into its own
        frame to 0x944cff at VA 0x0084baa5 and VA 0x0084bd2a.
        The pending text/line pushes across 0x90d130 belong to 0x944c9f.
        Those two buffers are
       declared here as one 0x4fc-byte scratch object at exactly the listing's
       offsets, left uninitialised like the original frame, and passed by
       address in listing order.  Nothing is zero-filled, widened or split
       into separate arrays, so the original's own stack-output contract (and
       its dependence on the caller's leftover bytes) is stated, not simulated
       away.
   (2) Import pointer.  The drive scan enters KERNEL32 only through the two
       recorded IAT DWORDs, in the order the listing reads them, so the boundary
       is one auditable 4-byte load plus an indirect stdcall.
    (3) Message loop.  The `GetLogicalDriveStringsA` / `GetDriveTypeA` /
       0x004c0aa0 / 0x004c0c10 enumeration and both report paths stay in
       listing order, including the unconditional `jmp 0x0084b963` back into the
        report loop at 0x0084ba42 and the exit call 0x00945934(-1).
    These source choices do not by themselves establish ADR0008 support. */

#include "cmre.h"

#define G_00adadfc CM_GLOBAL(volatile u32, 0x00adadfc)
#define G_00ae235c CM_GLOBAL(volatile s32, 0x00ae235c)
#define G_00ae23b4 CM_GLOBAL(volatile u32, 0x00ae23b4)
#define G_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define G_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define G_00dd7118 CM_GLOBAL(s32, 0x00dd7118)

extern u32 CM_CDECL cm_fn_004c0aa0(const char *path, u32 *rec, u32 mode);
extern u32 CM_CDECL cm_fn_004c0c10(u32 object);
extern void cm_fn_005a8f60(void);
extern unsigned long long CM_CDECL cm_fn_005cc7d0(void);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *cm_fn_0066f4e0(int param_1, char *param_2);
/* Pinned image above: VA 0x0084bb48 pushes the input and 0x0084bb49
   loads ECX before 0x0084bb4f; preserve both boundary inputs. */
extern u32 CM_THISCALL cm_fn_00775790(u32 receiver, u32 input);
extern u32 CM_THISCALL cm_fn_0084c510(int *param_1, int param_2);
/* One pushed argument and `add esp, 4` at every call site (0x0084bc5d,
   0x0084bc6e, 0x0084bc9c), so it is a 1-argument cdecl call returning in EAX. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
/* VA 0x0084bab8..0x0084bacc and 0x0084bd3d..0x0084bd54:
   pending text/line pushes belong to the following variadic formatter. */
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void cm_fn_00945934(u32 param_1);

/* The two KERNEL32 imports are reached exactly as the original reaches them:
   `call dword ptr [0x009670f8]` at 0x0084b970 and `call dword ptr [0x00967100]`
   at 0x0084b997.  Loading that DWORD and calling through it keeps the original
   import-pointer indirection (one 4-byte read of the recorded IAT slot per
   call) instead of depending on a compiler-generated import table.  Both are
   Win32 stdcall, as the listing shows by popping their own arguments (there is
   no `add esp, 8` / `add esp, 4` after either call). */
typedef u32 (CM_STDCALL *cm_iat_glds_t)(u32 nBufferLength, char *lpBuffer);
typedef u32 (CM_STDCALL *cm_iat_gdt_t)(const char *lpRootPathName);
#define PTR_00_9670f8 ((cm_iat_glds_t)(u32)CM_GLOBAL(volatile u32, 0x009670f8))
#define PTR_00_967100 ((cm_iat_gdt_t)(u32)CM_GLOBAL(volatile u32, 0x00967100))

int * CM_THISCALL cm_fn_0084b940(int *param_1, int param_2)
{
  u32 scratch[0x4fc / 4];
  u8 *const frame = (u8 *)scratch;
  u8 *const text_a = frame + 0x1c;
  u8 *const text_b = frame + 0x11c;
  char *const drives = (char *)(frame + 0x21c);
  char *const cdbuf = (char *)(frame + 0x2e4);
  u32 *const cdrec = (u32 *)(frame + 0x3e8);
  u32 *array;
  s32 count;
  u32 rem;

  count = 0;

  if (G_00dd7118 != 0) goto after_scan;

  /* 0x0084b963: drive scan; repeats the message box until a CD is accepted. */
  for (;;) {
    u32 n;

    n = PTR_00_9670f8(200, drives);
    if (n != 0) {
      if (drives[0] != '\0') {
        s8 ix;
        const char *root;

        ix = 0;
        root = drives;
        for (;;) {
          if (PTR_00_967100(root) == 5) {
            cm_fn_00944c9f((u32)cdbuf, 0x00987e00u, (u32)root,
                           'c', 'm', '0', '1', '0', '2', '.', 'e', 'x', 'e');
            if (cm_fn_004c0aa0(cdbuf, cdrec, 0) == 0) goto after_scan;
            cm_fn_004c0c10((u32)cdrec);
          }
          ix = (s8)((u8)ix + 4);
          root = drives + (s32)ix;
          if (drives[(s32)ix] == '\0') break;
        }
      }
    }
    cm_fn_0066f4e0((int)(u32)text_b, (char *)0x00987df4);
    cm_fn_0066f4e0((int)(u32)text_a, (char *)0x00987dc4);
    cm_fn_005e8290((const char *)text_b, (const char *)text_a, 1);
  }

after_scan:
  /* 0x0084ba4f */
  if (param_2 != 0) {
    cm_fn_0084c510(param_1, param_2);
    return param_1;
  }

  /* 0x0084ba73: selection array, DAT_00ae235c entries of 4 bytes. */
  array = (u32 *)(u32)cm_fn_00944e46(((u32)G_00ae235c) * 4u);
  if (array == 0) {
    /* 0x0084ba91: allocation failure report, line 0x49. */
    cm_fn_00944cff(0x00a8278cu, 0, 0, text_a, text_b);
    cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8u,
                   (u32)cm_fn_0090d130(), text_a, text_b, 0x49);
    cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, 0);
    G_00b67a34 = 0;
    cm_fn_005cc7d0();
    cm_fn_005a8f60();
    cm_fn_00945934(0xffffffffu);
    /* 0x00945934 does not return; the listing falls through to 0x0084bafb. */
  }

  /* 0x0084bafb: filter records into the array. */
  {
    s32 i;
    u32 off;
    u32 *out;
    u8 *recp;

    i = 0;
    off = 0;
    out = array;
    count = 0;
    if (G_00ae235c > 0) {
      recp = (u8 *)(G_00ae23b4 + 0x60u);
      for (;;) {
        u32 f;
        u32 recv;
        u32 input;

        /* 0x0084bb18 */
        f = *(u32 *)(recp - 9);
        if (f != 0) {
          u32 index = CM_GLOBAL(volatile u32, f);
          u32 table = G_00adadfc;
          if (CM_GLOBAL(volatile u32, table + index * 4u) != 0) goto next;
        }
        f = *(u32 *)recp;
        if (f != 0) {
          u32 index = CM_GLOBAL(volatile u32, f);
          u32 table = G_00adadfc;
          if (CM_GLOBAL(volatile u32, table + index * 4u) != 0) goto next;
        }
        /* 0x0084bb49: receiver load for the forwarded-ECX boundary. */
        input = off + G_00ae23b4;
        recv = CM_GLOBAL(volatile u32, 0x00b74318);
        if (cm_fn_00775790(recv, input) != 0) goto next;
        count = count + 1;
        *out = *(u32 *)(off + G_00ae23b4);
        out = out + 1;
      next:
        i = i + 1;
        off = off + 0x245u;
        recp = recp + 0x245;
        if (i >= G_00ae235c) break;
      }
    }
  }

  /* 0x0084bb8f: rows of 16 = ceil(count / 16), remainder kept in +0x18. */
  {
    s32 adj;
    s32 rows;
    u32 r;

    adj = (count < 0) ? 0xf : 0;
    rows = (count + adj) >> 4;
    r = ((u32)count) & 0x8000000fu;
    *param_1 = rows;
    if ((s32)r < 0) r = ((r - 1u) | 0xfffffff0u) + 1u;
    rem = r;
    if (rem != 0) *param_1 = rows + 1;
  }

  /* 0x0084bbb6: descending passes over the selected indices, ordered by the
     16-bit field at record + 0x80 (records are 0x245 bytes apart). */
  if (count - 1 > 0) {
    u32 *base_i;
    s32 i;

    base_i = array;
    i = 1;
    for (;;) {
      if (i < count) {
        s32 k;
        u32 *p1;
        u32 *p2;

        k = count - i;
        p1 = base_i;
        p2 = base_i + 1;
        do {
          u32 a;
          u32 b;
          u32 records;
          s16 rank_a;
          s16 rank_b;

          a = *p1;
          b = *p2;
          records = G_00ae23b4;
          rank_a = CM_GLOBAL(volatile s16, records + a * 0x245u + 0x80u);
          rank_b = CM_GLOBAL(volatile s16, records + b * 0x245u + 0x80u);
          if (rank_a < rank_b) {
            *p1 = b;
            *p2 = a;
          }
          p2 = p2 + 1;
          k = k - 1;
        } while (k != 0);
      }
      i = i + 1;
      base_i = base_i + 1;
      if (!(i - 1 < count - 1)) break;
    }
  }

  /* 0x0084bc4a: shuffle, about count/2 swaps with a +/- rand(count/150) jitter. */
  {
    s32 half;

    half = count / 2;
    if (half > 0) {
      s32 k;
      u32 idx;

      k = half;
      idx = cm_fn_0090cfc0((u32)count);
      for (;;) {
        u32 n150;
        u32 a;
        u32 b;
        s32 j;

        n150 = (u32)(count / 150);
        a = cm_fn_0090cfc0(n150);
        b = cm_fn_0090cfc0(n150);
        j = (s32)(idx - a + b);
        if (j < 0) continue;
        if (j >= count) continue;
        {
          u32 vj;
          u32 vi;

          vj = array[(u32)j];
          vi = array[idx];
          array[idx] = vj;
          array[(u32)j] = vi;
        }
        k = k - 1;
        if (k == 0) break;
        idx = cm_fn_0090cfc0((u32)count);
      }
    }
  }

  /* 0x0084bcc0: rows of 17 DWORDs (count + 16 values) per row. */
  {
    u32 *rowbase;
    u32 row;
    u32 bitoff;
    u32 src;
    s32 r;

    rowbase = (u32 *)(u32)cm_fn_00944e46(((u32)*param_1) * 0x44u);
    param_1[1] = (int)(u32)rowbase;
    row = 0;
    bitoff = 0;
    src = 0;
    r = 0;
    if (*param_1 > 0) {
      for (;;) {
        u8 *rows;
        u32 inner;

        rows = (u8 *)(u32)param_1[1];
        inner = 0;
        *(u32 *)(rows + row) = 0;
        for (;;) {
          if (src == (u32)count) {
            /* 0x0084bd14: more selected entries than the rows can hold. */
            cm_fn_00944cff(0x00a8278cu, 0, 0, text_a, text_b);
            cm_fn_00944c9f((u32)&G_00ae24d0, 0x009870e8u,
                           (u32)cm_fn_0090d130(), text_a, text_b, 0x95);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_00ae24d0, 0);
            G_00b67a34 = 0;
          }
          rows = (u8 *)(u32)CM_GLOBAL(volatile u32, (u32)param_1 + 4u);
          {
            u32 value = array[src];
            u32 used = CM_GLOBAL(volatile u32, (u32)rows + row);
            CM_GLOBAL(volatile u32, (u32)rows + (used + bitoff) * 4u + 4u) = value;
          }
          rows = (u8 *)(u32)CM_GLOBAL(volatile u32, (u32)param_1 + 4u);
          CM_GLOBAL(volatile u32, (u32)rows + row) =
            CM_GLOBAL(volatile u32, (u32)rows + row) + 1u;
          src = src + 1;
          if (rem != 0) {
            if (r < (s32)(16u - rem)) {
              if (inner == 0xe) break;
            }
          }
          inner = inner + 1;
          if (inner >= 0x10) break;
        }
        r = r + 1;
        bitoff = bitoff + 0x11u;
        row = row + 0x44u;
        if (r >= *param_1) break;
      }
    }
    cm_fn_009452ca((u32)array);
    return param_1;
  }
}
