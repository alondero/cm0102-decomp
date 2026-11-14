/* Plain C translation of fn_00942350 (VA 0x00942350, 526 bytes).
   Checked statement by statement against the listing in `cmre brief`. */
#include "cmre.h"

extern u32 *CM_CDECL cm_fn_0093bd70(u32 object_arg, u32 unused_arg, s16 first_value, s16 second_value, s16 third_value);
extern u32 * cm_fn_0093be40(int param_1, u32 param_2, u32 param_3, short param_4, short param_5, short param_6, short param_7, u32 param_8);
extern u32 * cm_fn_00941630(int param_1, u32 *param_2, int param_3, int param_4, int param_5, int *param_6);
extern void CM_CDECL cm_fn_00941760(u8 *object, u32 *output);
extern u32 CM_CDECL cm_fn_00942560(u32 allocator, u32 input, u32 copy, u32 a4, u32 a5, u32 a6, u32 a7);

typedef void (CM_CDECL *cm_callback_t)(u32 arg1, u32 arg2, u32 arg3, u32 arg4);

#define FIELD(off) (*(u32 *)((u8 *)param_1 + (off)))

int cm_fn_00942350(int *param_1, int param_2, u32 param_3, u16 *param_4, u16 *param_5)
{
  u32 *dir;
  u32 **table;
  s32 count;
  s32 idx;
  u32 *entry;
  u32 f20;
  u32 ranges;
  u32 lo;
  u32 hi;
  u32 delta;
  u16 v_hi_word;
  u16 v_lo_word;
  u32 v_second;
  u32 v_first;
  u32 *obj;
  u32 *out;
  u32 cb_arg3;

  *param_4 = 0;
  *param_5 = 0;

  dir = *(u32 **)param_1;
  count = (s32)*(s16 *)((u8 *)dir + 8);
  entry = 0;
  idx = 0;
  if (count > 0) {
    table = *(u32 ***)((u8 *)dir + 0x10);
    do {
      u32 *e = table[idx];
      if (*(u32 *)((u8 *)e + 4) == 0x676c7966u) {
        entry = e;
        break;
      }
      idx = idx + 1;
    } while (idx < count);
  }

  if ((entry != 0) && (FIELD(0x14) != 0)) {
    f20 = FIELD(0x20);
    if ((f20 != 0) || (FIELD(0x24) != 0)) {
      if ((param_2 >= 0) && (param_2 < (s32)FIELD(0x64))) {
        ranges = *(u32 *)((u8 *)FIELD(0x14) + 4);
        lo = *(u32 *)((u8 *)ranges + (u32)(param_2 * 4));
        hi = *(u32 *)((u8 *)ranges + (u32)(param_2 * 4) + 4);
        delta = hi - lo;
        v_hi_word = 0;
        v_lo_word = 0;
        if (f20 != 0) {
          u32 *a = *(u32 **)((u8 *)f20 + 0x10);
          u32 *b = *(u32 **)((u8 *)f20 + 0x0c);
          v_hi_word = *(u16 *)((u8 *)a + param_2 * 2);
          v_lo_word = *(u16 *)((u8 *)b + param_2 * 2);
        } else {
          v_hi_word = 0x800;
          v_lo_word = 0;
        }
        v_first = 0;
        v_second = 0;
        if (FIELD(0x24) != 0) {
          u32 *a = *(u32 **)((u8 *)FIELD(0x24) + 0x10);
          u32 *b = *(u32 **)((u8 *)FIELD(0x24) + 0x0c);
          v_first = *(u16 *)((u8 *)a + param_2 * 2);
          v_second = *(u16 *)((u8 *)b + param_2 * 2);
        } else {
          u32 *f = (u32 *)FIELD(0x18);
          if (f != 0) {
            u16 d = (u16)(*(u16 *)((u8 *)f + 0x0c) - *(u16 *)((u8 *)f + 0x0a) +
                           *(u16 *)((u8 *)f + 0x08));
            v_first = d;
            v_second = (u32)d / 10u;
          }
        }

        if (hi > lo) {
          u32 a6 = 0;
          int a5 = 2;
          int a4 = (int)delta;
          int a3;
          u32 a2;
          int a1;
          u32 r3 = *(u32 *)((u8 *)entry + 0x0c);
          a3 = (int)(r3 + lo);
          a2 = FIELD(0x54);
          a1 = (int)FIELD(0x5c);
          obj = cm_fn_00941630(a1, (u32 *)a2, a3, a4, a5, (int *)a6);
          if (*(u16 *)((u8 *)FIELD(0x0c) + 0x38) == 2000) {
            u32 a8 = FIELD(0x60);
            u32 a5b = v_hi_word;
            u32 a4b = v_lo_word;
            u32 a1b = FIELD(0x5c);
            out = cm_fn_0093be40((int)a1b, (u32)obj, param_3, (short)a4b, (short)a5b,
                                 (short)v_second, (short)v_first, a8);
          } else {
            u32 a1b = FIELD(0x5c);
            out = (u32 *)cm_fn_00942560(a1b, (u32)obj, param_3, v_lo_word, v_hi_word,
                                        v_second, v_first);
          }
          cm_fn_00941760((u8 *)obj, 0);
          if (FIELD(0x3c) != 0) {
            cm_callback_t cb;
            u32 tmp = FIELD(0x0c);
            cb_arg3 = 0x800;
            if (tmp != 0) {
              cb_arg3 = *(u16 *)((u8 *)tmp + 0x16);
            }
            cb = (cm_callback_t)FIELD(0x3c);
            cb((u32)out, FIELD(0x5c), cb_arg3, (u32)((u8 *)param_1 + 0x44));
          }
        } else {
          u32 a1 = FIELD(0x5c);
          out = cm_fn_0093bd70(a1, v_lo_word, (s16)v_hi_word, (s16)v_second,
                               (s16)v_first);
        }
        *param_4 = v_hi_word;
        *param_5 = (u16)v_first;
        *(u16 *)((u8 *)out + 0x71) = (u16)param_2;
        return (int)(u32)out;
      }
    }
  }

  {
    u32 a1 = FIELD(0x5c);
    u32 *r = cm_fn_0093bd70(a1, 0, 0, 0, 0);
    *(u16 *)((u8 *)r + 0x71) = (u16)param_2;
    return (int)(u32)r;
  }
}
