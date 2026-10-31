/* Preferred-image VA 0084b330, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_0084b330(volatile s8 kind, volatile u32 output, s32 capacity)
{
    u8 work[0x200];
    u32 index, target, format, destination, result, version;
    if (capacity < 30) {
        cm_fn_00944cff(0x00a825a4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x2daeu);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    index = (u32)(s32)kind - 1u;
    if (index > 7u) {
        format = 0x00a8262cu;
    } else {
        target = D(0x0084b4a8u + index * 4u);
        switch (target) {
        case 0x0084b3c4u: format = 0x00a8275cu; break;
        case 0x0084b3e2u: format = 0x00a8272cu; break;
        case 0x0084b400u: format = 0x00a826fcu; break;
        case 0x0084b40au: format = 0x00a826d0u; break;
        case 0x0084b428u: format = 0x00a826a0u; break;
        case 0x0084b446u: format = 0x00a82678u; break;
        case 0x0084b44du: format = 0x00a82650u; break;
        case 0x0084b46bu: format = 0x00a82638u; break;
        default: __builtin_unreachable(); /* Exhaustive pinned target table. */
        }
    }
    destination = output;
    return cm_fn_0066f4e0(destination, format);
}
