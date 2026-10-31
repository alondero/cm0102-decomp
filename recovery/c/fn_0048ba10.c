#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_STDCALL cm_fn_0048ba10(u32 first, u32 second)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch;
    u32 details, other, a, b, base, value, sum, name, extension, line, text;
    s16 ability, rating;
    if (first == 0)
        goto first_error;
    details = D(first + 0x69);
    if (details == 0)
        goto first_error;
    if (second == 0)
        goto second_error;
    other = D(second + 0x61);
    if (other == 0)
        goto second_error;
    ability = (s16)W(other + 5);
    if (ability > 0xa0)
        return 0;
    if ((s16)W(other + 7) > 0xa0 && ability > 0x64)
        return 0;
    rating = (s16)W(details + 4);
    if (rating > 0x8c)
        base = 5;
    else
        base = (((u32)(rating <= 0x50) - 1) & 0xfffffffbu) + 0xf;
    other = D(0x00ae1050);
    if (cm_fn_004cf6d0(other, second) != 0) {
        other = D(0x00ae1050);
        value = cm_fn_004cf5a0(other, second);
        D(0x00ae1080) = value;
        if (value != 0) {
            a = D(value + 4);
            b = D(0x00ae23b4);
            if (b + a * 0x245 != 0)
                return base;
        }
    }
    a = D(first);
    b = D(second);
    other = D(0x00ae1050);
    sum = a + b;
    value = cm_fn_004cf5a0(other, second);
    D(0x00ae1080) = value;
    if (value != 0 && (B(value + 0x35) & 0x3f) == 5)
        return (u32)((s32)sum % 10);
    return (u32)((s32)sum % 0x23) + base + 0x1e;
second_error:
    name = local + 0x100;
    extension = local;
    line = 0xac1;
    goto error;
first_error:
    name = local;
    extension = local + 0x100;
    line = 0xabc;
error:
    cm_fn_00944cff(0x0099b3e0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return 0xb4;
}
