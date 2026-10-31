#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch A is [esp] at 0x00870672 (scratch + 0).
   Scratch B is [esp + 0x100] at 0x00870676 (scratch + 0x100). Both are one
   0x200-byte frame. The pushes at 0x0087069b (0x1ac), 0x008706a0 (A) and
   0x008706a1 (B) are not consumed by cm_fn_0090d130 (void); they are the
   varargs of the following cm_fn_00944c9f call.
   Both exits leave the last callee's EAX in place (branch 1: cm_fn_005e8290;
   branch 2: cm_fn_007fe600), so the return value is that EAX. */

extern u32 CM_THISCALL cm_fn_007f7920(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2, u32 arg3,
                                      u32 arg4, u32 arg5);
extern u32 CM_CDECL cm_fn_005e8290(const char *message, const char *detail,
                                  int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_CDECL cm_fn_00870650(u32 param_1)
{
    u8 scratch[0x200];

    if (cm_fn_007f7920(CM_GLOBAL(u32, 0x00b74448u), 1u) == 0 && param_1 == 0) {
        (void)cm_fn_00944cff(0x00a84aacu, 0u, 0u, scratch + 0x100, scratch);
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x1acu);
        u32 result = cm_fn_005e8290((const char *)0x009870e0u,
                                    (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return result;
    }

    return cm_fn_007fe600(0x00b74450u, 0x00876520u, 0x00889c30u, 0u,
                          0x0088b8a0u, 0u);
}
