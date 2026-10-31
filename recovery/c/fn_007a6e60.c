#include "cmre.h"

/* Callee 0x007a7150 pops its one stack argument (ret 4) and does not read ECX. The caller
   loads ECX with the receiver before the call, so the boundary keeps that register value
   as an explicit first (thiscall) parameter. */
extern u32 CM_THISCALL cm_fn_007a7150(u32 receiver, int *param_1);

/* Thiscall with one stack argument (ret 4). param_1 is the receiver in ECX, param_2 the
   stack argument. The scan count is global 0x00ae2364 minus 16; the table base is global
   0x00ae23bc and each row is 0x6e bytes. A row counts only when the pointer at row+0x1a is
   non-null and the dword it points to equals param_2. Returns the best row pointer, or 0. */
u32 CM_THISCALL cm_fn_007a6e60(u32 param_1, u32 param_2)
{
    s32 count;
    u32 base;
    u32 best;
    s32 best_value;
    s32 index;
    s32 result;
    u32 q;

    best = 0;
    index = 0;
    best_value = (s32)0xfffffc18U;
    count = (s32)(CM_GLOBAL(u32, 0x00ae2364) - 0x10U);
    base = CM_GLOBAL(u32, 0x00ae23bc);
    if (count > 0) {
        do {
            q = *(u32 *)(base + 0x1a);
            if (q != 0 && *(u32 *)q == param_2) {
                result = (s32)cm_fn_007a7150(param_1, (int *)base);
                if (result > best_value && result > 0) {
                    best_value = result;
                    best = base;
                }
            }
            index++;
            base += 0x6e;
            count = (s32)(CM_GLOBAL(u32, 0x00ae2364) - 0x10U);
        } while (index < count);
    }
    if (best == 0) {
        base = CM_GLOBAL(u32, 0x00ae23bc);
        count = (s32)(CM_GLOBAL(u32, 0x00ae2364) - 0x10U);
        index = 0;
        best_value = (s32)0xffffc568U;
        if (count > 0) {
            do {
                result = (s32)cm_fn_007a7150(param_1, (int *)base);
                if (result > best_value && result > 0) {
                    best_value = result;
                    best = base;
                }
                index++;
                base += 0x6e;
            } while (index < (s32)(CM_GLOBAL(u32, 0x00ae2364) - 0x10U));
        }
    }
    return best;
}
