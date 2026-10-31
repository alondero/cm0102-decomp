#include "cmre.h"
/* Preferred-image VA 00666720/base 00400000, CM0102 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4, full incidental EAX. Scratch L0 allocation, L4/L104
   diagnostic buffers; one linked SEH record, handler 0095c2f0. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_THISCALL cm_fn_008af520(u32,u32,u32,u32,u32,u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

u32 CM_THISCALL cm_fn_00666720(u32 self, volatile u32 input_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 l = (u32)scratch, previous, owner, eax, input, allocation;
    u32 count = 0, index, label, category, table, serial;
    u32 prize, second_prize, flag, version;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095c2f0;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 8;
    eax = cm_fn_00944e46(0x20);
    D(self + 4) = eax;
    if (!eax) {
        cm_fn_00944cff(0x009eaf84, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x66);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    allocation = cm_fn_00944cf1(0x2b);
    D(l) = allocation;
    input = input_arg;
    frame.state = 0;
    if (allocation) {
        label = D(0x009cfc58);
        count = 1;
        eax = cm_fn_0092fe80(allocation, owner, 0, label, 0x1388, 0,
                            0xffffffffu, 0, 0, 0, input, 0);
    } else eax = 0;
    table = D(self + 4);
    frame.state = 0xffffffffu;
    D(table) = eax;
    for (index = 1; index < 8; ++index) {
        allocation = cm_fn_00944cf1(index < 4 ? 0x2b : 0x25d);
        D(l) = allocation;
        frame.state = index;
        if (allocation) {
            if (index < 4) {
                if (index == 1) {
                    label = D(0x009cfc4c); prize = 0x157c; second_prize = 0x5dc;
                } else if (index == 2) {
                    label = D(0x009cfc50); prize = 0x1388; second_prize = 0x1f4;
                } else {
                    label = D(0x009cfc54); prize = 0xbb8; second_prize = 0x64;
                }
                serial = count++;
                eax = cm_fn_0092fe80(allocation, owner, serial, label, prize,
                                    second_prize, 0xffffffffu, 0, 0, 0, input, 1);
            } else {
                category = D(index < 6 ? 0x009cf65c : 0x009cf768);
                if (index == 4) {
                    label = D(0x009cfcac); prize = 0x7d0; second_prize = 0; flag = 1;
                } else if (index == 5) {
                    label = D(0x009cfc5c); prize = 0xbb8; second_prize = 0x64; flag = 0;
                } else if (index == 6) {
                    label = D(0x009cfcb0); prize = 0x3e8; second_prize = 0; flag = 1;
                } else {
                    label = D(0x009cfd38); prize = 0x5dc; second_prize = 0x32; flag = 0;
                }
                serial = count;
                if (index != 7) count++;
                eax = cm_fn_008af520(allocation, owner, serial, label, prize,
                                    second_prize, category, flag, 0, input, 1);
            }
        } else eax = 0;
        table = D(self + 4);
        if (index != 7) frame.state = 0xffffffffu;
        D(table + index * 4u) = eax;
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
