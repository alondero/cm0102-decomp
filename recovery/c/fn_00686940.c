/* Preferred-image VA 0x00686940, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern void CM_THISCALL cm_fn_00868c50(u32 self, u32 a, u32 b);
extern void CM_THISCALL cm_fn_00869480(u32 self, u32 a, u32 b, u32 c);
extern void CM_THISCALL cm_fn_0086a1c0(u32 self, u32 a, u32 b);

u32 CM_THISCALL cm_fn_00686940(u32 self, u32 argument, u32 mode, u32 a,
                               u32 b, u32 c, u32 d)
{
    u32 value, receiver;
    (void)a; (void)b; (void)c; (void)d;
    if ((u8)mode == 5) {
        value = D(self + 4u);
        receiver = D(0x00dd7f00u);
        cm_fn_00868c50(receiver, argument, value);
    } else if ((u8)mode == 0) {
        value = D(self + 4u);
        receiver = D(0x00dd7f00u);
        cm_fn_00869480(receiver, argument, value, 100);
    } else if ((u8)mode == 3) {
        value = D(self + 4u);
        receiver = D(0x00dd7f00u);
        cm_fn_0086a1c0(receiver, argument, value);
    }
    return 0;
}
