#include "cmre.h"

extern u32 CM_CDECL cm_fn_009445d0(u32 self, u32 entry);

u32 CM_CDECL cm_fn_009442c0(u32 self)
{
    u32 cursor = self + 0x50;
    u32 result = 0;
    u32 i;
    for (i = 0; i != 6; ++i) {
        u32 entry = *(volatile u32 *)cursor;
        result = cm_fn_009445d0(self, entry);
        *(volatile u32 *)cursor = 0;
        *(volatile u32 *)(cursor + 0x18) = 0;
        *(volatile u32 *)(cursor + 0x30) = 1;
        cursor += 4;
    }
    return result;
}
