/* Preferred-image VA 008c1e40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0085e410(void *object);
/* Seven stack words, ret28; rate_low/rate_high are native IEEE double1.0. */
extern u32 CM_THISCALL cm_fn_004cda70(u32 self, u32 person, u32 club, void *object, u32 value, u32 rate_low, u32 rate_high, u32 mode);
extern u32 CM_THISCALL cm_fn_004cc490(u32 self, void *object, u32 first, u32 second, u32 mode, u32 zero, u32 one, u32 last);
extern u32 CM_THISCALL cm_fn_00849ce0(u32 self, u32 first, u32 second, u32 flag, u32 zero);
extern u32 CM_THISCALL cm_fn_00843e10(u32 self, u32 second, u32 first, u32 one, u32 flag);
extern u32 CM_THISCALL cm_fn_008bfcb0(u32 self, volatile u32 *record, u32 zero, u32 mode);
extern u32 CM_THISCALL cm_fn_008c41c0(u32 self, u32 person, u32 record, u32 one, u32 mode);

u32 CM_THISCALL cm_fn_008c1e40(u32 self, u32 first, u32 second, volatile u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x50];
    u32 pointer, value, other, receiver, result, id, flag, amount;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963bc8u;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0085e410(work);
    pointer = record;
    value = D(pointer + 0x20u);
    receiver = D(0x00ae1050u);
    frame.state = 0;
    result = cm_fn_004cda70(receiver, second, first, work, value, 0, 0x3ff00000u, 2);
    if (result != 0) {
        pointer = record;
        other = D(pointer + 0x24u);
        value = D(pointer + 0x20u);
        receiver = D(0x00ae1050u);
        result = cm_fn_004cc490(receiver, work, value, other, 11, 0, 1, 0);
    } else {
        pointer = record;
        id = D(second);
        other = D(pointer + 12u);
        if (id == other) {
            flag = B(pointer + 0x11u) != 11;
            receiver = D(self + 0x856u);
            pointer = cm_fn_00849ce0(receiver, first, second, flag, 0);
            if (pointer != 0) {
                value = B(pointer);
                receiver = D(self + 0x856u);
                flag = (pointer & 0xffffff00u) | (value != 11);
                cm_fn_00843e10(receiver, second, first, 1, flag);
            }
            result = cm_fn_008bfcb0(self, &record, 0, 0x16u);
        } else {
            amount = D(second + 0x52u);
            value = D(pointer + 0x1bu);
            D(pointer + 0x1bu) = value + amount;
            pointer = record;
            receiver = D(0x00de1f54u);
            result = cm_fn_008c41c0(receiver, second, pointer, 1, 0x16u);
        }
    }
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
