#include "cmre.h"

typedef u32 (CM_THISCALL *method_fn)(u32 self, u32 arg);

u32 CM_CDECL cm_fn_006692f0(void)
{
    u32 row;
    for (row = 0x00b63d6cu; row < 0x00b64744u; row += 0x48u) {
        u32 array = *(volatile u32 *)(row + 4u);
        s32 count;
        u32 index = 0;
        if (array == 0)
            continue;
        count = *(volatile s32 *)row;
        if (count <= 0)
            continue;
        do {
            u32 object, table;
            method_fn method;
            array = *(volatile u32 *)(row + 4u);
            object = *(volatile u32 *)(array + index * 4u);
            if (object != 0) {
                table = *(volatile u32 *)object;
                method = (method_fn)*(volatile u32 *)(table + 0x1cu);
                method(object, 1);
            }
            count = *(volatile s32 *)row;
            ++index;
        } while ((s32)index < count);
    }
    return 1;
}
