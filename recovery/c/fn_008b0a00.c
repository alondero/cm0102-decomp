/* Preferred-image VA 008b0a00, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_STDCALL cm_fn_009463af(u32 destination, u32 size, u32 count, u32 constructor, u32 destructor);
extern void CM_FASTCALL cm_fn_0068d020(u32 list);
extern u32 CM_THISCALL cm_fn_008b3410(u32 self, u32 club, u32 output);
extern u32 CM_THISCALL cm_fn_008b3930(u32 self, u32 value, u32 kind);
extern u32 CM_THISCALL cm_fn_008b35a0(u32 self, u32 value, u32 destination);

u32 CM_THISCALL cm_fn_008b0a00(u32 self, u32 club)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0xc];
    u32 predecessor, receiver, record, value, kind, source, destination, slot, i;
    u8 index;
    u16 word;
    u8 byte;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963836u;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    destination = self + 0xfau;
    D((u32)work + 8u) = self;
    cm_fn_009463af(destination, 0x9bu, 11, 0x008b4e10u, 0x005a53c0u);
    frame.state = 0;
    cm_fn_0068d020(self + 0x80au);
    D(self + 0x806u) = club;
    receiver = D(0x00de1f44u);
    CM_GLOBAL(volatile u8, (u32)&frame.state) = 1;
    cm_fn_008b3410(receiver, club, self);
    index = 0;
    B((u32)work) = index;
    D((u32)work + 4u) = self + 0x7dau;
    do {
        record = D(self + 0x806u);
        kind = D((u32)work);
        receiver = D(0x00de1f44u);
        value = D(record + 0xcfu);
        source = cm_fn_008b3930(receiver, value, kind);
        slot = D((u32)work + 4u);
        for (i = 0; i < 0x26u; ++i) {
            value = D(source + i * 4u);
            D(destination + i * 4u) = value;
        }
        ++index;
        word = W(source + 0x98u);
        W(destination + 0x98u) = word;
        byte = B(source + 0x9au);
        B(destination + 0x9au) = byte;
        D(slot) = 0;
        destination += 0x9bu;
        slot += 4;
        B((u32)work) = index;
        D((u32)work + 4u) = slot;
    } while ((s8)index < 10);
    record = D(self + 0x806u);
    receiver = D(0x00de1f44u);
    value = D(record + 0xcfu);
    cm_fn_008b35a0(receiver, value, self + 0x7a3u);
    predecessor = frame.prev;
    D(self + 0x802u) = 0;
    B(self + 0x7d9u) = 0;
    CM_FS(volatile u32, 0) = predecessor;
    return self;
}
