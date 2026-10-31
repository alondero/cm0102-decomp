/* Preferred-image VA 008f0e40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Native007ff270/0068d040 overwrite full EAX before any return path;
   retain their incidental results at explicit original boundaries. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 first_callback, u32 second_callback, u32 one, u32 zero, u32 last);
extern u32 CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 zero);
extern void CM_THISCALL cm_fn_0068d020(void *list);
extern u32 CM_THISCALL cm_fn_0068d040(void *list);
extern u32 CM_THISCALL cm_fn_0068d1b0(void *list);
extern u32 CM_THISCALL cm_fn_008c5cf0(u32 self, u32 record, void *list, u32 zero);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 key);

u32 CM_CDECL cm_fn_008f0e40(volatile u32 first, volatile u32 second, volatile s16 third, volatile s8 fourth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x10];
    u32 previous, result, saved_first, value, receiver, cursor, row, pointer;
    s16 kind;
    u8 tag;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00964c78u;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    result = cm_fn_007fe600(0x00b74450u, 0x008f0f90u, 0x008f16c0u, 1, 0, 0);
    if (result == 0)
        goto done;
    saved_first = first;
    cm_fn_007ff270(0x00b74450u, 0, saved_first, 0);
    value = second;
    cm_fn_007ff270(0x00b74450u, 1, value, 0);
    kind = third;
    cm_fn_007ff270(0x00b74450u, 2, (u32)(s32)kind, 0);
    value = (u32)(s32)fourth;
    result = cm_fn_007ff270(0x00b74450u, 4, value, 0);
    if (kind != 18 && kind != 32)
        goto done;
    cm_fn_0068d020(work);
    receiver = D(0x00de1f54u);
    frame.state = 0;
    cm_fn_008c5cf0(receiver, saved_first, work, 0);
    cursor = D((u32)work + 2u);
    D((u32)work + 10u) = cursor;
    row = cm_fn_0068d1b0(work);
    while (row != 0) {
        value = D(row);
        receiver = D(0x00de1f54u);
        pointer = cm_fn_008c61e0(receiver, value);
        pointer = D(pointer);
        tag = B(pointer + 0x2cu);
        if (tag == 11 || tag == 12 || tag == 13 || tag == 14 || tag == 19) {
            value = D(pointer + 8u);
            cm_fn_007ff270(0x00b74450u, 3, value, 0);
        }
        row = cm_fn_0068d1b0(work);
    }
    frame.state = 0xffffffffu;
    result = cm_fn_0068d040(work);
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
