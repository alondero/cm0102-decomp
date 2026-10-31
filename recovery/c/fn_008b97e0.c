/* Preferred-image VA 008b97e0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_008b4e10(u32 self);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_CDECL cm_fn_008b97e0(volatile u32 argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u32 previous, required, allocation, block = 0, source, destination, i, value, result, version;
    u16 word;
    u8 byte;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0096390eu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    required = argument;
    if (required == 0) {
        cm_fn_00944cff(0x00a8d1d8u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x6dcu);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    allocation = (u32)cm_fn_00944cf1(155);
    D((u32)work) = allocation;
    frame.state = 0;
    if (allocation != 0)
        block = cm_fn_008b4e10(allocation);
    value = (u32)(s32)(s8)B(required + 0x7d9u);
    source = required + value * 155u + 0xfau;
    destination = block;
    for (i = 0; i < 38u; ++i) {
        value = D(source);
        D(destination) = value;
        source += 4u;
        destination += 4u;
    }
    word = W(source);
    W(destination) = word;
    frame.state = 0xffffffffu;
    byte = B(source + 2u);
    B(destination + 2u) = byte;
    result = cm_fn_007fe600(0x00b74450u, 0x008b9940u, 0x008ba530u, 1, 0, 0);
    if (result != 0) {
        cm_fn_007ff270(0x00b74450u, 0, required, 0);
        cm_fn_007ff270(0x00b74450u, 1, block, 0);
        result = cm_fn_007ff270(0x00b74450u, 2, block + 9u, 1);
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
