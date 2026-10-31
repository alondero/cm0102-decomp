/* fn_0084b4d0 (FUN_0084b4d0, shortlist_manager.cpp): VA 0x0084b4d0, image base 0x00400000.
   ECX is loaded from the first stack argument before any read, so this is not thiscall:
   stdcall with two stack arguments (`ret 8`). Fields are named by offset. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u32 CM_STDCALL cm_fn_0084b4d0(u32 obj, u8 kind)
{
    /* One 0x200-byte scratch block: [0x000..0x0ff] is the low buffer (`lea [esp]`),
       [0x100..0x1ff] is the high buffer (`lea [esp+0x100]`). */
    u8 scratch[0x200];
    u32 text;

    if (*(volatile u32 *)(obj + 0x61) == 0) {
        cm_fn_00944cff(0x00a825a4u, 0u, 0u, scratch + 0x100, scratch);
        text = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x2de0u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 0;
    }
    if (kind != 6 && kind != 5) {
        return 1;
    }
    return (s8)(*(volatile u8 *)(obj + 0x18)) < 0x18;
}
