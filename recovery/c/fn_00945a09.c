/* Preferred-image VA 0x00945a09, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0094af9e(u32 stream);
extern void CM_CDECL cm_fn_0094af38(u32 stream);
extern unsigned long long CM_CDECL cm_fn_0094ae85(u32 descriptor);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 value);

u32 CM_CDECL cm_fn_00945a09(u32 stream)
{
    u32 flags = D(stream + 0xcu), result = 0xffffffffu;
    if (!(flags & 0x40u) && (flags & 0x83u)) {
        u32 close_result;
        result = cm_fn_0094af9e(stream);
        cm_fn_0094af38(stream);
        close_result = (u32)cm_fn_0094ae85(D(stream + 0x10u));
        if ((s32)close_result < 0) result = 0xffffffffu;
        else {
            u32 buffer = D(stream + 0x1cu);
            if (buffer) {
                cm_fn_009452ca(buffer);
                D(stream + 0x1cu) &= 0;
            }
        }
    }
    D(stream + 0xcu) &= 0;
    return result;
}
