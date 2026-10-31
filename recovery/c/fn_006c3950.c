#include "cmre.h"

/* Callee receivers: both calls set ECX to the object before the call.
   The 0x006f09b0 stack arguments are full dwords (push ebp = param_4, then push edi = param_3). */
extern long double CM_THISCALL cm_fn_006f09b0(u32 self, u32 param_3, u32 param_4);
extern u32 CM_FASTCALL cm_fn_00717410(u32 self);

long double CM_THISCALL
cm_fn_006c3950(u32 self, s8 param_2, u32 param_3, u32 param_4, s8 param_5)
{
  float local_4;
  s8 cVar2;
  u32 iVar3;
  u32 uVar1;
  long double fVar4;

  local_4 = 5000.0f;
  cVar2 = 0;
  iVar3 = self + 0x4796u + (u32)param_2 * 0x22d8u;
  do {
    if (*(s8 *)(iVar3 + 0x19) >= 0) {
      uVar1 = cm_fn_00717410(iVar3);
      if (((uVar1 & 1) == 0) &&
          ((param_5 != 0) || (*(s8 *)(iVar3 + 0x2b) == 0))) {
        fVar4 = cm_fn_006f09b0(iVar3, param_3, param_4);
        /* 006c39b6 tests only C3: equal or unordered returns; below continues. */
        long double zero = (long double)CM_GLOBAL(double, 0x009689a0);
        if (!(fVar4 < zero || fVar4 > zero)) {
          return (long double)CM_GLOBAL(float, 0x00968970);
        }
        if (fVar4 < (long double)local_4) {
          local_4 = (float)fVar4;
        }
      }
    }
    cVar2 = cVar2 + 1;
    iVar3 = iVar3 + 0x1beu;
  } while (cVar2 < 0x14);
  return (long double)local_4;
}
