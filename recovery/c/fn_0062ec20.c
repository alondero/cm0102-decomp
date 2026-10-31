#include "cmre.h"

/* Original frame: stack argument loaded from [esp+4] before `sub esp, 0x200`.
   One 0x200-byte scratch allocation: [esp] = scratch + 0x000, and
   [esp + 0x100] = scratch + 0x100 (both lea'd from the same base). */

extern int CM_THISCALL cm_fn_0058f120(u32 *param_1, int param_2, int param_3);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

/* ECX = this (param_1); stack argument = param_2 (pointer to a 4-byte index). */
int CM_THISCALL cm_fn_0062ec20(int *param_1, int *param_2)
{
    u8 scratch[0x200];

    if (param_2 == (int *)0) {
        u32 ret_0090d130;

        (void)cm_fn_00944cff(0x009e7f28u, 0u, 0u, scratch + 0x100, scratch);
        /* Pushes 0xd88, edx and eax before this call are not consumed by
           cm_fn_0090d130 (void); they are the varargs of cm_fn_00944c9f. */
        ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0xd88u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return 0;
    }

    {
        u32 recv;
        u32 idx;
        u32 base;
        u32 addr;
        u32 v18;
        u32 v14;

        /* mov ecx, [this + 0xc]; the receiver for the thiscall below. */
        recv = *(volatile u32 *)((u8 *)param_1 + 0xc);
        if (recv == 0) {
            return 0;
        }
        idx = *(volatile u32 *)param_2;
        base = *(volatile u32 *)param_1;
        addr = base + idx * 31u;
        v18 = *(volatile u32 *)(addr + 0x18u);
        v14 = *(volatile u32 *)(addr + 0x14u);
        return cm_fn_0058f120((u32 *)recv, (int)v14, (int)v18);
    }
}
