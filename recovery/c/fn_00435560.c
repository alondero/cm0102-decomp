/* Preferred-image VA 0x00435560, image base 0x00400000.
   Reconstructed from the original listing only. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00435580(u32 self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);

u32 CM_THISCALL cm_fn_00435560(u32 param_1,u8 param_2)
{
  cm_fn_00435580(param_1);
  if ((param_2 & 1) != 0) {
    cm_fn_00944c94((void *)(param_1));
  }
  return param_1;
}
