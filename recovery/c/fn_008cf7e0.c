/* Preferred-image VA 008cf7e0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both indexed callers discard EAX: 0045df14 overwrites it; the two paths
   after 008cf8a3 overwrite it at 008cf8ad or 008cfc7c before use. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_008cf6b0(u32 self, u32 record, void *output, u32 flag);
#define B(a) CM_GLOBAL(volatile u8, (a))

void CM_THISCALL cm_fn_008cf7e0(u32 self, u32 record, u32 output)
{
    volatile u8 scratch[0x2c];
    u8 value;
    CM_GLOBAL(volatile u32, output) = 0;
    CM_GLOBAL(volatile u16, output + 4u) = 0;
    (void)cm_fn_008cf6b0(self, record, (void *)scratch, 1);
    value = scratch[5];
    if (value != 0xff) {
        B(output) = 1;
        B(output + 5u) = value;
        return;
    }
    value = scratch[3];
    if (value != 0xff) {
        B(output) = 1;
        B(output + 5u) = value;
        return;
    }
    value = scratch[2];
    if (value != 0xff) {
        B(output) = 2;
        B(output + 5u) = value;
        return;
    }
    value = scratch[0];
    if (value != 0xff) {
        B(output) = 2;
        B(output + 5u) = value;
        return;
    }
    value = scratch[0x16];
    if (value != 0xff) {
        B(output + 1u) = value;
        value = scratch[0x17];
        B(output) = 3;
        B(output + 5u) = value;
    }
}
