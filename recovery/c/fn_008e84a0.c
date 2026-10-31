/* Preferred-image VA 008e84a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS5/RET14; full EAX returns the object. Preserve native field access order,
   including the record read before the zero stores and the second-path cached
   global read before object stores that may overlap that global. */
#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

u8 *CM_THISCALL cm_fn_008e84a0(u8 *object, u32 arg1, s32 arg2, s32 arg3, u32 arg4, u32 arg5)
{
    u32 self = (u32)object;
    u32 record, value;

    (void)arg4;
    (void)arg5;
    B(self + 9u) = (u8)arg1;
    D(self) = 0x00970458u;
    if (arg2 > -1) {
        record = D(0x00ae23a8u) + (u32)arg2 * 0x122u;
        D(self + 0x0au) = record;
        value = D(record + 0x71u);
        D(self + 4u) = 0;
        D(self + 0x15u) = 0;
        D(self + 0x0eu) = value;
        B(self + 0x12u) = 1;
        B(self + 0x13u) = 0x32;
        B(self + 0x14u) = 0x1e;
        return object;
    }
    D(self + 0x0au) = 0;
    if (arg3 > -1) {
        record = D(0x00ae23a4u);
        D(self + 4u) = 0;
        D(self + 0x15u) = 0;
        B(self + 0x12u) = 2;
        B(self + 0x13u) = 0x32;
        B(self + 0x14u) = 0x1e;
        D(self + 0x0eu) = record + (u32)arg3 * 0xc6u;
        return object;
    }
    D(self + 0x0eu) = 0;
    D(self + 4u) = 0;
    D(self + 0x15u) = 0;
    B(self + 0x12u) = 4;
    B(self + 0x13u) = 0x32;
    B(self + 0x14u) = 0x1e;
    return object;
}
