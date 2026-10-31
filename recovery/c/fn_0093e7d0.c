/* fn_0093e7d0 (VA 0x0093e7d0), cdecl: `ret` with no stack pop, two stack arguments
   (object at [esp+4] on entry, count at [esp+8]). Caller pops. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00936840(u32 object);
extern void CM_CDECL cm_fn_0093c5e0(volatile u32 *self);
extern void cm_fn_00944180(int param_1, int param_2);
extern u32 CM_CDECL cm_fn_009445d0(u32 arg1, u32 arg2);

void cm_fn_0093e7d0(u32 self, s32 count)
{
  u32 p;
  u32 value;
  u32 result;

  cm_fn_0093c5e0((volatile u32 *)(*(u32 *)(self + 0xc8)));
  *(u32 *)(self + 0xc8) = 0;
  if (count <= 0) {
    return;
  }
  p = *(u32 *)(self + 4);
  value = *(u32 *)(self + 0xda);
  if (value == *(u32 *)(p + 0x64)) {
    *(u32 *)(p + 0x94) = 1;
  } else {
    cm_fn_009445d0(p, value);
  }
  *(u32 *)(self + 0xda) = 0;
  if (count > 0x63) {
    p = *(u32 *)(self + 4);
    if (*(u32 *)(p + 0x7c) > 0x100u) {
      cm_fn_009445d0(p, *(u32 *)(p + 0x64));
      *(u32 *)(*(u32 *)(self + 4) + 0x64) = 0;
      *(u32 *)(*(u32 *)(self + 4) + 0x7c) = 0;
      *(u32 *)(*(u32 *)(self + 4) + 0x94) = 1;
    }
  }
  value = *(u32 *)(self + 0xde);
  if (value != 0) {
    cm_fn_009445d0(*(u32 *)(self + 4), value);
    *(u32 *)(self + 0xde) = 0;
  }
  if (count > 1) {
    result = cm_fn_00936840(*(u32 *)(self + 0x13d));
    *(u32 *)(self + 0x13d) = 0;
    *(u32 *)(self + 0x12a) = 0xffffffffu;
    *(u32 *)(self + 0x126) = 0xffffffffu;
    if (result != 0) {
      cm_fn_00944180((int)*(u32 *)(self + 4), (int)result);
    }
  }
}
