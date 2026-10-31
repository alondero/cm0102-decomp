#include "cmre.h"
extern u32 CM_THISCALL cm_fn_004cca30(u32 self, volatile u32 key);

u32 CM_THISCALL cm_fn_008c5450(u32 self, u32 record, u32 *out)
{
    u32 index = *(volatile u32 *)record;
    u32 base = *(volatile u32 *)(self + 0x86e);
    u32 result;
    if (!(*(volatile u8 *)(base + index * 8) & 0x40))
        return 0;
    result = cm_fn_004cca30(CM_GLOBAL(volatile u32, 0x00ae1050), index);
    if (result) {
        if (out) {
            index = *(volatile u32 *)(result + 4);
            base = CM_GLOBAL(volatile u32, 0x00ae23b4);
            *(volatile u32 *)out = base + index * 581;
        }
        return 1;
    }
    index = *(volatile u32 *)record;
    base = *(volatile u32 *)(self + 0x86e);
    *(volatile u8 *)(base + index * 8) = *(volatile u8 *)(base + index * 8) & 0xbf;
    return 0;
}
