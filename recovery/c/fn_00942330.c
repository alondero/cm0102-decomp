#include "cmre.h"

u32 CM_CDECL cm_fn_00942330(u32 self)
{
    u32 first = *(volatile u32 *)(self + 0x10u);
    u32 second = *(volatile u32 *)(self + 0x14u);
    u32 result = *(volatile u16 *)(first + 8u);
    if (second != 0) {
        u32 count = *(volatile u32 *)(second + 8u);
        if ((s32)count <= (s32)result)
            result = count - 1u;
    }
    return result;
}
