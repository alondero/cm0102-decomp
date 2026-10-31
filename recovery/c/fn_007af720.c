/* Preferred-image VA 007af720, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *date);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
extern void CM_THISCALL cm_fn_00417c10(void *self, void *output, u32 other);
extern u32 CM_THISCALL cm_fn_00795750(u32 self, u32 person, u32 record);
extern u32 CM_THISCALL cm_fn_007af870(u32 self, void *key);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007af720(u32 self, u32 record)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x238];
    u32 version, index, receiver, base, pointer, result;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095f3a6u;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work + 0x10);
    frame.state = 0;
    cm_fn_0054a110(work + 0x18);
    frame.state = 1;
    if (record == 0) {
        cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x138, work + 0x38);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x138, work + 0x38, 0x369u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        result = 0;
    } else {
        cm_fn_007e4920(record, (u32)work);
        cm_fn_00417c10(work + 0x18, work + 0x30, 0x00ae2c90u);
        index = D(self);
        B((u32)work + 0x20u) = 0;
        D((u32)work + 0x21u) = 0;
        D((u32)work + 0x25u) = 0;
        B((u32)work + 0x29u) = 0;
        B((u32)work + 0x2au) = 0;
        receiver = D(0x00b7433cu);
        base = D(0x00ae23bcu);
        pointer = cm_fn_00795750(receiver, base + index * 110u, record);
        B((u32)work + 0x2cu) = pointer != 0 ? B(pointer + 0x113u) : 0xff;
        result = cm_fn_007af870(self, work);
    }
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
