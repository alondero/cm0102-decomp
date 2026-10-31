#include "cmre.h"

/* Same instruction shape as fn_006b19d0 (family peer). Original frame: sub esp,
   0x200. Scratch buffer A is [esp+4] (scratch + 0) and buffer B is [esp+0x104]
   (scratch + 0x100), both in one 0x200-byte frame. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3,
                                      u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

void CM_CDECL cm_fn_006b2080(u32 param_1)
{
    u8 scratch[0x200];

    if (param_1 == 0) {
        /* push esi (x2) carries param_1, which is zero on this branch. */
        (void)cm_fn_00944cff(0x00a15138u, param_1, param_1,
                             scratch + 0x100, scratch);
        /* Pushes 0x60c, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x60cu);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u,
                       (int)param_1);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = param_1;
        return;
    }

    {
        u32 result = cm_fn_007fe600(0x00b74450u, 0x006b2130u, 0x006b2370u,
                                    0u, 0u, 0u);
        if (result != 0) {
            cm_fn_007ff270(0x00b74450u, 0u, param_1, 0u);
        }
    }
}
