/* fn_007a56f0 (module unknown): thiscall with no stack arguments (ret 0).
   ECX is the object pointer and is read before any write. Preferred-image VA
   0x007a56f0, image base 0x00400000. */
#include "cmre.h"

#define DAT_00ae2350 CM_GLOBAL(int, 0x00ae2350)

extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *arg4);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);

void CM_THISCALL cm_fn_007a56f0(int *param_1)
{
  int iVar1;
  int iVar2;

  if (param_1[1] != 0) {
    cm_fn_009452ca((u32)param_1[1]);
    param_1[1] = 0;
  }
  if (param_1[4] != 0) {
    cm_fn_009452ca((u32)param_1[4]);
    param_1[4] = 0;
  }
  if (param_1[7] != 0) {
    cm_fn_009452ca((u32)param_1[7]);
    param_1[7] = 0;
  }
  if (param_1[0xf] != 0) {
    cm_fn_009452ca((u32)param_1[0xf]);
    param_1[0xf] = 0;
  }
  if (param_1[0] != 0) {
    iVar2 = 0;
    if (0 < DAT_00ae2350) {
      do {
        /* [[this]+i*4] is re-read on every pass, as in the listing. */
        iVar1 = *(int *)(((u32)param_1[0]) + (u32)iVar2 * 4u);
        if (iVar1 != 0) {
          cm_fn_0068d040((int)iVar1);
          cm_fn_00944c94((void *)(u32)iVar1);
        }
        iVar2 = iVar2 + 1;
      } while (iVar2 < DAT_00ae2350);
    }
    cm_fn_009452ca((u32)param_1[0]);
    param_1[0] = 0;
  }
}
