/* fn_005b2ae0 (FUN_005b2ae0, formation.cpp): VA 0x005b2ae0, image base 0x00400000.
   ECX is the receiver (read before written), two stack arguments follow (`ret 8`):
   kind (byte at [esp+4]) and arg2 (dword at [esp+8]). Fields are named by offset. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

u16 CM_THISCALL cm_fn_005b2ae0(u32 obj, u8 kind, u32 arg2)
{
    /* One 0x200-byte scratch block: [0x000..0x0ff] is the low buffer (`lea [esp]`),
       [0x100..0x1ff] is the high buffer (`lea [esp+0x100]`). */
    u8 scratch[0x200];
    u32 text;

    /* Signed range check: kind must satisfy 0 <= kind <= 10. */
    if ((s8)kind < 0 || (s8)kind > 10) {
        cm_fn_00944cff(0x009cc3dcu, 0u, 0u, scratch + 0x100, scratch);
        text = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x102u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return 1;
    }

    if (arg2 == 0) {
        return *(volatile u16 *)(obj + 0x12d + (u32)(s32)(s8)kind * 2u);
    }
    return *(volatile u16 *)(obj + 0x14e + (u32)(s32)(s8)kind * 2u);
}
