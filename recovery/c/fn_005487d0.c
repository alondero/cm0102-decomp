#include "cmre.h"

u32 CM_CDECL cm_fn_005487d0(u32 arg)
{
    u32 value = arg;
    if (value != 0) {
        value = *(volatile u32 *)(value + 0x53u);
        if (value != 0) {
            value = *(volatile u32 *)value;
            if (value != CM_GLOBAL(volatile u32, 0x009cf218) &&
                value != CM_GLOBAL(volatile u32, 0x009cf300) &&
                value != CM_GLOBAL(volatile u32, 0x009cf2ac))
                return (value & 0xffffff00u) | 1u;
        }
    }
    return value & 0xffffff00u;
}
