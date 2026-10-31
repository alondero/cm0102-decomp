#include "cmre.h"

extern u32 cm_fn_0053e990(int param_1, int param_2, int param_3, int param_4, int param_5);
extern u32 cm_fn_007e4920(u32 *param_1, u32 *param_2);

/* Thiscall with one stack argument (ret 4) that returns 1. Fields are used by offset:
   +0x174, +0x178, +0x17c are three words passed to the callee at 0x0053e990, +0x180 is a
   signed count N, +0x184 is the address of a field passed as the fifth argument, +0x18c,
   +0x194 and +0x195 are cleared to -1. The global 0x00ae23bc is a base address; a record
   of 110 bytes per count step is addressed as base + 110 * N. */
u32 CM_THISCALL cm_fn_007ae8a0(u8 *param_1, u32 param_2)
{
    s32 n;
    u32 f;
    u32 g;
    u32 h;
    u32 base;
    u32 target;
    u32 found;

    target = 0;
    n = *(s32 *)(param_1 + 0x180);
    if (n > -1) {
        f = *(u32 *)(param_1 + 0x17c);
        g = *(u32 *)(param_1 + 0x178);
        h = *(u32 *)(param_1 + 0x174);
        base = CM_GLOBAL(u32, 0x00ae23bc);
        found = cm_fn_0053e990((int)(base + (u32)n * 110U), (int)h, (int)g, (int)f,
                               (int)(param_1 + 0x184));
        if (found != 0) {
            n = *(s32 *)(param_1 + 0x180);
            base = CM_GLOBAL(u32, 0x00ae23bc);
            target = base + (u32)n * 110U;
        }
    }
    if (param_2 == target) {
        return 1;
    }
    if (param_2 == 0) {
        *(s32 *)(param_1 + 0x180) = -1;
    } else {
        cm_fn_007e4920((u32 *)param_2, (u32 *)(param_1 + 0x174));
    }
    *(s32 *)(param_1 + 0x18c) = -1;
    *(u8 *)(param_1 + 0x194) = 0xffU;
    *(s32 *)(param_1 + 0x195) = -1;
    return 1;
}
