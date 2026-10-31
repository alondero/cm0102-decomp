#include "cmre.h"

/* Preferred-image VA007e78a0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/fullEAX. SEH960886, scratch474: countWORD L2, full112B recordL4,
   datesL8/L18, diagnosticsL74/L174/L274/L374. Constructor store7e7781 installs
   VT96f104; slot96f11c points to5bb550 (fullEAX1/RET4). Keep indirect dispatch.
   Forward physical EDX=145*secondClubID to68d0d0, not the computed pointer. */
#define D(a) (*(volatile u32 *)(a))
#define W(a) (*(volatile u16 *)(a))
extern u32 CM_CDECL cm_fn_00932c30(u32, u32, u32, u32);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_THISCALL cm_fn_0054a110(u32);
extern unsigned long long CM_FASTCALL cm_fn_0068d0d0(u32, u32, u32, s16);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_007e78a0(u32 self, u32 stream_box)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x474];
    u32 l = (u32)scratch, stream, value, count, pointer, id, base, edx;
    u32 index, target, name, extension, line, result;
    u16 count_word;
    frame.state = 0xffffffffu; frame.handler = 0x00960886;
    frame.prev = CM_FS(volatile u32, 0);
    CM_FS(volatile u32, 0) = (u32)&frame;
    stream = D(stream_box);
    value = cm_fn_00932c30(self + 0x13, 4, 1, stream);
    if (value != 1) {
        name = l + 0x174; extension = l + 0x74; line = 0x5f;
        goto diagnostic;
    }
    count = D(self + 0x13);
    if (count != 0) {
        D(self + 0xf) = count;
        pointer = (u32)cm_fn_00944e46(count * 14u);
        D(self + 0xb) = pointer;
        if (pointer == 0) {
            name = l + 0x74; extension = l + 0x174; line = 0x6c;
            goto diagnostic;
        }
        stream = D(stream_box);
        count = D(self + 0x13);
        value = cm_fn_00932c30(pointer, 14, count, stream);
        count = D(self + 0x13);
        if (value != count) {
            name = l + 0x74; extension = l + 0x174; line = 0x74;
            goto diagnostic;
        }
    }
    cm_fn_0054a110(l + 8);
    frame.state = 0;
    cm_fn_0054a110(l + 0x18);
    stream = D(stream_box);
    frame.state = 1;
    value = cm_fn_00932c30(l + 2, 2, 1, stream);
    if (value != 1) {
        name = l + 0x74; extension = l + 0x174; line = 0x7e;
        goto diagnostic;
    }
    count_word = W(l + 2);
    if ((s16)count_word > 0) {
        index = 0;
        do {
            stream = D(stream_box);
            value = cm_fn_00932c30(l + 4, 0x70, 1, stream);
            if (value != 1) {
                name = l + 0x374; extension = l + 0x274; line = 0x8b;
                goto diagnostic;
            }
            id = D(l + 0x24);
            base = D(0x00ae23d0);
            pointer = base + id * 0x6bu;
            id = D(l + 0x28);
            D(l + 0x38) = pointer;
            edx = id * 39u;
            base = D(0x00ae23b0);
            pointer = base + edx * 2u;
            id = D(l + 0x30);
            D(l + 0x3c) = pointer;
            base = D(0x00ae23b4);
            pointer = base + id * 0x245u;
            id = D(l + 0x34);
            D(l + 0x40) = pointer;
            edx = id * 145u;
            pointer = base + id + edx * 4u;
            target = D(self + 7);
            D(l + 0x44) = pointer;
            cm_fn_0068d0d0(target, edx, l + 4, 0x70);
            ++index;
            count_word = W(l + 2);
        } while ((s16)(u16)index < (s16)count_word);
    }
    target = D(self);
    target = D(target + 0x18);
    value = ((u32 (CM_THISCALL *)(u32, u32))target)(self, stream_box);
    result = value != 0;
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a7a858, 0, 0, name, extension);
    value = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, value, name, extension, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    result = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
