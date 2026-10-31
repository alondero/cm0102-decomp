/* Preferred-image VA 0x005b1fc0, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0090bde0(u32 a, u32 b);
extern u32 CM_CDECL cm_fn_009466d0(u32 destination, u32 source, u32 count);
extern u32 CM_CDECL cm_fn_005b2170(void);

u32 CM_CDECL cm_fn_005b1fc0(void)
{
    u32 value;
    if (!D(0x00ae2c68u)) {
        value = cm_fn_0090bde0(0x009c3718u, 0x009cc2c8u);
        if (!value) {
            value = cm_fn_0090bde0(0x009b5348u, 0x009cc2c8u);
            if (!value) {
                value = D(0x00ae2c70u);
                B(0x00ae2a64u) = 0;
                return value & 0xffffu;
            }
        }
        cm_fn_009466d0(0x00ae2a64u, value, 0x104u);
        B(0x00ae2b67u) = 0;
        if (cm_fn_005b2170()) D(0x00ae2c68u) = 1;
    }
    return D(0x00ae2c70u) & 0xffffu;
}
