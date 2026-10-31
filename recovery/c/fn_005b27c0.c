/* Preferred-image VA 005b27c0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_005b2ba0(u32 self, u32 index, u32 position, u32 option);
extern u32 CM_THISCALL cm_fn_005b7160(u32 self);

u32 CM_THISCALL cm_fn_005b27c0(u32 self)
{
    u32 i, scan, remaining, length, source, destination, value;
    u8 byte;
    for (i = 0; i < 0x638u; ++i)
        D(self + i * 4u) = 0;
    W(self + 0x18e0u) = 0;
    B(self + 0x18e2u) = 0;
    scan = 0x009cc3a8u;
    remaining = 0xffffffffu;
    do {
        byte = B(scan);
        scan += 1u;
        remaining -= 1u;
    } while (byte != 0 && remaining != 0);
    length = ~remaining;
    source = scan - length;
    destination = self;
    for (i = length >> 2; i != 0; --i) {
        value = D(source);
        D(destination) = value;
        source += 4u;
        destination += 4u;
    }
    for (i = length & 3u; i != 0; --i) {
        byte = B(source);
        B(destination) = byte;
        source += 1u;
        destination += 1u;
    }
    B(self + 0x33u) = 0;
    B(self + 0x65u) = 0;
    D(self + 0x143u) = 0;
    D(self + 0x147u) = 0;
    W(self + 0x14bu) = 0;
    B(self + 0x14du) = 0;
    cm_fn_005b2ba0(self, 0, 1, 0);
    cm_fn_005b2ba0(self, 1, 0x804, 0);
    cm_fn_005b2ba0(self, 2, 0x84, 0);
    cm_fn_005b2ba0(self, 3, 0x104, 0);
    cm_fn_005b2ba0(self, 4, 0x404, 0);
    cm_fn_005b2ba0(self, 5, 0x110, 0);
    cm_fn_005b2ba0(self, 6, 0x810, 0);
    cm_fn_005b2ba0(self, 7, 0x410, 0);
    cm_fn_005b2ba0(self, 8, 0x140, 0);
    cm_fn_005b2ba0(self, 9, 0x440, 0);
    cm_fn_005b2ba0(self, 10, 0x90, 0);
    source = self + 0x188bu;
    D(self + 0x584u) = 0x22a91u;
    D(source) = 0x15552221u;
    D(source + 4u) = 10;
    destination = source + 8u;
    /* The original forward copy propagates the first two DWORDs. */
    for (i = 0; i < 20; ++i) {
        value = D(source);
        D(destination) = value;
        source += 4u;
        destination += 4u;
    }
    D(self + 0x5b4u) = 0x12121212u;
    D(self + 0x5b8u) = 0x12121212u;
    W(self + 0x5bcu) = 0x1212;
    B(self + 0x5beu) = 0x12;
    for (i = 0; i < 11; ++i)
        D(self + 0x5bfu + i * 4u) = 0xffffffffu;
    cm_fn_005b7160(self);
    D(self + 0x187fu) = 1;
    D(self + 0x1883u) = 0;
    D(self + 0x1887u) = 0;
    return self;
}
