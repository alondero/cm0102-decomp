#include "cmre.h"
/* Original-instruction ABI evidence: preferred-image VAs below, CM0102
   3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   008cd447 ret 12: stdcall entry. ECX and pushes at 008cd294,
   008cd30f-008cd32b, 008cd365-008cd36c, 008cd382-008cd391,
   008cd396-008cd3a8 and 008cd3d9-008cd3eb supply receivers/arguments.
   No caller cleanup follows these calls; 004d1600 consumes six stack args. */
extern unsigned long long CM_THISCALL cm_fn_0043f460(const int *, u32);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32, u32);
extern u32 CM_THISCALL cm_fn_004d1600(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005a0590(u32, u32);
extern u32 CM_THISCALL cm_fn_00867b50(u32);
extern long long CM_THISCALL cm_fn_008e7b50(u32, u32, u32, u32, u32, u32);

u32 CM_STDCALL cm_fn_008cd240(volatile u32 arg1, u32 arg2, u16 *out)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 self, target, record, object, other, result = 1;
    u32 date;
    s32 amount, value, threshold;
    unsigned long long first, second;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963f00;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    self = arg1;
    target = CM_GLOBAL(volatile u32, 0x00ae23b4);
    target += CM_GLOBAL(volatile u32, self + 8) * 581u;
    record = CM_GLOBAL(volatile u32, self + 12);
    object = CM_GLOBAL(volatile u32, 0x00ae1050);
    record = CM_GLOBAL(volatile u32, 0x00ae23bc) + record * 110u;
    other = cm_fn_004cf5a0(object, record);
    CM_GLOBAL(volatile u32, 0x00ae1080) = other;
    if (!other) goto done;
    if ((CM_GLOBAL(volatile u8, other + 0x35) & 0x3f) == 5) goto done;
    object = CM_GLOBAL(volatile u32, other + 4);
    object = CM_GLOBAL(volatile u32, 0x00ae23b4) + object * 581u;
    if (!object) goto done;
    other = cm_fn_004cf5a0(CM_GLOBAL(volatile u32, 0x00ae1050), record);
    if (cm_fn_00867b50(other) &&
        CM_GLOBAL(volatile u32, self + 4) == 0 &&
        CM_GLOBAL(volatile u8, self + 0x40) == 0) {
        date = CM_GLOBAL(volatile u32, self + 0x1b);
        other = CM_GLOBAL(volatile u32, other + 0x21);
        frame.state = 0;
        arg1 = other;
        *(volatile u8 *)&frame.state = 1;
        first = cm_fn_0043f460((const int *)&arg1, 4);
        second = cm_fn_0043f460((const int *)&date, 4);
        frame.state = 0xffffffffu;
        if ((long long)second >= (long long)first) {
            if (out) *out = 0x1b;
            goto done;
        }
    }
    other = cm_fn_005a0590(CM_GLOBAL(volatile u32, 0x00ae2a38), object);
    result = 0;
    if (CM_GLOBAL(volatile u8, other + 0x165) != 1) goto done;
    amount = (s32)cm_fn_004d1600(CM_GLOBAL(volatile u32, 0x00ae1050),
                                 object, target, record, 0, 1, 1);
    value = (s32)cm_fn_008e7b50(self, 0, 0, 1, 0, 1);
    threshold = (s32)CM_GLOBAL(volatile s16, object + 0x80) * 20;
    if (threshold < 20000) threshold = 20000;
    if (amount < threshold || (s32)((u32)value * 2u) <= amount || value <= 50000)
        goto done;
    if (arg2) {
        arg1 = (u32)cm_fn_008e7b50(arg2, 0, 0, 1, 0, 1);
        if (!((long double)amount <
              (long double)(s32)arg1 * (long double)CM_GLOBAL(double, 0x00968de8)))
            goto done;
    }
    if (out) *out = 0x1c;
    result = 1;
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
