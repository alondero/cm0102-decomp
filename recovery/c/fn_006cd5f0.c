/* Preferred-image VA 0x006cd5f0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_THISCALL cm_fn_00714d30(u32 self);

u8 CM_THISCALL cm_fn_006cd5f0(u32 self, u32 object)
{
    u8 mode = cm_fn_00714d30(object);
    s8 value = (s8)B(object + 0x103u);
    u32 other;
    s8 threshold;
    if (!mode) {
        other = D(self + 0xf58eu);
        threshold = other ? (s8)B(other + 0x103u) : 12;
        return value <= threshold;
    }
    other = D(self + 0xf592u);
    threshold = other ? (s8)B(other + 0x103u) : -1;
    return value >= threshold;
}
