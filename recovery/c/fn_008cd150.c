/* Preferred-image VA 008cd150, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original helpers remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 key, u32 list, u32 flag);
extern u32 CM_THISCALL cm_fn_008cde70(u32 self, u32 key, u32 mode);

u32 CM_THISCALL cm_fn_008cd150(u32 self, u32 key)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[16];
    u32 list = (u32)work, value, count, address, element, answer;
    u8 kind;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963ed8u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0068d020(list);
    frame.state = 0;
    if (cm_fn_008cde70(self, key, 0) == 0) goto no;
    cm_fn_008c5cf0(self, key, list, 0);
    value = D(list + 2u);
    D(list + 10u) = value;
    value = cm_fn_0068d1b0(list);
    while (value != 0) {
        value = D(value);
        count = D(self + 4u);
        if ((s32)value < (s32)count) {
            address = D(self);
            address += value * 4u;
        } else {
            address = D(self);
        }
        element = D(address);
        kind = B(element + 0x2cu);
        if (kind == 0 || kind == 1 || kind == 5 || kind == 6) goto yes;
        value = cm_fn_0068d1b0(list);
    }
no:
    frame.state = 0xffffffffu;
    cm_fn_0068d040(list);
    answer = 0;
    goto done;
yes:
    frame.state = 0xffffffffu;
    cm_fn_0068d040(list);
    answer = 1;
done:
    value = frame.prev;
    CM_FS(u32, 0) = value;
    return answer;
}
