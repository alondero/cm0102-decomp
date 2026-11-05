/* Preferred-image VA 0x00413540, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees and virtual dispatch remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_00931aa0(u32 self, u32 key);

u32 CM_THISCALL cm_fn_00413540(u32 self, u32 key)
{
    u32 slot = D(self + 8u);
    u32 object, table, target, result;
    u32 count;

    for (count = 30; count != 0; --count) {
        object = D(slot);
        if (object != 0) {
            table = D(object);
            target = D(table + 4u);
            ((u32 (CM_THISCALL *)(u32, u32))target)(object, key);
        }
        slot += 4u;
    }
    result = D(self + 0xdcu);
    if (result != 0) {
        slot = self + 0x1cu;
        result = 0;
        for (count = 48; count != 0; --count) {
            D(slot) = 0;
            slot += 4u;
        }
        D(self + 0xdcu) = 0;
    }
    object = D(self + 0x10u);
    if (object != 0)
        result = cm_fn_00931aa0(object, key);
    object = D(self + 0x0cu);
    if (object != 0)
        result = cm_fn_00931aa0(object, key);
    return result;
}
