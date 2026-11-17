#include "cmre.h"
#include "cmre_x87.h"

extern unsigned long long CM_CDECL cm_fn_0090cfc0(u32 entry_eax, u32 entry_edx, u32 bound);

u8 CM_THISCALL cm_fn_007164b0(u32 self, s8 kind, volatile u32 arg)
{
    s32 threshold = 1;
    u32 entry_arg = arg;
    u32 bound;
    long double t;
    if (entry_arg != 0 && entry_arg == *(volatile u32 *)(self + 0x1a6))
        threshold = 8;
    t = cm_x87_CIpow((long double)*(volatile float *)(self + 0x1a2),
                     (long double)CM_GLOBAL(double, 0x968950)); /* 007164d6 */
    /* CM0102 3.9.68 SHA-256
       0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
       Preferred-image VA 007164e0 writes entry ESP+8 after the two
       pushes at 007164b4/007164b5. Preserve the caller-owned DWORD home:
       it can alias the receiver float reloaded at 007164fa. */
    arg = (u32)(long long)t; /* 007164db conversion, 007164e0 store */
    if ((u32)((s32)kind - 1) > 4)
        return 0;
    (void)CM_GLOBAL(volatile u32, 0x716590 + ((u32)((s32)kind - 1) * 4));
    switch (kind) {
    case 1:
        t = (long double)*(volatile float *)(self + 0x1a2);
        bound = (u32)(110 - (s32)(s8)(u8)(long long)t); /* 00716500 */
        break;
    case 2:
        t = (long double)CM_GLOBAL(double, 0x96d860) / (long double)(s32)arg;
        bound = (u32)(long long)t; /* 0071651c */
        break;
    case 3:
        t = (long double)CM_GLOBAL(double, 0x96aff8) / (long double)(s32)arg;
        bound = (u32)(long long)t; /* 0071656d */
        break;
    case 4:
        t = (long double)CM_GLOBAL(double, 0x96d858) / (long double)(s32)arg;
        bound = (u32)(long long)t; /* 00716547 */
        break;
    default:
        t = (long double)CM_GLOBAL(double, 0x96d850) / (long double)(s32)arg;
        bound = (u32)(long long)t; /* 0071656d */
        break;
    }
    return (s32)(u32)cm_fn_0090cfc0(bound, 0, bound) < threshold;
}
