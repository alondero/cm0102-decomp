/* fn_008f2e10: cdecl, three stack arguments (ret 0, caller cleans up; the early-return path
   does not save esi). Every thiscall in this routine passes the object 0x00b74450 in ECX.
   Preferred-image VA 0x008f2e10, image base 0x00400000. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

void cm_fn_008f2e10(int *param_1, u32 param_2, u32 param_3)
{
  u32 eax;

  eax = cm_fn_007fe600(0x00b74450u, 0x008f2ec0u, 0x008f3c70u, 1u, 0u, 0u);
  if (eax != 0u) {
    cm_fn_007ff270(0x00b74450, 0, (char *)param_1, 0);
    cm_fn_007ff270(0x00b74450, 1, (char *)param_2, 0);
    cm_fn_007ff270(0x00b74450, 2, (char *)param_3, 0);
    cm_fn_007ff270(0x00b74450, 3, (char *)0, 0);
    cm_fn_007ff270(0x00b74450, 4, (char *)(*(u32 *)((u32)param_1[0] + 0x24u)), 0);
    cm_fn_007ff270(0x00b74450, 5, (char *)(*(u32 *)((u32)param_1[0] + 0x0cu)), 0);
    cm_fn_007ff270(0x00b74450, 6, (char *)0, 0);
  }
}
