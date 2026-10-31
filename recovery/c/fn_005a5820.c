/* Preferred-image VA 005a5820, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller's next routine writes EAX before use. The final removal
   intentionally uses the first list's saved node, even on the second-list path. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_FASTCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 low, u32 high);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern u8 CM_THISCALL cm_fn_0062c400(u32 self, u32 club);
extern void CM_STDCALL cm_fn_005a7f20(u32 first, u32 second, s8 code);

void CM_THISCALL cm_fn_005a5820(u32 self)
{
    u8 work[4];
    u32 list, cursor, index, node, low, high, result, first, second, clubs, id, receiver;
    s32 limit, age;
    list = D(self);
    cursor = D(list + 2u);
    D(list + 0xau) = cursor;
    list = D(self);
    index = 0;
    limit = (s16)W(list);
    if (limit > 0) {
        do {
            node = cm_fn_0068d1b0(list);
            *(volatile u32 *)work = node;
            if (node == 0)
                return;
            high = D(node + 0xau);
            low = D(node + 6u);
            result = cm_fn_00456740(0x00ae2c90u, low, high);
            if (result != 0) {
                list = D(self);
                cm_fn_0068d1f0(list, node);
                break;
            }
            list = D(self);
            index += 1u;
            limit = (s16)W(list);
        } while ((s32)index < limit);
    }
    list = D(self + 4u);
    cursor = D(list + 2u);
    D(list + 0xau) = cursor;
    list = D(self + 4u);
    index = 0;
    limit = (s16)W(list);
    if (limit <= 0)
        return;
    do {
        node = cm_fn_0068d1b0(list);
        if (node == 0)
            return;
        high = D(node + 0xdu);
        low = D(node + 9u);
        result = cm_fn_00456740(0x00ae2c90u, low, high);
        if (result != 0)
            goto expired;
        list = D(self + 4u);
        index += 1u;
        limit = (s16)W(list);
    } while ((s32)index < limit);
    return;

expired:
    id = D(node);
    clubs = D(0x00ae23bcu);
    first = clubs + id * 110u;
    id = D(node + 4u);
    low = D(first + 0x3eu);
    second = clubs + id * 110u;
    high = D(first + 0x42u);
    age = cm_fn_0054cdb0(0x00ae2c90u, low, high);
    if (age > 30) {
        receiver = D(0x00b63d28u);
        result = cm_fn_0062c400(receiver, first);
        if ((u8)result != 2)
            cm_fn_005a7f20(first, second, (s8)B(node + 8u));
    }
    /* Uninitialized if the first list had no iterations; do not invent a node. */
    cursor = *(volatile u32 *)work;
    list = D(self + 4u);
    cm_fn_0068d1f0(list, cursor);
}
