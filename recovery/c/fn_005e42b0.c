#include "cmre.h"

extern unsigned long long CM_CDECL cm_fn_009452ca(u32 pointer);

void CM_CDECL cm_fn_005e42b0(u32 object)
{
    u32 field;
    if (object == 0) return;
    field = *(volatile u32 *)(object + 0x0cu);
    if (field != 0) {
        cm_fn_009452ca(field);
        *(volatile u32 *)(object + 0x0cu) = 0;
    }
    cm_fn_009452ca(object);
}
