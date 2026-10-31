#include "cmre.h"

u8 CM_STDCALL cm_fn_006ae180(u32 object, u8 kind)
{
    return kind != 2u || *(volatile u8 *)(object + 0x7fu) != 0x14u;
}
