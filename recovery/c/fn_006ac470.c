/* Preferred-image VA 006ac470, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole indexed caller0069ABEC consumes AL at0069ABF1. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 target);
extern s32 CM_THISCALL cm_fn_0054cdb0(u32 self, u32 low, u32 high);
extern u32 CM_THISCALL cm_fn_004a4850(u32 self, u32 target);
/* This caller discards all incidental EDX:EAX results of the original append.
   Its own instructions do not consume incoming EDX; no value is fabricated. */
extern void CM_THISCALL cm_fn_0068d0d0(u32 self, u32 source, u32 size);

u8 CM_THISCALL cm_fn_006ac470(u32 self, u32 target)
{
    volatile u8 message[8];
    u32 list, head, index = 0, value, id, other_id, person, high, low;
    u32 club, table, metadata, receiver, entry, queue, flags, address;
    u16 count;
    u8 mode, current;
    list = D(self + 0x10u);
    count = W(list);
    if (count == 0) return 0;
    head = D(list + 2u);
    D(list + 0xau) = head;
    list = D(self + 0x10u);
    count = W(list);
    if ((s16)count <= 0) return 0;
    for (;;) {
        value = cm_fn_0068d1b0(list);
        if (value != 0) {
            id = D(target);
            other_id = D(value);
            if (id == other_id) {
                list = D(self + 0x10u);
                cm_fn_0068d1f0(list, value);
                person = D(target + 0xcfu);
                if (person != 0) {
                    high = D(person + 0x42u);
                    low = D(person + 0x3eu);
                    if (cm_fn_0054cdb0(0x00ae2c90u, low, high) > 100) break;
                }
            }
        }
        list = D(self + 0x10u);
        ++index;
        count = W(list);
        if ((s32)index >= (s32)(s16)count) return 0;
    }
    id = D(target);
    B((u32)message) = 1;
    D((u32)message + 2u) = id;
    mode = 2;
    if (target != 0) {
        club = D(target + 0x57u);
        if (club != 0) {
            id = D(club);
            table = D(0x00adadfcu);
            metadata = D(table + id * 4u);
            if (metadata != 0) {
                receiver = D(metadata + 8u);
                if (receiver != 0) {
                    entry = cm_fn_004a4850(receiver, target);
                    if (entry == 0 || (s32)D(entry + 0x55u) <= 0) mode = 7;
                }
            }
        }
    }
    B((u32)message + 1u) = mode;
    queue = D(self + 4u);
    cm_fn_0068d0d0(queue, (u32)message, 6);
    id = D(target);
    flags = D(self);
    address = flags + id * 73u + 2u;
    current = B(address);
    B(address) = current | 2u;
    return 1;
}
