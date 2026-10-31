/* Preferred-image VA 0x00774f30, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, u32 output, u32 argument);

u32 CM_THISCALL cm_fn_00774f30(u32 self, u32 key)
{
    u8 scratch[8];
    u32 list = D(self), value, item;
    if (!list) return 0;
    value = D(list + 2u);
    D(list + 0xau) = value;
    list = D(self);
    item = cm_fn_0068d1b0(list);
    while (item) {
        value = D(item + 4u);
        if (value == D(key)) {
            value = cm_fn_0054c5b0(0x00ae2c90u, (u32)scratch, 1);
            W(item + 0x10u) = W(value);
        }
        list = D(self);
        item = cm_fn_0068d1b0(list);
    }
    return 0;
}
