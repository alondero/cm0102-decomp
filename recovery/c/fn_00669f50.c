/* Preferred-image VA 00669f50, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00669f50(u32 record, volatile u32 output, volatile s32 capacity)
{
    u8 work[0x200];
    u32 destination, id, format, version;
    if (record == 0)
        return 0;
    destination = output;
    if (destination != 0 && capacity < 20) {
        cm_fn_00944cff(0x009eaff4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x4a1u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    id = D(record);
    if (id == D(0x009cf2e4u)) {
        if (destination != 0)
            cm_fn_0066f4e0(destination, 0x009eb078u, 0x00ad9c64u);
        return 1;
    }
    if (id == D(0x009cf314u))
        format = 0x009eb064u;
    else if (id == D(0x009cf360u))
        format = 0x009eb050u;
    else if (id == D(0x009cf434u) || id == D(0x009cf488u))
        format = 0x009eb03cu;
    else if (id == D(0x009cf4a8u))
        format = 0x009eb02cu;
    else
        return 0;
    if (destination != 0)
        cm_fn_0066f4e0(destination, format);
    return 1;
}
