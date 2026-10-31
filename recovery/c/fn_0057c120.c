/* fn_0057c120 (FUN_0057c120, euro_champ.cpp): VA 0x0057c120, image base 0x00400000,
   thiscall receiver in ECX, one stack argument, `ret 4`.
   Fields are named by offset from the receiver. */
#include "cmre.h"

extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);

void CM_THISCALL cm_fn_0057c120(u32 obj, u32 arg1)
{
    /* One 0x200-byte scratch block: [0x000..0x0ff] is the low buffer (`lea [esp]`),
       [0x100..0x1ff] is the high buffer (`lea [esp+0x100]`). */
    u8 scratch[0x200];
    s16 ax;
    u32 base1;
    u32 base2;
    s16 ax2;
    u32 text;

    ax = *(volatile s16 *)(obj + 0x36);
    if (ax >= *(volatile s16 *)(obj + 0x38)) {
        cm_fn_00944cff(0x009c4638u, 0u, 0u, scratch + 0x100, scratch);
        text = (u32)cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text,
                       (u32)(scratch + 0x100), (u32)scratch, 0x365u);
        cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        return;
    }

    /* Record of 6 bytes at base + 6*index: byte +4 is 6, dword +0 is arg1. */
    base1 = *(volatile u32 *)(obj + 0x14);
    *(volatile u8 *)(base1 + (u32)(s32)ax * 6u + 4u) = 6;
    ax2 = *(volatile s16 *)(obj + 0x36);
    base2 = *(volatile u32 *)(obj + 0x14);
    *(volatile u32 *)(base2 + (u32)(s32)ax2 * 6u) = arg1;
    *(volatile u16 *)(obj + 0x36) = (u16)(*(volatile u16 *)(obj + 0x36) + 1u);
}
