/* Preferred-image VA 009020a0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 *CM_THISCALL cm_fn_008e84a0(u8 *self, u32 a, s32 b, s32 c, u32 d, u32 e);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_009020a0(u32 self, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u32 predecessor, allocation, version;
    frame.state = 0xffffffffu;
    frame.handler = 0x009651abu;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    D((u32)work) = self;
    cm_fn_008e84a0((u8 *)self, a, (s32)b, (s32)c, d, e);
    frame.state = 0;
    D(self) = 0x0097075cu;
    B(self + 8u) = 1;
    allocation = (u32)cm_fn_00944e46(12);
    D(self + 4u) = allocation;
    if (allocation == 0) {
        cm_fn_00944cff(0x00aa19a8u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x52u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    } else {
        B(allocation) = 0x18;
        B(allocation + 1u) = 0;
        B(allocation + 2u) = 0xff;
        B(allocation + 3u) = 1;
        B(allocation + 4u) = 5;
        B(allocation + 5u) = 1;
        allocation = D(self + 4u) + 6u;
        B(allocation) = 0x18;
        B(allocation + 1u) = 0;
        B(allocation + 2u) = 0xff;
        B(allocation + 3u) = 0x1f;
        B(allocation + 4u) = 0;
        B(allocation + 5u) = 0;
    }
    predecessor = frame.prev;
    B(self + 0x13u) = 8;
    B(self + 0x14u) = 8;
    CM_FS(volatile u32, 0) = predecessor;
    return self;
}
