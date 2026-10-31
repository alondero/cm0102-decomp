#include "cmre.h"

/* Original frame: sub esp, 0x200. Scratch buffer A is [esp+0] at 0x00669a1e
   and 0x00669a40 (scratch + 0). Scratch buffer B is [esp+0x100] at 0x00669a22
   and 0x00669a40 (scratch + 0x100). Both are adjacent in one 0x200-byte frame. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_CDECL cm_fn_00669a10(u32 param_1)
{
    u8 scratch[0x200];

    if (param_1 == 0) {
        (void)cm_fn_00944cff(0x009eaff4u, param_1, param_1,
                             scratch + 0x100, scratch);
        /* Pushes 0x3d7, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x3d7u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        /* mov eax, [eax]: the int stored at the pointer argument. */
        s32 value = *(volatile s32 *)param_1;

        if (value == CM_GLOBAL(volatile s32, 0x009cf79cu) ||
            value == CM_GLOBAL(volatile s32, 0x009cf77cu) ||
            value == CM_GLOBAL(volatile s32, 0x009cf774u) ||
            value == CM_GLOBAL(volatile s32, 0x009cf770u) ||
            value == CM_GLOBAL(volatile s32, 0x009cf780u) ||
            value == CM_GLOBAL(volatile s32, 0x009cf76cu) ||
            value == CM_GLOBAL(volatile s32, 0x009cf778u)) {
            return 1;
        }
        return 0;
    }
}
