/* Preferred-image VA 0094c034, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern u32 CM_CDECL cm_fn_0094ec7b(u32 descriptor);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

u32 CM_CDECL cm_fn_0094c034(u32 stream)
{
    u32 index, slot, value;
    if (cm_fn_0094ec7b(D(stream + 0x10u)) == 0)
        return 0;
    if (stream == 0x00ad92e0u)
        index = 0;
    else if (stream == 0x00ad9300u)
        index = 1;
    else
        return 0;

    D(0x00de3144u) = D(0x00de3144u) + 1u;
    if ((W(stream + 0xcu) & 0x10cu) != 0)
        return 0;
    slot = 0x00de3034u + index * 4u;
    if (D(slot) == 0) {
        value = (u32)cm_fn_00944e46(0x1000u);
        D(slot) = value;
        if (value == 0) {
            value = stream + 0x14u;
            D(stream + 8u) = value;
            D(stream) = value;
            D(stream + 0x18u) = 2;
            D(stream + 4u) = 2;
            W(stream + 0xcu) = (u16)(W(stream + 0xcu) | 0x1102u);
            return 1;
        }
    }
    value = D(slot);
    D(stream + 0x18u) = 0x1000u;
    D(stream + 8u) = value;
    D(stream) = value;
    D(stream + 4u) = 0x1000u;
    W(stream + 0xcu) = (u16)(W(stream + 0xcu) | 0x1102u);
    return 1;
}
