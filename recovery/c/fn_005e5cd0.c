#include "cmre.h"

/* Preferred-image VA 005e5cd0, base 00400000; target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The fixed-size path still reads the original dispatch table. The other
   path uses signed WORD indices and wrapping DWORD address arithmetic. */
u32 CM_CDECL cm_fn_005e5cd0(s16 index)
{
    u32 position;
    if (CM_GLOBAL(volatile u32, 0x009cc2c4u) != 0xffffffffu) {
        position = (u32)(s32)index;
        if (position > 7u)
            return 0;
        (void)CM_GLOBAL(volatile u32, 0x005e5d38u + position * 4u);
        switch (position) {
        case 0: case 1: return 0x0f;
        case 2: return 0x12;
        case 3: return 0x15;
        case 4: return 0x18;
        case 5: return 0x1b;
        case 6: return 0x27;
        case 7: return 0x2d;
        }
    }
    if (index == -1)
        return 0;
    position = (u32)(s32)index;
    position = ((position + position * 4u) << 8) + position;
    return CM_GLOBAL(volatile u32, 0x00ae1dacu + position * 4u);
}
