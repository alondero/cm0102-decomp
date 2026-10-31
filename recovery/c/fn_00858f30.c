#include "cmre.h"
/* Preferred-image VA00858f30/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX; SEH962940. Scratch204:
   allocationL0, diagnostic buffersL4/L104. Count is a DWORD, not a BYTE. */
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

u32 CM_THISCALL cm_fn_00858f30(u32 self, volatile u32 input_arg)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 scratch[0x204];
    u32 l = (u32)scratch, previous, owner, input, eax, allocation;
    u32 count = 0, label, category, serial, table, row, first, second, flag;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00962940;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 8;
    eax = cm_fn_00944e46(0x20);
    D(self + 4) = eax;
    if (!eax) {
        cm_fn_00944cff(0x00a82a90, 0, 0, l + 0x104, l + 4);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax, l + 0x104, l + 4, 0x66);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    allocation = cm_fn_00944cf1(0x2b);
    D(l) = allocation;
    input = input_arg;
    frame.state = 0;
    if (allocation) {
        label = D(0x009cfc34);
        count = 1;
        eax = cm_fn_0092fe80(allocation, owner, 0, label, 6000, 2500,
                            0xffffffffu, 1, 0, 0, input, 0);
    } else eax = 0;
    table = D(self + 4);
    frame.state = 0xffffffffu;
    D(table) = eax;
    for (row = 1; row < 8; ++row) {
        allocation = cm_fn_00944cf1(row == 2 || row == 5 ? 0x25d : 0x2b);
        D(l) = allocation;
        frame.state = row;
        if (allocation) {
            if (row == 1) {
                label = D(0x009cfb80);
                serial = count++;
                eax = cm_fn_0092fe80(allocation, owner, serial, label, 8000,
                                    4500, 0xffffffffu, 1, 0, 0, input, 1);
            } else {
                category = D(row < 5 ? 0x009cf624 : 0x009cf628);
                switch (row) {
                case 2: label = D(0x009cfcbc); first = 0; second = 0; flag = 0; break;
                case 3: label = D(0x009cfb84); first = 8000; second = 5500; flag = 1; break;
                case 4: label = D(0x009cfb7c); first = 6000; second = 3000; flag = 0; break;
                case 5: label = D(0x009cfcc0); first = 0; second = 0; flag = 0; break;
                case 6: label = D(0x009cfd40); first = 4000; second = 2700; flag = 1; break;
                default: label = D(0x009cfd3c); first = 3000; second = 1500; flag = 0; break;
                }
                serial = count;
                if (row != 7) count++;
                if (row == 2 || row == 5)
                    eax = cm_fn_008af520(allocation, owner, serial, label, first,
                                        second, category, 1, 0, input, 1);
                else
                    eax = cm_fn_0092fe80(allocation, owner, serial, label, first,
                                        second, category, 1, 0, flag, input, 1);
            }
        } else eax = 0;
        table = D(self + 4);
        if (row != 7) frame.state = 0xffffffffu;
        D(table + row * 4u) = eax;
    }
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return eax;
}
