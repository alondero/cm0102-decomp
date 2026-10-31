/* Preferred-image VA 0076d430, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Preserve full EAX, including AL-only mode update on the fallback path. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_0076d430(u32 object, u8 mode, u32 first, u32 second)
{
    u8 work[0x200];
    u32 result, expected, version;
    if (first == 0 || second == 0) {
        cm_fn_00944cff(0x00a5b2a4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x7dau);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    result = object;
    if (result != 0) {
        result = D(result + 0x14u);
        if (result != 0) {
            result = D(result);
            expected = D(0x009cf79cu);
            if (result == expected)
                goto equal;
            expected = D(0x009cf7a0u);
            if (result == expected)
                goto equal;
            expected = D(0x009cf7a4u);
            if (result == expected)
                goto equal;
            expected = D(0x009cf7a8u);
            if (result == expected)
                goto equal;
            expected = D(0x009cf7acu);
            if (result == expected)
                goto equal;
            expected = D(0x009cf790u);
            if (result == expected)
                goto equal;
            expected = D(0x009cf8a0u);
            if (result == expected)
                goto equal;
            expected = D(0x009cf78cu);
            if (result == expected)
                goto equal;
            expected = D(0x009cf964u);
            if (result == expected)
                goto equal;
        }
    }
    result = (result & 0xffffff00u) | mode;
    B(first) = mode == 0 || mode == 2 ? 26 : 24;
    B(second) = 22;
    return result;
equal:
    B(first) = 23;
    B(second) = 23;
    return result;
}
