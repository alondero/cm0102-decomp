/* Preferred-image VA 0x0094b003, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0094af63(u32 stream);

u32 CM_CDECL cm_fn_0094b003(u32 mode)
{
    u32 index = 0, count = 0, error = 0;
    if ((s32)D(0x00de42c0u) > 0) {
        do {
            u32 table = D(0x00de32bcu);
            u32 stream = D(table + index * 4u);
            if (stream) {
                u32 flags = D(stream + 0xcu);
                if (flags & 0x83u) {
                    if (mode == 1) {
                        if (cm_fn_0094af63(stream) != 0xffffffffu) ++count;
                    } else if (!mode && (flags & 2u)) {
                        if (cm_fn_0094af63(stream) == 0xffffffffu) error = 0xffffffffu;
                    }
                }
            }
            ++index;
        } while ((s32)index < (s32)D(0x00de42c0u));
    }
    return mode == 1 ? count : error;
}
