#include "cmre.h"

u32 CM_CDECL cm_fn_00943d50(u32 arg)
{
    u32 pointer = *(volatile u32 *)(arg + 0x10u);
    s16 first = *(volatile s16 *)(pointer + 0xau);
    s16 second = *(volatile s16 *)(pointer + 0xeu);
    return (u32)(s32)(first > second ? first : second);
}
