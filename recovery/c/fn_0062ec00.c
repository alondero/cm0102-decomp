#include "cmre.h"

u32 CM_STDCALL cm_fn_0062ec00(u32 selector)
{
    u32 index = (u32)(s32)(s8)selector;
    u8 value = *(volatile u8 *)(0x009e7ad9u + index * 13u);
    return (index & 0xffffff00u) | value;
}
