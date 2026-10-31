/* Preferred-image VA 0x008e7f10, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);

u8 CM_THISCALL cm_fn_008e7f10(u32 self, u32 key)
{
    u32 list, item, value;
    if (key == D(self + 0xcu)) {
        if (B(self + 0x2cu) == 3) B(self + 0x10u) = 4;
        return B(self + 0x10u);
    }
    list = D(self + 4u);
    if (!list) return 4;
    value = D(list + 2u);
    D(list + 0xau) = value;
    do {
        list = D(self + 4u);
        item = cm_fn_0068d1b0(list);
        if (!item) return 4;
        if (D(item) == key) return B(item + 4u);
    } while (1);
}
