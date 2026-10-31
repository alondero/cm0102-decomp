/* Preferred-image VA 0048ece0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original007862d0 returns full EAX=0 at0078637f or1 at007863e0. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 zero);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern u32 CM_THISCALL cm_fn_007862d0(u32 self, void *record, u32 club);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_0048ece0(u32 self, volatile u32 first, volatile u32 second,
                              volatile u32 third, volatile s8 fourth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x2e0];
    u32 previous, required, pointer, optional, value, receiver, result, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009560ebu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    required = second;
    if (required == 0) {
        cm_fn_00944cff(0x0099b80cu, 0, 0, work + 0x1e0, work + 0xe0);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x1e0, work + 0xe0, 0x897u);
        result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    cm_fn_0077bbe0(work, 35, 0);
    pointer = D(self + 4u);
    frame.state = 0;
    value = D(pointer);
    cm_fn_00785790(work, 0, value);
    value = D(required);
    cm_fn_00785790(work, 1, value);
    pointer = third;
    value = pointer != 0 ? D(pointer) : 0xffffffffu;
    cm_fn_00785790(work, 2, value);
    optional = first;
    value = optional != 0 ? D(optional) : 0xffffffffu;
    cm_fn_00785790(work, 3, value);
    value = (u32)(s32)fourth;
    cm_fn_00785790(work, 4, value);
    pointer = D(self + 4u);
    receiver = D(0x00b7431cu);
    result = cm_fn_007862d0(receiver, work, pointer);
    if (optional != 0) {
        receiver = D(0x00b7431cu);
        result = cm_fn_007862d0(receiver, work, optional);
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
