#include "cmre.h"

u32 CM_STDCALL cm_fn_008d0d90(u32 object)
{
    u8 kind = *(volatile u8 *)(object + 0x2c);
    if (kind == 2 || kind == 3 || kind == 0x14 || kind == 0x10 || kind == 4)
        return 1;
    if (kind == 0x0f)
        return *(volatile u8 *)(object + 0x2e) >= 3;
    return 0;
}
