/* Preferred-image VA 0x0094c9ae, image base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both indexed callers overwrite EAX immediately (00946c58, 0094c4d4).
   ECX passes unchanged to 0094c9c3: its initial saved ECX word is later
   supplied as the default-character output slot for the conversion calls.
   Naming that register boundary preserves its actual value without inventing
   initialization or Windows API output. The original initializer remains code.
 */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0094c9c3(u32 forwarded_ecx);

void CM_FASTCALL cm_fn_0094c9ae(u32 forwarded_ecx)
{
    if (CM_GLOBAL(volatile u32, 0x00de3118u) == 0) {
        u32 current;
        cm_fn_0094c9c3(forwarded_ecx);
        current = CM_GLOBAL(volatile u32, 0x00de3118u);
        CM_GLOBAL(volatile u32, 0x00de3118u) = current + 1u;
    }
}
