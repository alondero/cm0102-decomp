#include "cmre.h"

/* Original frame: sub esp, 0x200; push esi. Scratch buffer A is [esp+4] at
   0x008976b1 (scratch + 0). Scratch buffer B is [esp+0x104] at 0x008976b5
   (scratch + 0x100). Both are adjacent in one 0x200-byte frame. */

extern void cm_fn_005e8290(const char *message, const char *detail, int flags);
/* Listing sets ECX = esi (this) before `call 0x8982b0`; the callee pops its
   two stack arguments (ret 8), so this is a thiscall with the object first. */
extern int CM_THISCALL cm_fn_008982b0(u32 self, int *arg1, int arg2);
/* No stack arguments: pushes made before this call are the varargs of the
   following cm_fn_00944c9f call. */
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                  void *arg5);

u32 CM_THISCALL cm_fn_008976a0(u32 self, u32 arg1, u32 arg2)
{
    u8 scratch[0x200];

    if (arg1 == 0) {
        (void)cm_fn_00944cff(0x00a8b908u, 0, 0, scratch + 0x100, scratch);
        /* Pushes 0x527, edx and eax are not consumed by cm_fn_0090d130 (void).
           They are the varargs of the following cm_fn_00944c9f call. */
        u32 ret_0090d130 = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, ret_0090d130,
                             scratch + 0x100, scratch, 0x527u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0;
        return 0;
    }

    {
        int index = cm_fn_008982b0(self, (int *)arg1, (int)arg2);
        if (index == -1) {
            return 0;
        }
        return *(volatile u32 *)(*(volatile u32 *)(self + 0x601) +
                                 (u32)index * 0x91u + 0x52u);
    }
}
