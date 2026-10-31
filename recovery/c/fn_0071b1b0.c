/* Preferred-image VA 0071b1b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original007ff130 initializes full EAX at VA007ff154 before its branches;
   preserve the returned EAX rather than discarding the final setter result. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007fe4a0(u32 self, u32 record, u32 first_callback, u32 second_callback, u32 first, u32 second, u32 third);
extern u32 CM_THISCALL cm_fn_007ff130(u32 self, u32 record, u32 index, u32 value, u32 flag);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_0071b1b0(u32 first, u32 second)
{
    u8 work[0x200];
    u32 version, result;
    if (first == 0 || second == 0) {
        cm_fn_00944cff(0x00a161d4u, 0, 0, work + 0x100, work);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x806u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return result;
    }
    result = cm_fn_007fe4a0(0x00b74450u, first, 0x0071b300u, 0x0071b3d0u, 0, 0, 0);
    if (result == 0)
        return result;
    cm_fn_007ff130(0x00b74450u, first, 0, 0, 0);
    cm_fn_007ff130(0x00b74450u, first, 14, second, 0);
    cm_fn_007ff130(0x00b74450u, first, 15, 0, 0);
    cm_fn_007ff130(0x00b74450u, first, 16, 0xffffffffu, 0);
    cm_fn_007ff130(0x00b74450u, first, 17, 1, 0);
    cm_fn_007ff130(0x00b74450u, first, 18, 0, 0);
    cm_fn_007ff130(0x00b74450u, first, 19, 1, 0);
    return cm_fn_007ff130(0x00b74450u, first, 20, 0, 0);
}
