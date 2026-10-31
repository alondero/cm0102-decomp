#include "cmre.h"

u32 CM_CDECL cm_fn_00600e90(u32 self)
{
    u32 best = *(volatile u32 *)(self + 0xd3);
    u32 i;
    if (best)
        return best;
    for (i = 0; i < 5; ++i) {
        u32 item = *(volatile u32 *)(self + 0x19f + i * 4);
        if (item) {
            u32 data = *(volatile u32 *)(item + 0x69);
            if (data) {
                if (best) {
                    u32 old = *(volatile u32 *)(best + 0x69);
                    s16 rank = *(volatile s16 *)(data + 10);
                    if (rank <= *(volatile s16 *)(old + 10))
                        continue;
                }
                best = item;
            }
        }
    }
    return best;
}
