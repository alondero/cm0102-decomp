/* Preferred-image VA 0077b700, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole direct caller VA004e0350 overwrites EAX at004e0358. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 first, u32 second);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 person, u32 type, void *destination);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 flags);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_CDECL cm_fn_0077b700(volatile u32 first, volatile u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x2e0];
    u32 previous, saved, type, receiver, value, version;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095e0abu;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(work, 0, 0);
    frame.state = 0;
    if (cm_fn_007fe600(0x00b74450u, 0x0077b850u, 0x0077bb30u, 0, 0, 0) == 0)
        goto done;
    saved = first;
    if (saved != 0) {
        type = second;
        receiver = D(0x00b7431cu);
        if (cm_fn_00786b70(receiver, saved, type, work) == 0) {
            cm_fn_00944cff(0x00a649e8u, 0, 0, work + 0x1e0, work + 0xe0);
            version = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x1e0, work + 0xe0, 0x4au);
            cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
            D(0x00b67a34u) = 0;
            goto done;
        }
    }
    cm_fn_007ff270(0x00b74450u, 0, 1, 0);
    value = saved != 0 ? (u32)cm_fn_00785830(work, 0) : 0;
    cm_fn_007ff270(0x00b74450u, 1, value, 0);
done:
    CM_FS(volatile u32, 0) = frame.prev;
}
