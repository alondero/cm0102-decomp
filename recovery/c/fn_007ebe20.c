/* fn_007ebe20 -- CM 01/02 3.9.68 (cm0102.exe, SHA-256
 * 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86),
 * preferred-image VA 0x007ebe20, module
 * e:\dev\cm3\cm3 00-01\cm3\code\comp\leagues\rus_prm.cpp.
 *
 * Entry convention: ECX is read before written (0x007ebe26 stores the incoming
 * ECX into L+0x20 and it is reloaded at 0x007ec0f9 / 0x007ec0b9), so the
 * routine is a thiscall member function; EDX is written before it is read, so
 * the convention is not fastcall.  EAX at return is incidental (the last
 * callee's result), the routine is void.
 *
 * Frame: "sub esp,0x22c" (0x007ebe20) plus ebx/ebp/esi/edi pushed at
 * 0x007ebe2a..0x007ebe33.  Naming the post-sub stack base L, the local slots
 * are (offsets are relative to L):
 *   L+0x00 dword  matched-entry counter, later reused as the array cursor
 *   L+0x04 dword  loop index
 *   L+0x08 dword  record cursor, later the matched-entry count
 *   L+0x0c dword  flag B
 *   L+0x10 dword  flag C
 *   L+0x14 dword  club id derived from DAT_009cf920
 *   L+0x18 dword  allocated array pointer
 *   L+0x1c dword  flag A
 *   L+0x20 dword  incoming ECX (this)
 *   L+0x24 dword  club id derived from DAT_009cf928
 *   L+0x28 dword  league object pointer
 *   L+0x2c bytes  buffer 1 (256 bytes, ends at L+0x12c)
 *   L+0x12c bytes buffer 2 (256 bytes, ends at L+0x22c)
 * The two buffers stay in one allocation so their original adjacency (and any
 * overflow from buffer 1 into buffer 2) is preserved.
 */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

#define DAT_009cf914 CM_GLOBAL(int, 0x009cf914)
#define DAT_009cf918 CM_GLOBAL(int, 0x009cf918)
#define DAT_009cf91c CM_GLOBAL(int, 0x009cf91c)
#define DAT_009cf920 CM_GLOBAL(int, 0x009cf920)
#define DAT_009cf924 CM_GLOBAL(int, 0x009cf924)
#define DAT_009cf928 CM_GLOBAL(int, 0x009cf928)
#define DAT_009cf92c CM_GLOBAL(int, 0x009cf92c)
#define DAT_00adadfc CM_GLOBAL(int, 0x00adadfc)
#define DAT_00ae2350 CM_GLOBAL(int, 0x00ae2350)
#define DAT_00ae235c CM_GLOBAL(int, 0x00ae235c)
#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23d0 CM_GLOBAL(int, 0x00ae23d0)
#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)
#define STR_009870e0 ((const char *)0x009870e0)
#define STR_009870e8 ((const char *)0x009870e8)
#define STR_00a7acc8 ((const char *)0x00a7acc8)

/* Callee map, taken from the original instructions (see per-call comments):
 * 0x004afbd0 -- cdecl, three stack arguments (base, count, wanted): the call at
 *   0x007ec009 is preceded by "push 3 / push ecx / push eax", so the callee
 *   reads base = the array pointer, count = the matched-entry counter and
 *   wanted = 3; the caller releases them with "add esp,0xc" at 0x007ec013.
 * 0x005e8290 -- cdecl, three stack arguments (0x007ebf37..0x007ebf43).
 * 0x006826d0 -- thiscall, one stack argument (0x007ec0f1/0x007ec10f/0x007ec12b
 *   push the index, ECX = league object at 0x007ec0ea/0x007ec110/0x007ec12c).
 * 0x006830b0 -- three stack arguments, callee pops 12 bytes (the stack is back
 *   at L-0x10 for the reload at 0x007ec0c3), ECX = this (0x007ec0b9 loads
 *   L+0x20), so the receiver is the first parameter.
 * 0x006831a0 -- three stack arguments, callee pops 12 bytes, ECX = this
 *   (0x007ec0fe / 0x007ec119 / 0x007ec136 load L+0x20), so the receiver is
 *   the first parameter.
 * 0x0090d130 -- cdecl, three stack arguments: the single "add esp,0x24" at
 *   0x007ebf48 only balances if this call, 0x00944c9f and 0x005e8290 each
 *   leave three pushed words behind.
 * 0x00944c9f -- cdecl, variadic (0x007ebf27..0x007ebf32); it also receives
 *   the three words 0x0090d130 left on the stack.
 * 0x00944cff -- cdecl, five stack arguments (0x007ebefd..0x007ebf03).
 * 0x00944e46 -- cdecl, one stack argument (0x007ebee1).
 * 0x009452ca -- cdecl, one stack argument (0x007ebff1, 0x007ec141).
 */
extern u32 CM_CDECL cm_fn_004afbd0(u32 base, u32 count, u32 wanted);
extern void CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern u32 CM_THISCALL cm_fn_006830b0(void *self, u32 arg1, u32 arg2, u32 arg3);
extern void CM_THISCALL cm_fn_006831a0(void *self, u8 *param_2, u32 *param_3, u32 param_4);
extern char *CM_CDECL cm_fn_0090d130(char *param_1, char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

/* The two scratch buffers are stack bytes the original never writes before the
 * failure path uses them; the callees at 0x00944cff / 0x0090d130 fill them
 * before reading them back.  They stay in one allocation so their original
 * adjacency (and any overflow from buffer 1 into buffer 2) is preserved. */
typedef struct {
  u8 buf1[0x100];   /* L+0x2c */
  u8 buf2[0x100];   /* L+0x12c */
} cm_scratch_007ebe20;

void CM_THISCALL cm_fn_007ebe20(void *self)
{
  cm_scratch_007ebe20 sc;
  u32 base;         /* DAT_00ae23d0, read once at 0x007ebe6e */
  u32 esi_id;       /* 107*DAT_009cf914 + base */
  u32 edi_id;       /* 107*DAT_009cf918 + base */
  u32 ebx_id;       /* 107*DAT_009cf91c + base */
  u32 id_920;       /* 107*DAT_009cf920 + base (L+0x14) */
  u32 ebp_id;       /* 107*DAT_009cf924 + base */
  u32 id_928;       /* 107*DAT_009cf928 + base (L+0x24) */
  u32 id_92c;       /* 107*DAT_009cf92c + base */
  u32 league;       /* DAT_00adadfc[DAT_009cf92c] (L+0x28) */
  u32 arr;          /* allocated array (L+0x18) */
  u32 count;        /* matched-entry counter (L+0x00) */
  u32 count2;       /* (s16)count, the second loop bound */
  u32 idx;          /* loop index (L+0x04) */
  u32 rec;          /* record cursor (L+0x08 in the first loop) */
  u32 bound;        /* DAT_00ae235c - 2*DAT_00ae2350 */
  u32 flagA;        /* L+0x1c */
  u32 flagB;        /* L+0x0c */
  u32 flagC;        /* L+0x10 */
  u32 cursor;       /* L+0x00 reused as the array cursor */
  u32 type;
  u32 entry;
  u32 value;

  /* 0x007ebe2b..0x007ebea4: the six club ids, each 107*count + base.
   * The original builds 107*x as (x*3 + x*3*8) * 4 - x and adds the base
   * read once at 0x007ebe6e; the arithmetic is 32 bit and wraps. */
  base = (u32)DAT_00ae23d0;
  esi_id = (u32)DAT_009cf914 * 107u + base;
  edi_id = (u32)DAT_009cf918 * 107u + base;
  ebx_id = (u32)DAT_009cf91c * 107u + base;
  id_920 = (u32)DAT_009cf920 * 107u + base;
  ebp_id = (u32)DAT_009cf924 * 107u + base;
  id_928 = (u32)DAT_009cf928 * 107u + base;

  /* 0x007ebec0: league = DAT_00adadfc[DAT_009cf92c] */
  league = *(u32 *)((u32)DAT_00adadfc + (u32)DAT_009cf92c * 4u);

  count = 0;        /* 0x007ebe3c */
  flagA = 0;        /* 0x007ebec5 */
  flagB = 0;        /* 0x007ebec9 */
  flagC = 0;        /* 0x007ebecd */

  /* 0x007ebed1..0x007ebeea: allocate 4*DAT_00ae235c bytes, keep EAX */
  arr = (u32)cm_fn_00944e46((u32)DAT_00ae235c * 4u);
  if (arr == 0) {
    /* 0x007ebef2..0x007ebf43: report the allocation failure. */
    cm_fn_00944cff((u32)STR_00a7acc8, 0, 0, (void *)sc.buf2, (void *)sc.buf1);
    /* 0x007ebf1b..0x007ebf21 push three words for 0x0090d130.  That callee
     * is cdecl, so its words stay on the stack ("add esp,0x24" at 0x007ebf48
     * releases all three calls at once) and become the trailing arguments of
     * the formatter call at 0x007ebf32, which sees
     * (dest, fmt, EAX, buffer2, buffer1, 0x2bd). */
    value = (u32)cm_fn_0090d130((char *)sc.buf2, (char *)sc.buf1, 0x2bd);
    cm_fn_00944c9f((u32)&DAT_00ae24d0, (u32)STR_009870e8, value,
                   (u32)sc.buf2, (u32)sc.buf1, 0x2bd);
    cm_fn_005e8290(STR_009870e0, (const char *)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;   /* 0x007ebf4b */
    return;
  }

  /* 0x007ebf60..0x007ebfe5: collect the records whose type field matches one
   * of the six club ids.  The record cursor starts at DAT_00ae23b4 and steps
   * by 0x245 per entry. */
  rec = (u32)DAT_00ae23b4;
  idx = 0;
  bound = (u32)DAT_00ae235c - (u32)DAT_00ae2350 * 2u;
  if ((s32)bound > 0) {
    do {
      type = *(u32 *)(rec + 0x57);
      if (type == esi_id || type == edi_id || type == ebx_id ||
          type == id_920 || type == ebp_id || type == id_928) {
        /* 0x007ebfaa: the index is the sign extended low word of the counter */
        *(u32 *)(arr + (u32)(s32)(s16)count * 4u) = rec;
        count++;
      }
      rec += 0x245u;
      idx++;
      bound = (u32)DAT_00ae235c - (u32)DAT_00ae2350 * 2u;
    } while ((s32)idx < (s32)bound);
  }

  /* 0x007ebfeb: "cmp cx,3" - only the low word of the counter is tested. */
  if ((s16)count >= 3) {
    /* 0x007ec005..0x007ec009: three pushed words, EAX = arr at the call. */
    cm_fn_004afbd0(arr, count, 3);

    /* 0x007ec00e: the loop bound is the sign extended low word of the
     * counter; 0x007ec016..0x007ec024 reset the index and test the bound. */
    count2 = (u32)(s32)(s16)count;
    idx = 0;
    if ((s32)count2 > 0) {
      cursor = arr;   /* 0x007ec02a..0x007ec02e */
      for (;;) {
        /* 0x007ec032..0x007ec048: stop once all three types were seen. */
        if (flagA && flagB && flagC) {
          break;
        }
        rec = *(u32 *)cursor;
        type = *(u32 *)(rec + 0x57);
        if (type == esi_id || type == edi_id) {
          if (flagA) {
            goto next_entry;
          }
          flagA = 1;
        }
        if (type == ebx_id || type == id_920) {
          if (flagB) {
            goto next_entry;
          }
          flagB = 1;
        }
        if (type == id_928 || type == ebp_id) {
          if (flagC) {
            goto next_entry;
          }
          flagC = 1;
        }
        /* 0x007ec09f..0x007ec0be: the club id is recomputed every pass. */
        id_92c = (u32)DAT_009cf92c * 107u + (u32)DAT_00ae23d0;
        cm_fn_006830b0(self, rec, id_92c, 1);
      next_entry:
        idx++;
        cursor += 4u;
        if (!((s32)idx < (s32)count2)) {
          break;
        }
      }
    }

    /* 0x007ec0e3..0x007ec138: hand the top three league entries to the
     * receiver with the club ids of DAT_009cf918 / DAT_009cf91c /
     * DAT_009cf924. */
    entry = cm_fn_006826d0((void *)league,
                            (u32)((s32)(s16)*(s16 *)((u32)league + 0x3e) - 1));
    value = *(u32 *)entry;
    cm_fn_006831a0(self, (u8 *)value, (u32 *)edi_id, 1);

    entry = cm_fn_006826d0((void *)league,
                            (u32)((s32)(s16)*(s16 *)((u32)league + 0x3e) - 2));
    value = *(u32 *)entry;
    cm_fn_006831a0(self, (u8 *)value, (u32 *)ebx_id, 1);

    entry = cm_fn_006826d0((void *)league,
                            (u32)((s32)(s16)*(s16 *)((u32)league + 0x3e) - 3));
    value = *(u32 *)entry;
    cm_fn_006831a0(self, (u8 *)value, (u32 *)ebp_id, 1);
  }

  /* 0x007ebff1 / 0x007ec13d: release the array. */
  cm_fn_009452ca(arr);
  return;
}
