/* Preferred-image VA 0x008b2230, image base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX is read before written and forwarded to the callee, so the entry is
   thiscall with the receiver in ECX and no stack arguments (ret 0). The body
   calls 0x008b50f0 exactly 0xa7 times, passing ECX = base + k * 0x9b for
   k = 0..0xa6, advancing the receiver by 0x9b after each call and counting
   down a 0xa7 iteration counter. ESI/EDI are saved and restored, so both
   callee-saved registers leave unchanged. EAX is never defined by this body.
   Its direct caller 0x008b3d40 forwards EAX through its epilogue; that caller's
   only indexed caller at 0x008b1038 overwrites EAX at 0x008b104c before use,
   establishing the void return transitively. Indirect callers remain an explicit
   evidence boundary. The callee stays an original boundary. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_008b50f0(volatile u8 *self);

void CM_THISCALL cm_fn_008b2230(volatile u8 *self)
{
    u32 address = (u32)(unsigned long)self;
    u32 count = 0xa7u;

    do {
        cm_fn_008b50f0((volatile u8 *)(unsigned long)address);
        address += 0x9bu;
        count -= 1u;
    } while (count != 0u);
}
