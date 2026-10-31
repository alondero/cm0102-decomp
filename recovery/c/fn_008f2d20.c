#include "cmre.h"
/* Preferred-image VA 0x008f2d20, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   The original null-record branch still reads byte [record+0x2c]. */
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00786b70(u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_00785830(u32, u32);
extern u32 CM_THISCALL cm_fn_008c61e0(u32, u32);
extern u32 CM_THISCALL cm_fn_007fe600(u32, u32, u32, u32, u32, u32);
extern unsigned long long CM_THISCALL cm_fn_007ff270(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_008f2e10(u32, u32, u32);
#define D(a) (*(volatile u32 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

u32 CM_CDECL cm_fn_008f2d20(u32 first, u32 second)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0xe0];
    u32 work = (u32)scratch, receiver, value, other, pointer, record, eax, prev;
    frame.state = 0xffffffffu;
    frame.handler = 0x00964d9b;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;
    cm_fn_0077bbe0(work, 0, 0);
    receiver = D(0x00b7431c);
    frame.state = 0;
    cm_fn_00786b70(receiver, first, second, work);
    value = (u32)cm_fn_00785830(work, 6);
    other = (u32)cm_fn_00785830(work, 7);
    receiver = D(0x00de1f54);
    pointer = cm_fn_008c61e0(receiver, value);
    record = D(pointer);
    if (record && D(record + 0x24) != other) goto create;
    if (B(record + 0x2c) == 3) goto create;
    eax = cm_fn_008f2e10(pointer, first, second);
    goto done;
create:
    eax = cm_fn_007fe600(0x00b74450, 0x008f24c0, 0x008f2800, 1, 0, 0);
    if (eax) {
        cm_fn_007ff270(0x00b74450, 0, record, 0);
        eax = (u32)cm_fn_007ff270(0x00b74450, 1, other, 0);
    }
done:
    prev = frame.prev;
    CM_FS(u32, 0) = prev;
    return eax;
}
