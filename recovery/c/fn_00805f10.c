/* fn_00805f10: cdecl, two stack arguments (ret 0, caller cleans up).
   Preferred-image VA 0x00805f10, image base 0x00400000. */
#include "cmre.h"

extern u32 cm_fn_00806260(char *param_1, char *param_2, int param_3);

u32 CM_CDECL cm_fn_00805f10(int param_1, int param_2)
{
  u32 p;

  if (param_1 == 0 || param_2 == 0) {
    return 0;
  }

  /* Offsets 0x04, 0x1f, 0x23 and 0x3d: a null field is skipped, a nonzero
     callee result returns 1 at once. */
  p = (u32)param_1 + 0x04u;
  if (p != 0u && cm_fn_00806260((char *)p, (char *)(u32)param_2, 0) != 0u) {
    return 1;
  }
  p = (u32)param_1 + 0x1fu;
  if (p != 0u && cm_fn_00806260((char *)p, (char *)(u32)param_2, 0) != 0u) {
    return 1;
  }
  p = (u32)param_1 + 0x23u;
  if (p != 0u && cm_fn_00806260((char *)p, (char *)(u32)param_2, 0) != 0u) {
    return 1;
  }
  p = (u32)param_1 + 0x3du;
  if (p != 0u && cm_fn_00806260((char *)p, (char *)(u32)param_2, 0) != 0u) {
    return 1;
  }

  /* Offset 0xa3 is the last check: a null field returns 0 directly, and the
     call result is returned as 1 or 0. */
  p = (u32)param_1 + 0xa3u;
  if (p == 0u) {
    return 0;
  }
  return cm_fn_00806260((char *)p, (char *)(u32)param_2, 0) != 0u ? 1u : 0u;
}
