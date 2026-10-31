/* Preferred-image VA 0x00664390, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
typedef unsigned long long (CM_THISCALL *slot_fn)(u32 self);
extern unsigned long long CM_CDECL cm_fn_004afce0(u32 a, u32 b, u32 c, u32 d, u32 e);
extern u32 CM_THISCALL cm_fn_006826d0(u32 self, u32 arg);

unsigned long long CM_THISCALL cm_fn_00664390(u32 self)
{
    u32 list, first, second, nested, selected, saved;
    if (D(self + 0x2cu) != 2) {
        list = D(self + 0xcu);
        selected = D(list + 8u);
        nested = D(selected);
        return ((slot_fn)D(nested + 0x30u))(selected);
    }
    list = D(self + 0xcu);
    first = D(list);
    nested = D(first + 0xb1u);
    second = D(list + 4u);
    saved = D(nested);
    second = cm_fn_006826d0(second, 1);
    list = D(self + 0xcu);
    first = cm_fn_006826d0(D(list), 1);
    if ((s16)W(second + 0x19u) > (s16)W(first + 0x19u)) {
        list = D(self + 0xcu);
        selected = D(list + 4u);
    } else {
        list = D(self + 0xcu);
        selected = D(list);
    }
    selected = cm_fn_006826d0(selected, 1);
    return cm_fn_004afce0(self, saved, D(selected), 0, 0);
}
