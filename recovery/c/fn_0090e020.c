#include "cmre.h"

/* Stdcall, one stack argument arg_1 (ret 4). Object pointer P = dword at arg_1+0x61.
   P == 0 returns 0xff. Otherwise the routine compares signed bytes at P+offset with
   15 (cl = 0x0f; jl/jge/setl/setge are signed). Several results are written to al
   only, so the upper 24 bits of P survive into the return value; that is the
   original behaviour and is kept as hi = P & 0xffffff00. */
#define OBJ_S8(off) (*(volatile s8 *)(p + (off)))

u32 CM_STDCALL cm_fn_0090e020(u32 arg_1)
{
    u32 p;
    u32 hi;

    p = *(volatile u32 *)(arg_1 + 0x61u);
    if (p == 0u) {
        return 0xffu;
    }
    hi = p & 0xffffff00u;

    if (OBJ_S8(0x0f) < 15) {
        goto L_03e;
    }
    return hi;

L_03e:
    if (OBJ_S8(0x11) >= 15) {
        goto L_07b;
    }
    if (OBJ_S8(0x10) >= 15) {
        goto L_07b;
    }
    if (OBJ_S8(0x12) >= 15) {
        goto L_065;
    }
    if (OBJ_S8(0x13) >= 15) {
        goto L_065;
    }
    if (OBJ_S8(0x14) >= 15) {
        goto L_065;
    }
    /* setl al; dec eax; and eax,8; dec eax */
    if (OBJ_S8(0x15) < 15) {
        return 0xffffffffu;
    }
    return 7u;

L_065:
    if (OBJ_S8(0x18) < 15) {
        goto L_06f;
    }
    return hi | 4u;

L_06f:
    /* setge al; add eax,5 */
    if (OBJ_S8(0x17) >= 15) {
        return hi + 6u;
    }
    return hi + 5u;

L_07b:
    if (OBJ_S8(0x18) < 15) {
        goto L_085;
    }
    return hi | 1u;

L_085:
    /* setl al; add eax,2 */
    if (OBJ_S8(0x17) < 15) {
        return hi + 3u;
    }
    return hi + 2u;
}
