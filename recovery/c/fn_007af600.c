/* Preferred-image VA 007af600, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_FASTCALL cm_fn_0054a110(void *self);
extern u32 CM_CDECL cm_fn_007e4920(u32 source, u32 destination);
extern u32 CM_CDECL cm_fn_00946464(void *key, u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007af600(u32 self, u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x230];
    u32 predecessor, count, table, pointer, text, result = 0;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095f386u;
    predecessor = CM_FS(volatile u32, 0);
    frame.prev = predecessor;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work + 0x10);
    frame.state = 0;
    cm_fn_0054a110(work + 0x18);
    frame.state = 1;
    if (input == 0) {
        cm_fn_00944cff(0x00a701b0u, 0, 0, work + 0x130, work + 0x30);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, text, work + 0x130, work + 0x30, 0x33eu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    count = D(self + 0x1f6u);
    if (count == 0)
        goto done;
    cm_fn_007e4920(input, (u32)work);
    count = D(self + 0x1f6u);
    table = D(self + 0x1eeu);
    pointer = cm_fn_00946464(work, table, count, 0x2du, 0x007bce50u);
    if (pointer != 0 && (B(pointer + 0x20u) & 1u) == 0)
        result = 1;
done:
    predecessor = frame.prev;
    CM_FS(volatile u32, 0) = predecessor;
    return result;
}
