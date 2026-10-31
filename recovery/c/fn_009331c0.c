/* Preferred-image VA 009331c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern unsigned long long CM_CDECL cm_fn_00944c94(u32 allocation);
extern u32 CM_CDECL cm_fn_00945b00(u32 path, u32 mode);
extern u32 CM_CDECL cm_fn_00945bfe(u32 buffer, u32 size, u32 count, u32 stream);
extern u32 CM_CDECL cm_fn_00945a09(u32 stream);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_009331c0(volatile u32 argument)
{
    u8 work[0x200];
    u32 allocation, path, stream, count, changed = 0, kind, result, version;
    allocation = (u32)cm_fn_00944cf1(0x416ecu);
    if (allocation == 0) {
        cm_fn_00944cff(0x00ad6ab0u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x250u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 20;
    }
    path = argument;
    stream = cm_fn_00945b00(path, 0x0098f928u);
    if (stream == 0) {
        cm_fn_00944c94(allocation);
        return 20;
    }
    if (cm_fn_00945bfe(allocation, 12, 1, stream) != 0) {
        if (D(allocation) == 1) {
            D(allocation) = 3;
            changed = 1;
        }
        if (D(allocation) == 2) {
            D(allocation) = 4;
            changed = 1;
        }
        count = ((0u - changed) & 0xfffffd12u) + 1000u;
        cm_fn_00945bfe(allocation + 12u, 0x10cu, count, stream);
    }
    cm_fn_00945a09(stream);
    kind = D(allocation);
    if (kind != 3 && kind != 4) {
        cm_fn_00944c94(allocation);
        return 20;
    }
    result = D(allocation + 4u);
    cm_fn_00944c94(allocation);
    return result;
}
