#include "cmre.h"

u32 CM_CDECL cm_fn_005487b0(u32 arg)
{
    u32 value = arg;
    if (value != 0) {
        value = *(volatile u32 *)(value + 0x53u);
        if (value != 0) {
            u32 reference;
            value = *(volatile u32 *)value;
            reference = CM_GLOBAL(volatile u32, 0x009cf218);
            return (value & 0xffffff00u) | (value != reference);
        }
    }
    return value & 0xffffff00u;
}
