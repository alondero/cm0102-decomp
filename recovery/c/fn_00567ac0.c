/* Preferred-image VA 0x00567AC0, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Physical thiscall with four caller-supplied stack dwords, RET 0x10.
   Object pointer is ECX. Stores, in order:
     [ECX+0x28] = arg1 (ESP+4 at entry)
     [ECX+0x2C] = arg3 (ESP+0xC at entry)
     [ECX+0x30] = arg2 (ESP+8 at entry)
     [ECX+0x34] = arg4 (ESP+0x10 at entry)
   No semantic return is produced; direct callers do not observe EAX or EDX.
   Field meanings remain unknown. */
#include "cmre.h"

void CM_THISCALL cm_fn_00567ac0(void *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u8 *base = (u8 *)self;

    *(volatile u32 *)(base + 0x28) = arg1;
    *(volatile u32 *)(base + 0x2c) = arg3;
    *(volatile u32 *)(base + 0x30) = arg2;
    *(volatile u32 *)(base + 0x34) = arg4;
}
