#include "cmre.h"
/* Preferred-image VA 0043f4e0, image base00400000; primary3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native thiscall/one DWORD flags/RET4. EDX:EAX is fully defined by final
   original FTOL lowering; store only lowDWORD through the original receiver.
   All integer helpers remain calls to original native functions. */
extern unsigned long long CM_CDECL cm_fn_004426e0(u32 low, u32 high, u32 flags);
extern unsigned long long CM_STDCALL cm_fn_00945e80(u32 low, u32 high,
                                                  u32 divisor_low, u32 divisor_high);
extern unsigned long long CM_STDCALL cm_fn_00945e40(u32 low, u32 high,
                                                  u32 multiplier_low, u32 multiplier_high);

__attribute__((target("no-sse3")))
unsigned long long CM_THISCALL cm_fn_0043f4e0(u32 self, u32 flags)
{
    long double value_fp;
    unsigned long long value, quantum, half, quotient, selected;
    value_fp = (long double)*(volatile s32 *)self;
    value_fp *= (long double)CM_GLOBAL(volatile double, 0x00ad9cb8u);
    /* Original __ftol site 0043f4f4. */
    value = (unsigned long long)(long long)value_fp;
    if ((flags & 0x20u) == 0u) {
        quantum = cm_fn_004426e0((u32)value, (u32)(value >> 32), flags);
        half = cm_fn_00945e80((u32)quantum, (u32)(quantum >> 32), 2u, 0u);
        value += half; /* Original ADD/ADC wraps the full64 bits. */
        quotient = cm_fn_00945e80((u32)value, (u32)(value >> 32),
                                  (u32)quantum, (u32)(quantum >> 32));
        value = cm_fn_00945e40((u32)quotient, (u32)(quotient >> 32),
                               (u32)quantum, (u32)(quantum >> 32));
    }
    selected = value + 1ull;
    quantum = cm_fn_004426e0((u32)selected, (u32)(selected >> 32), flags);
    value += quantum; /* Original ADD/ADC or SUB/SBB, modulo2^64. */
    value_fp = (long double)(long long)value;
    value_fp /= (long double)CM_GLOBAL(volatile double, 0x00ad9cb8u);
    /* Original __ftol site 0043f56e. */
    value = (unsigned long long)(long long)value_fp;
    *(volatile u32 *)self = (u32)value;
    return value;
}
