/* Preferred-image VA 0x007e7760, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, ret 4, returns the receiver in EAX.
   Scratch L is entry ESP-0x210: allocation at L, buffers L+4 and L+0x104. */
#include "cmre.h"

extern unsigned long long cm_fn_00944cf1(u32 size);
/* The original 0x0068d020 leaves its incoming ECX in EAX. */
extern u32 CM_FASTCALL cm_fn_0068d020(u32 self);
/* 0x007e77d5 tests all of EAX; 0x007e78a0 normalizes it to 0 or 1. */
extern u32 CM_THISCALL cm_fn_007e78a0(u32 self, u32 input);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u32 CM_THISCALL cm_fn_007e7760(u32 self, u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[129];
    u32 allocation;
    u32 value;
    u32 predecessor;

    frame.state = 0xffffffffu;
    frame.handler = 0x0096085eu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u32 *)self = 0x0096f104u;
    allocation = (u32)cm_fn_00944cf1(14);
    *(volatile u32 *)scratch = allocation;
    frame.state = 0;
    if (allocation != 0)
        value = cm_fn_0068d020(allocation);
    else
        value = 0;
    *(volatile u32 *)(self + 7u) = value;
    frame.state = 0xffffffffu;
    *(volatile u32 *)(self + 0xbu) = 0;
    *(volatile u32 *)(self + 0xfu) = 0;
    *(volatile u32 *)(self + 0x13u) = 0;
    if (input != 0 && cm_fn_007e78a0(self, input) == 0) {
        char *text;
        cm_fn_00944cff(0x00a7a858u, 0, 0,
                       (u8 *)scratch + 0x104, (u8 *)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       (u8 *)scratch + 0x104, (u8 *)scratch + 4, 0x36u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    }
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return self;
}
