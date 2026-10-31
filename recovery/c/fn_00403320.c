#include "cmre.h"

u32 CM_THISCALL cm_fn_00403320(u32 self, u32 arg)
{
    u32 limit = *(volatile u32 *)(self + 0x14u);
    u32 byte = *(volatile u8 *)(self + 0xbacu);
    u32 result;
    limit = limit - byte + 1u;
    result = (s32)arg < (s32)limit ? arg : limit;
    *(volatile u32 *)(self + 0x1fcu) = (s32)result > 0 ? result : 0;
    return result;
}
