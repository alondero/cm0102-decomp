/* fn_00716310: thiscall (ECX = object, read before written), no stack arguments (ret 0).
   Preferred-image VA 0x00716310, image base 0x00400000.
   Object fields: 0x19 byte passed as the char argument of 0x005b5270; 0x27 signed byte
   used in the index for the 0x005b5270 object pointer; 0x19e object pointer that is the
   receiver of 0x006c38c0 and the base of the 0x005b5270 receiver. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_005b5270(int param_1, char param_2);
extern u32 CM_THISCALL cm_fn_006c38c0(int param_1, int param_2);

u32 CM_THISCALL cm_fn_00716310(int param_1)
{
  u32 obj;
  u32 r;
  unsigned long long t;
  u32 lo;
  s32 scale;
  u8 ch;

  obj = *(u32 *)(param_1 + 0x19e);
  if (obj != 0u) {
    r = cm_fn_006c38c0((int)obj, param_1);
    if ((u16)r == 1u) {
      goto block_b;
    }
  }

  /* first probe (reads 0x19, 0x27, 0x19e in listing order): receiver = [0x19e] + 6371*sx([0x27]) + 0x91e2, char = [0x19] */
  ch = *(volatile u8 *)(param_1 + 0x19);
  scale = (s32)*(volatile s8 *)(param_1 + 0x27);
  obj = *(volatile u32 *)(param_1 + 0x19e);
  t = cm_fn_005b5270((int)(obj + (u32)scale * 6371u + 0x91e2u), (char)ch);
  lo = (u32)t & 0x80000000u;
  if (lo != 0u) {
    return lo | 3u;
  }

block_b:
  obj = *(u32 *)(param_1 + 0x19e);
  if (obj != 0u) {
    r = cm_fn_006c38c0((int)obj, param_1);
    if ((u16)r == 1u) {
      return r & 0xffffff00u;
    }
  }

  /* second probe (reads 0x27, 0x19, 0x19e in listing order): same receiver and character as the first probe, bit 19 tested */
  scale = (s32)*(volatile s8 *)(param_1 + 0x27);
  ch = *(volatile u8 *)(param_1 + 0x19);
  obj = *(volatile u32 *)(param_1 + 0x19e);
  t = cm_fn_005b5270((int)(obj + (u32)scale * 6371u + 0x91e2u), (char)ch);
  lo = (u32)t & 0x80000u;
  if (lo != 0u) {
    return lo | 4u;
  }
  return 0u;
}
