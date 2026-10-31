#include "cmre.h"

u32 CM_STDCALL cm_fn_004cb960(u32 object)
{
    u8 kind = *(volatile u8 *)(object + 0x3d);
    if (kind >= 1 && kind <= 4) {
        u32 value = *(volatile u32 *)(object + 0x39);
        if (value != 0)
            return 0;
    }
    return 1;
}
