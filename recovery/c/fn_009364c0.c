#include "cmre.h"

u32 CM_CDECL cm_fn_009364c0(u32 self, u32 source, u32 value)
{
    u32 i;
    if (!self || *(volatile u32 *)self != 0xa5a0f5a5 ||
        *(volatile u32 *)(self + 0x44c) != 0x0fa55af0)
        return 0xffffffff;
    *(volatile u32 *)(self + 0x2a6) = value;
    if (source) {
        for (i = 0; i < 0x1b; ++i) {
            u32 word = *(volatile u32 *)(source + i * 4);
            *(volatile u32 *)(self + 0x34e + i * 4) = word;
        }
    }
    *(volatile u8 *)(self + 0x443) = 1;
    return 0;
}
