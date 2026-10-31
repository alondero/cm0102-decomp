#include "cmre.h"

/* Preferred-image VA 0x00797b40, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Full EAX is retained on AL-only exits. 549ef0 masks the year DWORD
   to its low WORD before use; the incoming EDX high WORD is not consumed. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
extern u32 CM_THISCALL cm_fn_00549ef0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00798220(u32);
extern u32 CM_THISCALL cm_fn_007978c0(u32);
extern u32 CM_THISCALL cm_fn_006835c0(u32);
extern u32 CM_THISCALL cm_fn_006827d0(u32, u32);
extern u32 CM_THISCALL cm_fn_00797e80(u32);
extern void CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_THISCALL cm_fn_006847c0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00797b40(u32 self, u32 date, volatile u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x218];
    u32 local = (u32)scratch, prev, low, high, day_arg, result, target_date;
    u32 value, vtable, target, name, ext, line, text;
    u16 year_arg, target_year, input_year, next_year;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095eb16;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    low = D(date);
    high = D(date + 4);
    D(local + 0x10) = low;
    D(local + 0x14) = high;
    year_arg = W(self + 0xb8);
    day_arg = (low & 0xffff0000u) | W(self + 0xb6);
    year_arg = (u16)(year_arg + W(self + 0x40));
    frame.state = 0;
    result = cm_fn_00549ef0(local + 8, day_arg, year_arg);
    low = D(date);
    high = D(date + 4);
    target_year = W(local + 10);
    result = (result & 0xffff0000u) | target_year;
    D(local) = low;
    D(local + 4) = high;
    input_year = W(local + 2);
    *(volatile u8 *)&frame.state = 1;
    if (input_year < target_year) goto false_byte;
    target_date = D(local + 8);
    if (input_year == target_year && (s16)low < (s16)target_date) goto false_byte;
    low = D(date);
    high = D(date + 4);
    D(local) = low;
    next_year = W(local + 2);
    D(local + 4) = high;
    if (next_year != target_year || (u16)low != (u16)target_date) goto generic;
    if (D(self + 0x45) != 0 || mode != 0) goto generic;
    cm_fn_00798220(self);
    vtable = D(self);
    target = D(vtable + 0x5c);
    ((u32 (CM_THISCALL *)(u32))target)(self);
    value = cm_fn_007978c0(self);
    if (value == 0) {
        name = local + 0x118; ext = local + 0x18; line = 0x21d;
        goto error;
    }
    value = cm_fn_006835c0(self);
    if (value == 0) {
        name = local + 0x18; ext = local + 0x118; line = 0x224;
        goto error;
    }
    value = cm_fn_006827d0(self, 0);
    if (value == 0) {
        name = local + 0x18; ext = local + 0x118; line = 0x22b;
        goto error;
    }
    cm_fn_00797e80(self);
    value = D(self + 0xee);
    cm_fn_009452ca(value);
    D(self + 0xee) = 0;
    D(self + 0xf2) = 0;
    D(self + 0x45) = 1;
generic:
    result = cm_fn_006847c0(self, date, mode);
    goto done;
error:
    cm_fn_00944cff(0x00a6a068, 0, 0, name, ext);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, ext, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = (result & 0xffffff00u) | 1u;
    goto done;
false_byte:
    result &= 0xffffff00u;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
