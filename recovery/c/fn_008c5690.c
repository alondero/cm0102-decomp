/* Preferred-image VA 008c5690, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_008c5690(volatile s16 kind, volatile u32 destination, s32 capacity)
{
    u8 work[0x200];
    u32 version, result, format, output;
    s32 value;
    if (capacity < 50) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x163du);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    value = kind;
    if (value > 0x400) {
        if (value == 0x800)
            format = 0x00a8ea4cu;
        else if (value == 0x1000)
            format = 0x00a8ea70u;
        else if (value == 0x2000)
            format = 0x00a826a0u;
        else
            return (u32)value;
    } else if (value == 0x400) {
        format = 0x00a8272cu;
    } else if (value == 0) {
        output = destination;
        return cm_fn_0066f4e0(output, 0x00a8eaa4u, 0x00ad9c64u);
    } else if (value == 10) {
        format = 0x00a8eae0u;
    } else if (value == 0x200) {
        format = 0x00a8275cu;
    } else {
        return (u32)value;
    }
    output = destination;
    return cm_fn_0066f4e0(output, format);
}
