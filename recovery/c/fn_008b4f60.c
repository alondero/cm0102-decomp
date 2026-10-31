/* WIP: outcome mismatch, case 0 (original faults, C returns). Blocker: the fild/fmul
   products are passed to 0x00945640 in ST(0), which plain C cannot express; see
   docs/agent/C-BOUNDARIES.md (x87 transport needs parent-owned original_x87 hints).
   fn_008b4f60: thiscall (ECX = object, read before written), no stack arguments, ret 0.
   Preferred-image VA 0x008b4f60, image base 0x00400000. */
#include "cmre.h"

extern long long cm_fn_00945640(void);

int CM_THISCALL cm_fn_008b4f60(int param_1)
{
  s16 sVar1;
  s16 sVar2;
  s16 sVar3;
  s16 sVar4;
  s16 sVar5;
  int iVar6;

  (void)param_1;
  /* Each call consumes the value that the preceding fild/fmul left in ST(0). */
  sVar1 = (s16)cm_fn_00945640();
  sVar2 = (s16)cm_fn_00945640();
  sVar3 = (s16)cm_fn_00945640();
  sVar4 = (s16)cm_fn_00945640();
  sVar5 = (s16)cm_fn_00945640();
  sVar5 = (s16)((((s32)sVar2 - (s32)sVar1) - (s32)sVar3) - (s32)sVar4 - (s32)sVar5);
  iVar6 = (int)sVar5;
  if (99 < sVar5) {
    iVar6 = 100;
  }
  return iVar6;
}
