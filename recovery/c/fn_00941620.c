#include "cmre.h"

u32 CM_CDECL cm_fn_00941620(u32 address)
{
    u32 first = *(volatile u32 *)(address + 8u);
    u32 second = *(volatile u32 *)(address + 12u);
    return first - second;
}
