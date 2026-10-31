/* Preferred-image VA 008043c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

/* Carry the complete pushed slot, including the high bytes saved by PUSH ECX.
   The original callee reads its first stack argument as a signed byte. */
extern u32 CM_THISCALL cm_fn_007f6cd0(u32 self, u32 kind_word, u32 second);
extern u32 CM_THISCALL cm_fn_007ae950(u32 self);
extern u32 CM_THISCALL cm_fn_007ae8a0(u32 self, u32 input);

u32 CM_THISCALL cm_fn_008043c0(u32 self)
{
    u32 destination = self + 0x1b0eu;
    u32 kind_word = self & 0xffffff00u;
    u32 item, input;
    u8 kind = 0;

    do {
        item = cm_fn_007f6cd0(D(0x00b74448u), kind_word, 0);
        if (B(item + 0xau) == 4) {
            item = cm_fn_007f6cd0(D(0x00b74448u), kind_word, 0);
            input = cm_fn_007ae950(item);
            (void)cm_fn_007ae8a0(destination - 0x1b0du, input);
            item = cm_fn_007f6cd0(D(0x00b74448u), kind_word, 0);
            input = cm_fn_007ae950(item);
            (void)cm_fn_007ae8a0(destination, input);
        }
        kind = (u8)(kind + 1u);
        destination += 0x2b0u;
        kind_word = (kind_word & 0xffffff00u) | kind;
    } while ((s8)kind < 10);
    return 1;
}
