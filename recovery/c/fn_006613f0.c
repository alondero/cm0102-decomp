/* Preferred-image VA 006613f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_006613f0(u32 self)
{
    u8 work[0x200];
    u32 allocation, version, result;
    B(self + 8u) = 3;
    allocation = (u32)cm_fn_00944e46(36);
    D(self + 4u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x009ea9a4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x52u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    B(allocation) = 0xe;
    B(allocation + 1u) = 0;
    B(allocation + 2u) = 0xff;
    B(allocation + 3u) = 1;
    B(allocation + 4u) = 6;
    B(allocation + 5u) = 1;
    allocation = D(self + 4u) + 6u;
    B(allocation) = 0xe;
    B(allocation + 1u) = 0;
    B(allocation + 2u) = 4;
    B(allocation + 3u) = 0x1b;
    B(allocation + 4u) = 9;
    B(allocation + 5u) = 0;
    allocation = D(self + 4u) + 12u;
    B(allocation) = 0xe;
    B(allocation + 1u) = 1;
    B(allocation + 2u) = 0xff;
    B(allocation + 3u) = 2;
    B(allocation + 4u) = 0;
    B(allocation + 5u) = 1;
    allocation = D(self + 4u) + 18u;
    B(allocation) = 0xe;
    B(allocation + 1u) = 1;
    B(allocation + 2u) = 0xff;
    B(allocation + 3u) = 0x1f;
    B(allocation + 4u) = 0;
    B(allocation + 5u) = 0;
    allocation = D(self + 4u) + 24u;
    B(allocation) = 0xe;
    B(allocation + 1u) = 2;
    B(allocation + 2u) = 0xff;
    B(allocation + 3u) = 1;
    B(allocation + 4u) = 6;
    B(allocation + 5u) = 1;
    allocation = D(self + 4u);
    B(allocation + 35u) = 0;
    allocation += 30;
    B(allocation) = 0xe;
    B(allocation + 1u) = 2;
    B(allocation + 2u) = 0xff;
    B(allocation + 3u) = 0x1e;
    B(allocation + 4u) = 3;
    return allocation;
}
