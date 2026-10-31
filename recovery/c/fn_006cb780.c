/* WIP: passing byte-width tests do not prove the full return contract.
   Caller 006bc446 keeps upper EAX bits through MOVZX AX at006bc48a,
   then LEA EAX at006bc4a8 contributes to EBP arithmetic. Recover the
   incidental EAX alongside ST0 from original floating callees before retry.
   Preferred-image VA 006cb780, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_FASTCALL cm_fn_0070dfa0(u32);
extern long double CM_FASTCALL cm_fn_006fa1e0(u32);
extern u8 CM_FASTCALL cm_fn_006f4650(u32);
extern u32 CM_FASTCALL cm_fn_00717410(u32);
extern long double CM_THISCALL cm_fn_006f09b0(u32, u32, u32);

u8 CM_THISCALL cm_fn_006cb780(u32 self, u32 person)
{
    if (cm_fn_0070dfa0(person)) return 0;
    u32 a = (u32)(s32)(s8)B(person + 0x102);
    u32 b = (u32)(s32)(s8)B(person + 0x103);
    u8 flag = B(person + 0x27);
    u32 offset = (a * 12u + b) * 90u;
    if (B(self + (flag == 0) + offset + 0x21b6)) return 0;
    long double value = cm_fn_006fa1e0(person);
    double extra = CM_GLOBAL(volatile double, 0x00968968);
    volatile float threshold = (float)(value + (long double)extra);
    if (!cm_fn_006f4650(person)) {
        float previous = threshold;
        double addition = CM_GLOBAL(volatile double, 0x00967890);
        threshold = (float)((long double)previous + (long double)addition);
    }
    flag = B(person + 0x27);
    u32 bank = flag == 0;
    u32 entry = self + bank * 8920u + 0x4796;
    for (u32 i = 0; i < 20; ++i, entry += 0x1be) {
        if ((s8)B(entry + 0x19) < 0) continue;
        if ((u16)cm_fn_00717410(entry) == 1) continue;
        flag = B(person + 0x27);
        value = cm_fn_006f09b0(entry, 4, flag ? 0 : 11);
        if (!(value > (long double)threshold)) return 0;
    }
    bank = (u32)(s32)(s8)B(person + 0x27);
    entry = self + bank * 8920u + 0x4796;
    for (u32 i = 0; i < 20; ++i, entry += 0x1be) {
        if ((s8)B(entry + 0x19) < 0) continue;
        if ((u16)cm_fn_00717410(entry) == 1) continue;
        flag = B(person + 0x27);
        value = cm_fn_006f09b0(entry, 4, flag ? 0 : 11);
        if (!(value >= (long double)threshold)) return 2;
    }
    return 1;
}
