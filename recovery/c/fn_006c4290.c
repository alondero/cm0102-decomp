/* fn_006c4290 - match_eng.cpp, preferred-image VA 0x006c4290 (image base 0x00400000).
   Translation of the original listing only; widths, signedness and call order
   follow the original instructions. */
#include "cmre.h"

#define DAT_00ae24d0 CM_GLOBAL(u8, 0x00ae24d0)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_CDECL cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern int CM_THISCALL cm_fn_006ceb90(int param_1, int param_2, int param_3, int param_4);
extern unsigned long long CM_THISCALL cm_fn_006f0630(u32 self);
extern void CM_THISCALL cm_fn_006f46b0(int param_1, char param_2, char param_3);
extern void CM_THISCALL cm_fn_007160e0(void *self, u32 arg1);
extern char *cm_fn_0090d130(char *arg1, char *arg2, int arg3);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);



static u32 cm_read_u32le(const u8 *p)
{
  return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static void cm_write_u16le(u8 *p, u16 v)
{
  p[0] = (u8)v;
  p[1] = (u8)(v >> 8);
}

u32 CM_FASTCALL cm_fn_006c4290(u8 *self)
{
  u8 buf_100[256];
  u8 buf_200[256];
  u8 *rec;
  u8 *base;
  s32 sel;
  u32 outer;
  u32 counter;
  u32 result;
  s8 v87;

  /* 006c429c: al = self[0x8eae]; ebx = (s8)al */
  sel = (s32)(s8)self[0x8eae];
  /* 006c42ad: eax = 27 * ebx; byte at self[0xf5c8 + 2*eax] is incremented */
  base = self + 0xf5c8 + 54 * sel;
  *base = (u8)(*base + 1);

  /* 006c42c5: clamp self[0x8ea7] into [0,8] with signed byte compares */
  if ((s8)self[0x8ea7] < 0) {
    self[0x8ea7] = 0;
  }
  if ((s8)self[0x8ea7] > 8) {
    self[0x8ea7] = 8;
  }
  /* 006c42e8: clamp self[0x8ea8] into [0,11] */
  if ((s8)self[0x8ea8] < 0) {
    self[0x8ea8] = 0;
  }
  else if ((s8)self[0x8ea8] > 11) {
    self[0x8ea8] = 11;
  }

  /* 006c4306: cm_fn_006ceb90(self, (char)self[0x8eae], 2, 0) */
  result = (u32)cm_fn_006ceb90((int)(u32)self, (int)(u32)self[0x8eae], 2, 0);

  /* 006c431a: two passes over 20 records of stride 0x1be from self + 0x4796 */
  rec = self + 0x4796;
  for (outer = 0; outer < 2; outer++) {
    for (counter = 0x14; counter != 0; counter--) {
      u8 v19 = rec[0x19];
      if ((s8)v19 >= 0) {
        u32 same = ((s32)sel == (s32)outer) ? 1u : 0u;
        u32 flag = (rec[0x27] == 1) ? 1u : 0u;
        u32 table = cm_read_u32le(self + 0xf56e + 4 * flag);
        u32 e = 9u * (same + 2u * (u32)(s32)(s8)v19) + (u32)(s32)(s8)self[0x8ea7];
        u32 addr = table + 12u * e + (u32)(s32)(s8)self[0x8ea8];
        u8 b = *(u8 *)(u32)addr;
        /* 006c4372: cm_fn_006f46b0(rec, (char)(b & 0xf), (char)(b >> 4)) */
        cm_fn_006f46b0((int)(u32)rec, (char)(b & 0xf), (char)(b >> 4));
        /* 006c4382: cm_fn_007160e0(rec, 0) */
        cm_fn_007160e0((void *)(u32)rec, 0);
      }
      rec += 0x1be;
    }
  }

  if (result != 0) {
    u8 a87 = self[0x8ea7];
    u8 a88 = self[0x8ea8];
    u32 rv;
    /* 006c43b8: cm_fn_006f46b0(result, (char)self[0x8ea7], (char)self[0x8ea8]) */
    cm_fn_006f46b0((int)result, (char)a87, (char)a88);
    /* 006c43c1: cm_fn_006f0630(result) */
    cm_fn_006f0630((u32)result);
    /* 006c43c8: re-read self[0x8ea7], then store 0x19c in listing order */
    v87 = (s8)self[0x8ea7];
    cm_write_u16le((u8 *)(u32)result + 0x19c, 0xc8);
    /* 006c43d7..006c43e1: eax = (self[0x8ea7] != 0) ? 0x10e : 0x5a; 006c43e4 stores the low word */
    rv = ((v87 != 0) ? 0xb4u : 0u) + 0x5au;
    cm_write_u16le((u8 *)(u32)result + 0x19a, (u16)rv);
    return rv;
  }
  else {
    char *r90;
    u32 diag;
    /* 006c43f5: cm_fn_00944cff(0x00a15884, 0, 0, buf_100, buf_200) */
    (void)cm_fn_00944cff(0x00a15884u, 0u, 0u, (void *)buf_100, (void *)buf_200);
    /* 006c4417: cm_fn_0090d130(buf_100, buf_200, 0x23b1) */
    r90 = cm_fn_0090d130((char *)buf_100, (char *)buf_200, 0x23b1);
    /* 006c4413: cm_fn_00944c9f(0x00ae24d0, 0x009870e8, r90, buf_100, buf_200, 0x23b1) */
    (void)cm_fn_00944c9f((u32)(u32)&DAT_00ae24d0, 0x009870e8u, (u32)(u32)r90,
                         (u32)(u32)buf_100, (u32)(u32)buf_200, 0x23b1u);
    /* 006c443a: cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0); 006c4465 ret forwards its EAX */
    diag = cm_fn_005e8290((const char *)0x009870e0u, (const char *)(u32)&DAT_00ae24d0, 0);
    DAT_00b67a34 = 0;
    self[0] = 1;
    return diag;
  }
}
