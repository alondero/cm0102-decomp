/* Preferred-image VA 0090a000, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_0090a000(u32 club)
{
    u8 work[0x200];
    u32 value, expected, version;
    value = D(club + 0x57u);
    if (value == 0)
        return 0;
    value = D(value);
    expected = D(0x009cf590u);
    if (value != expected)
        return value & 0xffff0000u;
    value = D(club);
    if (value == D(0x009d0390u) || value == D(0x009d0394u) ||
        value == D(0x009d0388u) || value == D(0x009d0384u) ||
        value == D(0x009d03a4u) || value == D(0x009d0380u))
        return (value & 0xffff0000u) | 0x413u;
    if (value == D(0x009d03a8u) || value == D(0x009d03a0u) ||
        value == D(0x009d039cu) || value == D(0x009d0398u) ||
        value == D(0x009d038cu) || value == D(0x009d037cu))
        return (value & 0xffff0000u) | 0x414u;
    cm_fn_00944cff(0x00aa1fb4u, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x743u);
    value = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return value & 0xffff0000u;
}
