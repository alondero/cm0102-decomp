#include "cmre.h"
/* Preferred-image VA 0x008bed60, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
extern u32 CM_FASTCALL cm_fn_008e5680(u32);
extern void CM_THISCALL cm_fn_008e7810(u32, u32, u32);
extern u32 CM_FASTCALL cm_fn_0085e410(u32);
extern void CM_THISCALL cm_fn_0085e590(u32, u32, u32, u32, u32, u32, u32);
extern void CM_THISCALL cm_fn_008be300(u32, u32, u32);
extern void CM_THISCALL cm_fn_004cc490(u32, u32, u32, u32, u32, u32, u32, u32);
extern void CM_FASTCALL cm_fn_008e5710(u32);
#define D(a) (*(volatile u32 *)(u32)(a))

u32 CM_THISCALL cm_fn_008bed60(u32 self, u32 first, u32 second,
                              u32 third, u32 fourth, u32 fifth, u32 sixth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x94];
    u32 work = (u32)scratch, a, b, receiver, result, prev;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963ac3;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_008e5680(work);
    frame.state = 0;
    cm_fn_008e7810(work, first, second);
    cm_fn_0085e410(work + 0x44);
    ((volatile u8 *)&frame.state)[0] = 1;
    cm_fn_0085e590(work + 0x44, first, second, third, fourth, fifth, sixth);
    cm_fn_008be300(self, work, work + 0x44);
    a = D(work + 0x24);
    b = D(work + 0x20);
    receiver = D(0x00ae1050);
    cm_fn_004cc490(receiver, work + 0x44, b, a, 11, 0, 1, 0);
    result = D(work + 0x20);
    frame.state = 0xffffffffu;
    cm_fn_008e5710(work);
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return result;
}
