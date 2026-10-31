#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(u32);
extern void CM_THISCALL cm_fn_0068d020(u32);
extern void CM_THISCALL cm_fn_0068d040(u32);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern void CM_THISCALL cm_fn_004cf560(u32, u32);
extern u32 CM_THISCALL cm_fn_008cee70(u32, u32, u32);
extern void CM_THISCALL cm_fn_008c41c0(u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_008c47d0(u32, u32);
/* 008e05d0 forwards incoming ECX to 008d1130 at VA008e061b. */
extern void CM_THISCALL cm_fn_008e05d0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008cb9a0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008c6220(u32, u32, u32, u32);

u32 CM_THISCALL cm_fn_008c3310(u32 self, u32 record, u32 other)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x18];
    u32 local = (u32)scratch;
    u32 eax, value, current, index, base, person, list, node, context;
    u16 current_year, target_year;
    u8 status;
    int matching, earlier;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963c10;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0054a110(local);
    eax = other;
    frame.state = 0;
    matching = 0;
    if (eax != 0) {
        eax = D(eax);
        value = D(record + 8);
        matching = eax == value;
    }
    status = B(record + 0x2c);
    /* Every status load in this branch is MOV AL: wrappers can forward EAX. */
    eax = (eax & 0xffffff00u) | status;
    if (matching) {
        if (status == 11)
            B(record + 0x2c) = 13;
        else if (status == 12)
            B(record + 0x2c) = 14;
    } else {
        if (status == 11)
            B(record + 0x2c) = 12;
        else if (status == 13)
            B(record + 0x2c) = 14;
    }
    if (B(record + 0x2c) != 14)
        goto done;
    if ((B(record + 0x30) & 15) == 0)
        goto tick;
    cm_fn_008cee70(self, record, local);
    current = D(0x00ae2c90);
    value = D(0x00ae2c94);
    D(local + 8) = current;
    current_year = W(local + 10);
    D(local + 12) = value;
    target_year = W(local + 2);
    earlier = current_year < target_year;
    if (current_year == target_year)
        earlier = (s16)current < (s16)W(local);
    if (!earlier)
        goto tick;
    /* The list at L+8 replaces the copied current-date scratch. */
    cm_fn_0068d020(local + 8);
    index = D(record + 0xc);
    base = D(0x00ae23bc);
    *(volatile u8 *)&frame.state = 1;
    person = base + index * 110u;
    cm_fn_008c41c0(self, person, record, 0, 0);
    list = D(record + 4);
    if (list != 0) {
        value = D(list + 2);
        D(list + 10) = value;
        list = D(record + 4);
        node = cm_fn_0068d1b0(list);
        while (node != 0) {
            index = D(node);
            base = D(0x00ae23bc);
            person = base + index * 110u;
            cm_fn_008c41c0(self, person, record, 0, 0);
            index = D(node);
            base = D(0x00ae23bc);
            person = base + index * 110u;
            cm_fn_008c47d0(self, person);
            list = D(record + 4);
            node = cm_fn_0068d1b0(list);
        }
    }
    *(volatile u8 *)&frame.state = 0;
    cm_fn_0068d040(local + 8);
    index = D(record + 0xc);
    context = D(0x00ae1050);
    cm_fn_004cf560(context, index);
    cm_fn_008e05d0(self, record, local);
    eax = cm_fn_008cb9a0(self, record, local);
    goto done;
tick:
    eax = cm_fn_008c6220(self, record, 0, 1);
done:
    CM_FS(u32, 0) = frame.prev;
    return eax;
}
