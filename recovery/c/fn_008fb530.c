#include "cmre.h"

/* fn_008fb530: cdecl, two stack arguments (ret 0), no ECX read before write.
   Preferred-image VA 0x008fb530, image base 0x00400000.
   Every thiscall in this routine passes the object 0x00b74450 in ECX. */

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

void cm_fn_008fb530(u32 param_1, s8 param_2)
{
    if (cm_fn_007fe600(0x00b74450u, 0x008fb6a0u, 0x008fb9e0u, 1u, 0u, 0u) != 0u) {
        cm_fn_007ff270(0x00b74450, 0, (char *)param_1, 0);
        cm_fn_007ff270(0x00b74450, 1, (char *)(s32)param_2, 0);
        cm_fn_007ff270(0x00b74450, 2, (char *)0xffffffffu, 0);
        cm_fn_007ff270(0x00b74450, 3, (char *)0xffffffffu, 0);
        cm_fn_007ff270(0x00b74450, 4, (char *)0xffffffffu, 0);
        cm_fn_007ff270(0x00b74450, 5, (char *)0xffffffffu, 0);
        cm_fn_007ff270(0x00b74450, 6, (char *)0xffffffffu, 0);
        cm_fn_007ff270(0x00b74450, 7, (char *)0, 0);
    }
}
