/* Preferred-image VA 0085e750, base 00400000; CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   0043f460 reads ECX and RET4; its EDX:EAX result is pushed as two words. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32, u32);
extern u32 CM_CDECL cm_fn_00441c10(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);

u32 CM_THISCALL cm_fn_0085e750(u32 self, u32 output, u32 mode, u32 extra_flags)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xd0];
    u32 previous = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00962a0b;
    frame.prev = previous;
    CM_FS(u32, 0) = (u32)&frame;
    u32 amount = D(self + 0xc);
    D((u32)scratch) = amount;
    u32 settings = D(0x00ae3138);
    frame.state = 0;
    u32 flags = D(settings + 4);
    u8 marker;
    if (flags & 0x200000) {
        D((u32)scratch) = amount;
        marker = 8;
    } else if (flags & 0x400000) {
        D((u32)scratch) = amount * 4u;
        marker = 8;
    } else {
        D((u32)scratch) = amount * 52u;
        marker = 4;
    }
    B((u32)scratch + 4) = marker | (u8)extra_flags;
    u32 pushed_flags = D((u32)scratch + 4);
    unsigned long long value = cm_fn_0043f460((u32)scratch, pushed_flags);
    cm_fn_00441c10((u32)scratch + 8, 200, (u32)value,
                   (u32)(value >> 32), pushed_flags);
    u32 result;
    if ((u8)mode == 0) {
        result = cm_fn_00944c9f(output, 0x00991944, scratch + 8);
    } else {
        settings = D(0x00ae3138);
        flags = D(settings + 4);
        u32 format = 0;
        if ((u8)mode == 2) {
            if (flags & 0x200000) format = 0x00a836c4;
            else if (flags & 0x400000) format = 0x00a836ac;
            else if (flags & 0x800000) format = 0x00a83694;
        } else {
            if (flags & 0x200000) format = 0x00a83678;
            else if (flags & 0x400000) format = 0x00a8365c;
            else if (flags & 0x800000) format = 0x00a83640;
        }
        result = flags;
        if (format) result = cm_fn_0066f4e0(output, format, scratch + 8);
    }
    CM_FS(u32, 0) = frame.prev;
    return result;
}
