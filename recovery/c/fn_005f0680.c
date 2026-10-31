/* Preferred-image VA 005f0680, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_005f0680(u32 buffer, s32 count, u32 expected)
{
    u8 work[0x204];
    u32 version, i, first, second, value, sum = 0;
    u8 a, b;
    if (buffer == 0 || count < 0) {
        cm_fn_00944cff(0x009ce408u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x39fu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    i = (u32)count;
    while (i != 0) {
        --i;
        value = D(0x009cdc88u + (i % 480u) * 4u);
        first = (u32)((s32)value % count);
        value = D(0x009cdc88u + ((i + 1u) % 480u) * 4u);
        second = (u32)((s32)value % count);
        a = B(buffer + second);
        b = B(buffer + first);
        B(buffer + first) = a;
        B(buffer + second) = b;
    }
    for (i = 0; (s32)i < count; ++i) {
        a = B(buffer + i);
        b = B(0x009cdc88u + (i % 480u) * 4u);
        a = (u8)(a ^ b);
        B(buffer + i) = a;
        sum += a;
    }
    return sum == expected;
}
