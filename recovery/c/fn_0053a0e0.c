#include "cmre.h"

extern u32 CM_CDECL cm_fn_00946a04(void);
extern long long CM_STDCALL cm_fn_00945e40(long long left, long long right);
extern long long CM_STDCALL cm_fn_00945e80(long long dividend, long long divisor);

u32 CM_CDECL cm_fn_0053a0e0(const u32 *arg)
{
    u32 selected_index = 0;
    if (arg != (const u32 *)0) {
        u32 item_index = CM_GLOBAL(volatile u32, (u32)arg);
        u32 rows = CM_GLOBAL(volatile u32, 0x00ae2338u);
        u32 row = rows + item_index * 0x24u;
        s32 bound = CM_GLOBAL(volatile s32, row + 0x1cu);
        if (bound > 10) {
            s32 row_origin = CM_GLOBAL(volatile s32, row + 4u);
            s8 best = -1;
            u32 last_table = 0;
            for (u32 iteration = 0; iteration < 3u; iteration++) {
                u32 random_value = cm_fn_00946a04();
                long long product = cm_fn_00945e40((long long)(s32)random_value, (long long)bound);
                long long quotient = cm_fn_00945e80(product, 0x8000ll);
                u32 candidate_index = (u32)row_origin + (u32)quotient;
                last_table = CM_GLOBAL(volatile u32, 0x00ae23f4u);
                s8 candidate = CM_GLOBAL(volatile s8, last_table + 0x3bu + candidate_index * 0x3cu);
                if (candidate > best) {
                    best = candidate;
                    selected_index = candidate_index;
                }
            }
            return last_table + selected_index * 0x3cu;
        }
    }

    s32 amount = CM_GLOBAL(volatile s32, 0x00ae239cu);
    if (amount > 0x7fff)
        amount = 0x7fff;
    u32 random_value = cm_fn_00946a04();
    long long product = (long long)(s32)random_value * (long long)amount;
    long long quotient = cm_fn_00945e80(product, 0x8000ll);
    u32 table = CM_GLOBAL(volatile u32, 0x00ae23f4u);
    return table + (u32)quotient * 0x3cu;
}