/* WIP: semantic import-order mismatch in downstream original cleanup, case120.
   See the shutdown-stale-stack-attributes work hold; do not tune the C frame. */
#include "cmre.h"
/* Preferred-image VA 0x005cc6d0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The second construction leaves SEH state one until the frame is unlinked. */
extern u32 CM_FASTCALL cm_fn_005d72c0(u32);
extern u32 CM_FASTCALL cm_fn_005ef0b0(u32);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_CDECL cm_fn_009532a6(u32);
extern u32 CM_CDECL cm_fn_00945a60(u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_005cc6d0(u32 filename)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 prev, source, destination, remaining, count, words, value, allocation;
    u8 byte;
    prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095a3e6;
    frame.prev = prev;
    CM_FS(u32, 0) = (u32)&frame;
    source = filename ? filename : 0x00ad9c64;
    remaining = 0xffffffffu;
    do {
        byte = B(source);
        ++source;
        --remaining;
    } while (remaining && byte);
    count = ~remaining;
    source -= count;
    destination = 0x00ae2c98;
    words = count >> 2;
    while (words) {
        value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
        --words;
    }
    remaining = count & 3;
    while (remaining) {
        byte = B(source);
        B(destination) = byte;
        ++source;
        ++destination;
        --remaining;
    }
    cm_fn_009532a6(0x00ae2c98);
    allocation = (u32)cm_fn_00944cf1(0x31b);
    frame.state = 0;
    value = allocation ? cm_fn_005d72c0(allocation) : 0;
    frame.state = 0xffffffffu;
    D(0x00ae3138) = value;
    allocation = (u32)cm_fn_00944cf1(0x52080);
    frame.state = 1;
    value = allocation ? cm_fn_005ef0b0(allocation) : 0;
    D(0x00b5ded8) = value;
    D(0x00b67a30) = 1;
    if (B(0x00ae2c98)) {
        if (!cm_fn_00945a60(0x00ae2c98, 0x009cd238)) D(0x009cd1fc) = 0;
    }
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return 1;
}
