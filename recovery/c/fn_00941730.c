#include "cmre.h"

u32 CM_CDECL cm_fn_00941730(u32 address)
{
    u32 value = *(volatile u32 *)(address + 0xcu);
    *(volatile u32 *)(address + 4u) = value;
    return address;
}
