/* Preferred-image VA 009391a0/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00944840(s32, s32);
extern s32 CM_CDECL cm_fn_009446e0(s32, s32);
extern u32 CM_CDECL cm_fn_009447e0(u32, u16);
typedef u32 (__attribute__((cdecl, regparm(3))) *callback_9391a0)(
    u32 eax_value, u32 edx_value, u32 ecx_value, u32 object, u32 x, u32 y);

/* CDECL4/fullEAX: return the cached table pointer, including when both
   coefficient WORDs are zero. The callback has three caller-cleaned stack
   arguments. regparm(3) also carries the EAX/EDX/ECX left by 009391c8-d6,
   including ECX at an unowned indirect target. No callback is substituted.
   9447e0 consumes only its second argument's low WORD. */
u32 CM_CDECL cm_fn_009391a0(u32 self, u32 first_out, u32 second_out, u32 index_out)
{
    u32 index, table, ybase, yother, yfirst, ylast, xother, xbase, xfirst, xlast;
    u32 dx, dy, target, value, quotient, result;
    u16 scale, coefficient;
    if (B(self + 4u) & 1u) {
        index = D(self + 0x34u);
        table = D(self + 8u);
    } else {
        index = D(self + 0x38u);
        table = D(self + 0xcu);
    }
    ybase = D(table + 4u);
    yother = D(table + 0xcu);
    yfirst = D(ybase + index * 4u);
    ylast = D(yother + index * 4u);
    xother = D(table + 8u);
    dy = yfirst - ylast;
    xbase = D(table);
    xfirst = D(xbase + index * 4u);
    xlast = D(xother + index * 4u);
    dx = xfirst - xlast;
    target = D(self + 0x46u);
    value = ((callback_9391a0)target)(xother, dx, xlast, self, dx, dy);
    scale = W(self + 0x40u);
    if (scale != 0x4000u) {
        coefficient = W(self + 0x18u);
        if (coefficient != 0) {
            quotient = cm_fn_00944840((s16)coefficient, (s16)scale);
            result = (u32)cm_fn_009446e0((s32)value, (s32)quotient);
            D(first_out) = result;
        }
        coefficient = W(self + 0x1au);
        if (coefficient != 0) {
            scale = W(self + 0x40u);
            quotient = cm_fn_00944840((s16)coefficient, (s16)scale);
            result = (u32)cm_fn_009446e0((s32)value, (s32)quotient);
            D(second_out) = result;
        }
    } else {
        coefficient = W(self + 0x18u);
        if (coefficient != 0) {
            result = cm_fn_009447e0(value, coefficient);
            D(first_out) = result;
        }
        coefficient = W(self + 0x1au);
        if (coefficient != 0) {
            result = cm_fn_009447e0(value, coefficient);
            D(second_out) = result;
        }
    }
    D(index_out) = index;
    return table;
}
