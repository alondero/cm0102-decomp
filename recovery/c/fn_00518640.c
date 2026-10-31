#include "cmre.h"

/* Preferred-image VA 0x00518640, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern unsigned long long CM_THISCALL cm_fn_0048c580(void *);

u32 CM_FASTCALL cm_fn_00518640(u32 self)
{
    cm_fn_0048c580((void *)self);
    *(volatile u32 *)self = 0x009693c8u;
    *(volatile u8 *)(self + 0xabu) = 0;
    *(volatile u8 *)(self + 0xacu) = 0;
    *(volatile u8 *)(self + 0x43u) = 1;
    return self;
}
