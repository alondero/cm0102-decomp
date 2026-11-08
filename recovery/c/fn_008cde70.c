/* Preferred-image VA 008cde70, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original native helpers remain explicit boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d020(u32 self);
extern void CM_THISCALL cm_fn_0068d040(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_007f7490(u32 self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 key, u32 list, u32 flag);
extern u32 CM_CDECL cm_fn_00600c90(u32 club, u32 skip, u8 flag);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 record);
extern u32 CM_THISCALL cm_fn_004cc9e0(u32 self, u32 first, u32 second);

u32 CM_THISCALL cm_fn_008cde70(u32 self, u32 key, u32 mode)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[16];
    u32 list = (u32)work, value, active, eligible, node, element, index;
    u32 base, record, contract, receiver, answer;
    u8 kind;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963f58u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0068d020(list);
    frame.state = 0;
    if (key != 0) {
        value = D(key);
        if ((s32)value >= (s32)D(0x00ae2364u)) goto no;
    }
    if (mode == 0) {
        index = D(key);
        base = D(self + 0x86eu);
        if ((B(base + index * 8u) & 1u) == 0) goto no;
        goto yes;
    }
    active = (u32)(s32)(s16)W(0x00b77c7au);
    value = D(0x00b74c26u + active * 768u);
    if (value == 0) {
        index = D(key);
        base = D(self + 0x86eu);
        frame.state = 0xffffffffu;
        answer = B(base + index * 8u) & 1u;
        cm_fn_0068d040(list);
        goto done;
    }
    receiver = D(0x00b74448u);
    eligible = cm_fn_007f7490(receiver, key, value);
    cm_fn_008c5cf0(self, key, list, 0);
    value = D(list + 2u);
    D(list + 10u) = value;
    node = cm_fn_0068d1b0(list);
    while (node != 0) {
        index = D(node);
        base = D(self);
        element = D(base + index * 4u);
        kind = B(element + 0x2du);
        if (kind != 0) goto yes;
        index = D(element + 8u);
        base = D(0x00ae23b4u);
        record = base + index * 581u;
        if (cm_fn_00600c90(record, 0, 1) != 0) goto yes;
        index = D(element + 0xcu);
        base = D(0x00ae23bcu);
        record = base + index * 110u;
        receiver = D(0x00ae1050u);
        contract = cm_fn_004cf5a0(receiver, record);
        D(0x00ae1080u) = contract;
        if (contract != 0) {
            kind = B(contract + 0x35u) & 0x3fu;
            if (kind != 5) {
                index = D(contract + 4u);
                base = D(0x00ae23b4u);
                record = base + index * 581u;
                if (record != 0) {
                    index = D(element + 0xcu);
                    base = D(0x00ae23bcu);
                    record = base + index * 110u;
                    receiver = D(0x00ae1050u);
                    contract = cm_fn_004cf5a0(receiver, record);
                    D(0x00ae1080u) = contract;
                    record = 0;
                    if (contract != 0) {
                        kind = B(contract + 0x35u) & 0x3fu;
                        if (kind != 5) {
                            index = D(contract + 4u);
                            base = D(0x00ae23b4u);
                            record = base + index * 581u;
                        }
                    }
                    if (cm_fn_00600c90(record, 0, 1) != 0) goto yes;
                }
            }
        }
        if (eligible != 0) goto yes;
        node = cm_fn_0068d1b0(list);
    }
    active = (u32)(s32)(s16)W(0x00b77c7au);
    value = D(0x00b74c26u + active * 768u);
    value = D(value + 0x39u);
    if (value != 0) {
        value = D(value);
        index = D(key);
        receiver = D(0x00ae1050u);
        contract = cm_fn_004cc9e0(receiver, index, value);
        if (contract != 0) {
            kind = B(contract + 0x35u) & 0x3fu;
            if (kind == 5) goto yes;
        }
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
