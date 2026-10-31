/* Preferred-image VA 0x005f8820, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, ret 8, returns the receiver. Scratch L is entry ESP-0x214:
   allocation L, saved receiver L+4, buffers L+8 and L+0x108. */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_00518640(u32 self);
extern u32 CM_THISCALL cm_fn_0051fc00(u32 self, char flag);
extern u32 CM_THISCALL cm_fn_005f8c10(u32 self);
extern u32 CM_FASTCALL cm_fn_00518790(u32 self);
extern unsigned long long cm_fn_00944cf1(u32 size);
extern u32 CM_THISCALL cm_fn_0049ee70(u32 self, u32 parent);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

u32 CM_THISCALL cm_fn_005f8820(u32 self, u16 arg1, u32 arg2)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[130];
    u32 vtable;
    u32 target;
    u32 result;
    u32 line;
    u32 predecessor;
    void *first_buffer;
    void *second_buffer;
    char *text;

    frame.state = 0xffffffffu;
    frame.handler = 0x0095ae39u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    *(volatile u32 *)((u8 *)scratch + 4) = self;
    cm_fn_00518640(self);
    *(volatile u32 *)(self + 4u) = arg2;
    frame.state = 0;
    *(volatile u32 *)self = 0x0096b9d4u;
    *(volatile u16 *)(self + 0x40u) = arg1;
    *(volatile u8 *)(self + 0xabu) = 0;
    *(volatile u8 *)(self + 0x44u) = 0xff;
    *(volatile u32 *)(self + 0x2cu) = 0;
    *(volatile u32 *)(self + 0x30u) = 0xffffffffu;
    *(volatile u8 *)(self + 0x42u) = 1;
    *(volatile u8 *)(self + 0x50u) = 0xf;
    *(volatile u8 *)(self + 0xb1u) = 0;
    if (cm_fn_0051fc00(self, 1) != 0)
        goto done;
    if ((s32)cm_fn_005f8c10(self) < 0) {
        first_buffer = (u8 *)scratch + 0x108;
        second_buffer = (u8 *)scratch + 8;
        line = 0x3fu;
        goto error;
    }
    vtable = *(volatile u32 *)self;
    target = *(volatile u32 *)(vtable + 0x3cu);
    result = ((u32 (CM_THISCALL *)(u32, u32, u32, u32, u32))target)
        (self, 0xffffffffu, self + 0x3cu, self + 0x3au, 0);
    *(volatile u32 *)(self + 0xa3u) = result;
    if ((s32)cm_fn_00518790(self) < 0) {
        first_buffer = (u8 *)scratch + 8;
        second_buffer = (u8 *)scratch + 0x108;
        line = 0x4du;
        goto error;
    }
    result = (u32)cm_fn_00944cf1(0x5ceu);
    *(volatile u32 *)scratch = result;
    *(volatile u8 *)&frame.state = 1;
    if (result != 0)
        result = cm_fn_0049ee70(result, self);
    *(volatile u8 *)&frame.state = 0;
    *(volatile u32 *)(self + 8u) = result;
    if (result != 0)
        goto done;
    first_buffer = (u8 *)scratch + 8;
    second_buffer = (u8 *)scratch + 0x108;
    line = 0x54u;
error:
    cm_fn_00944cff(0x009cefdcu, 0, 0, first_buffer, second_buffer);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                   first_buffer, second_buffer, line);
    cm_fn_005e8290((const char *)0x009870e0u,
                   (const char *)0x00ae24d0u, 0);
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
done:
    predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return self;
}
