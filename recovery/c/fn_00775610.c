/* Preferred-image VA 00775610, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_00775610(volatile u8 kind_argument, volatile u32 flag,
                             volatile u32 output, s32 capacity, volatile u8 mode_argument)
{
    u8 work[0x200];
    u8 mode, kind;
    u32 format, destination, result, version;
    if (capacity < 20) {
        cm_fn_00944cff(0x00a5b2a4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x1c98u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    mode = mode_argument;
    kind = kind_argument;
    if (mode == 1) {
        if (kind == 0)
            format = 0x009cd9b4u;
        else if (kind == 1)
            format = 0x00a62fa0u;
        else if (flag != 0)
            format = 0x00a62f94u;
        else
            format = 0x00a62f88u;
    } else {
        if (kind == 0)
            format = 0x00a62f80u;
        else if (kind == 1)
            format = 0x00a62fa0u;
        else if (flag != 0)
            format = 0x00a62f74u;
        else
            format = 0x00a62f68u;
    }
    destination = output;
    return cm_fn_0066f4e0(destination, format);
}
