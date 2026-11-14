/* Preferred-image VA 0x0094633b, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Element walker: the four arguments mirror the enclosing routine's
   (element, stride, count, callback). Each pass steps the element back by the
   stride and calls the callback with the new element in ECX. */
#include "cmre.h"

/* The callback is invoked with the element in ECX and no stack argument. */
typedef void (CM_THISCALL *cm_element_cb)(u32 element);

void CM_STDCALL cm_fn_0094633b(u32 element, u32 stride, u32 count,
                                cm_element_cb callback)
{
    /* Registration record: prev, handler 0x0094bf5c, scope table 0x009713e0,
       state, pushed in the listing's order. */
    volatile struct { void *prev; u32 handler; u32 scope; s32 state; } seh;

    seh.state = -1;
    seh.scope = 0x009713e0u;
    seh.handler = 0x0094bf5cu;
    seh.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&seh;

    seh.state = 0;

    for (;;) {
        count = count - 1u;
        if ((s32)count < 0) {
            break;
        }
        element = element - stride;
        callback(element);
    }

    seh.state = -1;
    CM_FS(void *, 0) = seh.prev;
}
