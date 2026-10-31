/* Preferred-image VA 0x0053a5b0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090cfc0(u32 limit);

u32 CM_CDECL cm_fn_0053a5b0(void)
{
    u32 index = D(0x009b59b4u), count, base, cursor;
    if (index == 0xffffffffu) {
        count = D(0x00ae23a0u);
        index = 0;
        if ((s32)count > 0) {
            base = D(0x00ae23f8u);
            cursor = base;
            do {
                if (!B(cursor)) {
                    D(0x009b59b4u) = index;
                    if (index != 0xffffffffu) return base + index * 60u;
                    break;
                }
                ++index;
                cursor += 60u;
            } while ((s32)index < (s32)count);
        }
        index = cm_fn_0090cfc0(count);
    }
    base = D(0x00ae23f8u);
    return base + index * 60u;
}
