/* Preferred-image VA 00591840, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_00591840(u32 first, volatile u32 second, volatile u32 third,
                           volatile u32 fourth, volatile s16 fifth,
                           volatile s16 sixth, volatile u32 seventh)
{
    u8 work[0x200];
    u32 b, c, value, result, version;
    if (first == 0)
        goto diagnostic;
    b = second;
    if (b == 0)
        goto diagnostic;
    c = third;
    if (c == 0)
        goto diagnostic;
    result = cm_fn_007fe600(0x00b74450u, 0x00591b70u, 0x00591dc0u, 1, 0, 0);
    if (result == 0)
        return result;
    cm_fn_007ff270(0x00b74450u, 0, first, 1);
    cm_fn_007ff270(0x00b74450u, 1, b, 1);
    cm_fn_007ff270(0x00b74450u, 2, c, 1);
    value = fourth;
    cm_fn_007ff270(0x00b74450u, 3, value, 0);
    value = (u32)(s32)fifth;
    cm_fn_007ff270(0x00b74450u, 4, value, 0);
    value = (u32)(s32)sixth;
    cm_fn_007ff270(0x00b74450u, 5, value, 0);
    cm_fn_007ff270(0x00b74450u, 6, 0, 0);
    value = seventh;
    return cm_fn_007ff270(0x00b74450u, 7, value, 0);
diagnostic:
    cm_fn_00944cff(0x009c7a48u, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x5e2u);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
