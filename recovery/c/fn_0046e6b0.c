/* Preferred-image VA 0046e6b0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_0046e6b0(u32 first, volatile u32 second, volatile u32 third)
{
    u8 work[0x200];
    u32 other, result, index, record, selected, id, total, value, version;
    if (first == 0)
        goto diagnostic;
    other = second;
    if (other == 0)
        goto diagnostic;
    result = cm_fn_007fe600(0x00b74450u, 0x0046e820u, 0x0046ffd0u, 0, 0, 0);
    if (result == 0)
        return result;
    index = (u32)(s32)(s16)W(0x00b77c7au);
    record = D(0x00b74c26u + index * 768u);
    if (record != 0) {
        id = D(record);
        total = D(0x00ae2364u);
        selected = 0x00adadf0u + (id - total) * 42u;
    } else {
        selected = 0x00adab50u;
    }
    cm_fn_007ff270(0x00b74450u, 0, first, 0);
    cm_fn_007ff270(0x00b74450u, 1, other, 0);
    cm_fn_007ff270(0x00b74450u, 2, 0x221, 0);
    cm_fn_007ff270(0x00b74450u, 3, selected, 0);
    cm_fn_007ff270(0x00b74450u, 4, 0, 0);
    cm_fn_007ff270(0x00b74450u, 5, 0, 0);
    value = third;
    return cm_fn_007ff270(0x00b74450u, 6, value, 0);
diagnostic:
    cm_fn_00944cff(0x0098e598u, 0, 0, work + 0x100, work);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x100, work, 0x2dd0u);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
