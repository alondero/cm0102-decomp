#include "cmre.h"

/* Preferred-image VA 0x0058c4b0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   No indexed direct callers: retain full EAX, including XOR AL-only exits. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0058c6b0(u32);
extern u32 CM_THISCALL cm_fn_0058bcb0(u32);
extern u32 CM_FASTCALL cm_fn_006835c0(u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern u32 CM_FASTCALL cm_fn_0058c100(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_006847c0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
typedef u32 (CM_THISCALL *SetupCall)(u32);

u32 CM_THISCALL cm_fn_0058c4b0(u32 self, volatile u32 first, volatile u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x210];
    u32 local = (u32)scratch, previous, year_arg, day_arg, date, low, high;
    u32 result, target_date, value, vtable, target, mode, name, extension, line, text;
    u16 input_year, target_year, next_year;
    frame.state = 0xffffffffu;
    frame.handler = 0x00958deb;
    previous = CM_FS(u32, 0);
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    year_arg = (previous & 0xffff0000u) | W(self + 0xb8);
    day_arg = (self & 0xffff0000u) | W(self + 0xb6);
    year_arg = (year_arg & 0xffff0000u) | (u16)((u16)year_arg + W(self + 0x40));
    cm_fn_00549ef0(local + 8, day_arg, year_arg);
    date = first;
    frame.state = 0;
    low = D(date);
    high = D(date + 4);
    D(local) = low;
    input_year = W(local + 2);
    D(local + 4) = high;
    target_year = W(local + 0xa);
    result = (high & 0xffff0000u) | target_year;
    if (input_year < target_year) goto false_low_byte;
    target_date = D(local + 8);
    if (input_year == target_year && (s16)low < (s16)target_date) goto false_low_byte;
    low = D(date);
    high = D(date + 4);
    D(local) = low;
    next_year = W(local + 2);
    D(local + 4) = high;
    if (next_year != target_year || (u16)low != (u16)target_date) goto generic;
    if (D(self + 0x45) != 0) goto generic;
    mode = second;
    if (mode != 0) goto generic;
    cm_fn_0058c6b0(self);
    vtable = D(self);
    target = D(vtable + 0x5c);
    ((SetupCall)target)(self);
    value = cm_fn_0058bcb0(self);
    if (value == 0) {
        name = local + 0x110; extension = local + 0x10; line = 0x32b;
        goto error;
    }
    value = cm_fn_006835c0(self);
    if (value == 0) {
        name = local + 0x10; extension = local + 0x110; line = 0x332;
        goto error;
    }
    value = cm_fn_006827d0(self, 0);
    if (value == 0) {
        name = local + 0x10; extension = local + 0x110; line = 0x339;
        goto error;
    }
    cm_fn_0058c100(self);
    value = D(self + 0xee);
    cm_fn_009452ca(value);
    D(self + 0xee) = 0;
    D(self + 0xf2) = 0;
    D(self + 0x45) = 1;
generic:
    mode = second;
    result = cm_fn_006847c0(self, date, mode);
    goto done;
error:
    cm_fn_00944cff(0x009c7474, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
false_low_byte:
    result &= 0xffffff00u;
done:
    value = frame.prev;
    CM_FS(u32, 0) = value;
    return result;
}
