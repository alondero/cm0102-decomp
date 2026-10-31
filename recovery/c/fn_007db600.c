#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007e82e0(u32, u32, u32);
extern void CM_FASTCALL cm_fn_0054a110(u32);
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00556940(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00557280(u32, u32, u32, u32);

/* Incoming ECX is forwarded to007e82e0 even though this body does not
   otherwise read it. No direct callers prove EAX dead; retain full EAX. */
u32 CM_THISCALL cm_fn_007db600(u32 incoming, u32 record, volatile u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x98];
    u32 local = (u32)scratch;
    u32 person_id, lookup, pointer, context, base, other, result;
    u8 flag, counter, severity, action, category, bits;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095fddb;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    B(local + 2) = 0;
    B(local + 3) = 0xff;
    flag = B(record + 0x2c);
    if (flag == 0) {
        flag = B(record + 0x2d);
        if (flag == 0) {
            result = frame.prev & 0xffffff00u;
            goto done;
        }
    }
    person_id = D(record);
    lookup = cm_fn_007e82e0(incoming, person_id, 1);
    flag = B(record + 0x2c);
    if (flag == 1) {
        counter = B(lookup + 4);
        counter = (u8)(counter + 1u);
        severity = counter;
        B(lookup + 4) = counter;
        if (counter == 5 || (counter >= 7 && (((u32)counter - 5u) & 1u) == 0)) {
            B(local + 2) = 1;
            B(local + 3) = 1;
        } else {
            severity = B(local + 3);
        }
    } else {
        severity = B(local + 3);
    }
    if (B(record + 0x2d) == 1) {
        cm_fn_0054a110(local + 0x2c);
        frame.state = 0;
        cm_fn_0054a110(local + 0x3c);
        frame.state = 1;
        cm_fn_0054a110(local + 0x18);
        counter = B(lookup + 5);
        counter = (u8)(counter + 1u);
        B(lookup + 5) = counter;
        action = B(local + 2);
        action = (u8)(action + (counter < 3 ? 1u : 2u));
        B(local + 2) = action;
        B(local + 3) = 2;
        severity = 1;
        frame.state = 0xffffffffu;
    }
    action = B(local + 2);
    if (action == 0) {
        result = lookup & 0xffffff00u;
        goto done;
    }
    cm_fn_0054a110(local + 8);
    action = B(local + 2);
    frame.state = 2;
    B(local + 4) = action;
    W(local + 6) = 0xffff;
    B(local + 5) = 0;
    cm_fn_00417c10(local + 8, local + 0x20, 0x00ae2c90);
    bits = B(local + 0x17);
    category = B(local + 3);
    flag = B(record + 0x48);
    bits = (u8)(bits & 0xf9u);
    if (flag == 0)
        bits = (u8)(bits | 4u);
    B(local + 0x14) = category;
    W(local + 0x15) = (u16)(s16)(s8)severity;
    person_id = D(record);
    B(local + 0x17) = bits;
    context = D(0x00ae28f0);
    B(local + 0x10) = 0x22;
    B(local + 0x11) = 0xff;
    B(local + 0x12) = 0xff;
    B(local + 0x13) = 0xff;
    cm_fn_00556940(context, person_id, local + 4);
    other = argument;
    person_id = D(record);
    base = D(0x00ae23bc);
    pointer = base + person_id * 110u;
    context = D(0x00ae28f0);
    result = cm_fn_00557280(context, pointer, local + 4, other);
done:
    CM_FS(u32, 0) = frame.prev;
    return result;
}
