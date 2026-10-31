/* Preferred-image VA 008c8740, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_FASTCALL cm_fn_008e5680(u32 self);
extern void CM_THISCALL cm_fn_008e78f0(u32 self, u32 second, u32 first, u32 fifth);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 first);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_STDCALL cm_fn_008c8740(u32 first, u32 second, u32 output, u32 status, u32 fifth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x204];
    u32 predecessor, allocation, object, receiver, result, version;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963dceu;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    allocation = (u32)cm_fn_00944cf1(65);
    D((u32)work) = allocation;
    frame.state = 0;
    object = allocation != 0 ? cm_fn_008e5680(allocation) : 0;
    D(output) = object;
    frame.state = 0xffffffffu;
    cm_fn_008e78f0(object, second, first, fifth);
    receiver = D(0x00ae2a38u);
    result = cm_fn_005a0590(receiver, first);
    if (result == 0) {
        cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x1f34u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
    }
    CM_GLOBAL(volatile u8, status) = 0x32;
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
    return 1;
}
