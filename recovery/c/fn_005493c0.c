#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0062c400(int *param_1, int *param_2);

/* cdecl with one stack argument (ret 0). ECX is pushed and popped but never used as an
   input, so this is not a thiscall. The counter is one byte that shares its address with
   the saved-ECX slot; the listing writes it as `esp+7` before the null test and reads it
   as `esp+0x13` after the loop. Returns a byte: 1 when the counter is greater than 4. */
u8 CM_CDECL cm_fn_005493c0(u8 *param_1)
{
    u8 count;
    long long product;
    s32 hi;
    s32 t;
    s16 lim;
    u32 ebp_ptr;
    u32 q;
    u32 r;
    u32 r2;
    u32 ret;
    int k;

    count = 0;
    if (param_1 == 0) {
        return 0;
    }
    product = (long long)(s32)0x7e07e07fU * (long long)(s32)*(s16 *)(param_1 + 0x80);
    hi = (s32)((unsigned long long)product >> 32);
    hi = hi >> 5;
    t = hi + (s32)((u32)hi >> 31) + 0x28;
    lim = (s16)t;
    if (lim < 0x28) {
        lim = 0x28;
    } else if (lim > 0x8c) {
        lim = 0x8c;
    }
    ebp_ptr = (u32)param_1 + 0xd7U;
    for (k = 0; k < 0x32; k++) {
        q = *(u32 *)ebp_ptr;
        if (q != 0) {
            r = *(u32 *)(q + 0x61);
            if (r != 0) {
                ret = cm_fn_0062c400((int *)CM_GLOBAL(u32, 0x00b63d28), (int *)q);
                if ((u8)ret != 2) {
                    if ((s32)*(s32 *)q < (s32)CM_GLOBAL(s32, 0x00ae2364)) {
                        r2 = *(u32 *)(q + 0x61);
                        if ((s16)*(s16 *)(r2 + 5) > lim) {
                            if ((s8)*(s8 *)(r2 + 0x44) > 0x0e) {
                                count++;
                            }
                        }
                    }
                }
            }
        }
        ebp_ptr += 4U;
    }
    return ((s8)count > 4) ? 1 : 0;
}
