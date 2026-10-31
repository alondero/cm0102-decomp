/* Preferred-image VA 0x00430050, image base 0x00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, ret 0. The sole direct caller forwards EAX unchanged.
   L = entry ESP-0x41c, the base of the original 0x410-byte scratch.
   L+0 holds four initialized bytes, L+4/L+8 are virtual-call outputs, L+0xc
   records the second allocation. Error buffers are L+0x10/L+0x210 and
   L+0x110/L+0x310, respectively. The original DWORD pushes from L+4/L+8
   contain unknown upper halves, but 0x00522e00 observes only their low WORDs
   at VAs 0x00522e5e/0x00522e6c. Its seventh stack argument is likewise read
   only as a WORD at 0x00522e31; the caller's scratch-pointer high half is dead. */
#include "cmre.h"

extern unsigned long long cm_fn_00944e46(u32 size);
extern unsigned long long cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_00522e00(u32 allocation, u32 a1, u32 a2,
    u32 a3, u16 a4, u32 a5, u32 a6, u16 a7, u32 a8, u32 a9,
    u16 a10, u32 a11, u32 a12, u32 a13, u32 a14, u32 a15);
extern u32 CM_THISCALL cm_fn_0051c800(u32 self, u32 index);
extern unsigned long long cm_fn_009452ca(u32 pointer);
/* This original boundary leaves the incidental EAX subsequently returned. */
extern u32 cm_fn_005e8290(const char *title, const char *text, u32 flags);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);

u32 CM_FASTCALL cm_fn_00430050(u32 self)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch_words[0x104];
    u8 *scratch = (u8 *)scratch_words;
    u32 members;
    u32 source;
    u32 value;
    u32 vtable;
    u32 target;
    u32 virtual_result;
    u32 allocation;
    u16 word40;
    u16 out4;
    u16 out8;
    u32 field4;
    u32 slot;
    u32 object;
    u32 result;
    u32 predecessor;
    u32 line;
    void *first_buffer;
    void *second_buffer;
    char *text;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095462eu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u8 *)(scratch + 0) = 0;
    *(volatile u8 *)(scratch + 1) = 3;
    *(volatile u8 *)(scratch + 2) = 1;
    *(volatile u8 *)(scratch + 3) = 2;
    members = (u32)cm_fn_00944e46(0x10);
    if (members == 0) {
        first_buffer = scratch + 0x10;
        second_buffer = scratch + 0x210;
        line = 0x21bu;
        goto error;
    }
    source = *(volatile u32 *)(self + 0xb1u);
    value = *(volatile u32 *)source;
    *(volatile u32 *)members = value;
    source = *(volatile u32 *)(self + 0xb1u);
    value = *(volatile u32 *)(source + 0xb1u);
    *(volatile u32 *)(members + 4u) = value;
    source = *(volatile u32 *)(self + 0xb1u);
    value = *(volatile u32 *)(source + 0x3bu);
    *(volatile u32 *)(members + 8u) = value;
    source = *(volatile u32 *)(self + 0xb1u);
    value = *(volatile u32 *)(source + 0x76u);
    *(volatile u32 *)(members + 12u) = value;
    vtable = *(volatile u32 *)self;
    target = *(volatile u32 *)(vtable + 0x3cu);
    virtual_result = ((u32 (CM_THISCALL *)(u32, u32, void *, void *, u32))target)
        (self, 0, scratch + 8, scratch + 4, 0);
    allocation = (u32)cm_fn_00944cf1(0xb2u);
    *(volatile u32 *)(scratch + 12) = allocation;
    frame.state = 0;
    if (allocation != 0) {
        out4 = *(volatile u16 *)(scratch + 4);
        word40 = *(volatile u16 *)(self + 0x40u);
        field4 = *(volatile u32 *)(self + 4u);
        out8 = *(volatile u16 *)(scratch + 8);
        object = cm_fn_00522e00(allocation, self, 4, members, out8,
            field4, virtual_result, word40, 0, 1, out4, 0x14, 0, 1,
            0, (u32)scratch);
    } else {
        object = 0;
    }
    slot = *(volatile u32 *)(self + 12u);
    frame.state = 0xffffffffu;
    *(volatile u32 *)slot = object;
    slot = *(volatile u32 *)(self + 12u);
    object = *(volatile u32 *)slot;
    if (object != 0) {
        cm_fn_0051c800(object, 0);
        cm_fn_009452ca(virtual_result);
        result = (u32)cm_fn_009452ca(members);
        *(volatile u32 *)(self + 0x30u) = 0;
        goto done;
    }
    first_buffer = scratch + 0x310;
    second_buffer = scratch + 0x110;
    line = 0x22fu;
error:
    cm_fn_00944cff(0x0098acacu, 0, 0, first_buffer, second_buffer);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
        first_buffer, second_buffer, line);
    result = cm_fn_005e8290((const char *)0x009870e0u,
        (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
done:
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return result;
}
