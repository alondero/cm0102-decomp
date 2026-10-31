#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_00944cf1(u32);
extern u32 CM_THISCALL cm_fn_0092fe80(u32, u32, u32, u32, u32, u32,
                                    u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_008af520(u32, u32, u32, u32, u32, u32,
                                    u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Preferred-image VA008937b0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX. SEH9632fc; scratch204 with saved
   allocationL0 and diagnostic buffersL4/L104. Ten independently checked
   constructor tuples; identifiers increment only on nonnull allocation.
   The row5 rewards 300/1000 are the original values. All callees remain
   explicit original boundaries. */
u32 CM_THISCALL cm_fn_008937b0(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x204 / 4];
    u32 l = (u32)scratch, owner, allocation, eax, pointer, version;
    u32 stream, identifier = 0, row, title_address, title, high, low, number, nation;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x009632fc;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 10;
    eax = cm_fn_00944e46(40);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00a8b5ec, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x68);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    for (row = 0; row != 7; ++row) {
        allocation = cm_fn_00944cf1(0x2b);
        D(l) = allocation;
        if (row == 0) stream = input;
        frame.state = row;
        if (allocation != 0) {
            switch (row) {
            case 0: title_address = 0x009cfc38; high = 4000; low = 500; break;
            case 1: title_address = 0x009cfb88; high = 5000; low = 2500; break;
            case 2: title_address = 0x009cfb90; high = 4500; low = 2500; break;
            case 3: title_address = 0x009cfd10; high = 3000; low = 1000; break;
            case 4: title_address = 0x009cfd14; high = 3000; low = 1000; break;
            case 5: title_address = 0x009cfd20; high = 300; low = 1000; break;
            default: title_address = 0x009cfd18; high = 3000; low = 100; break;
            }
            title = D(title_address);
            number = identifier++;
            eax = cm_fn_0092fe80(allocation, owner, number, title, high, low,
                                0xffffffffu, 1, 0, 0, stream, row != 0);
        } else eax = 0;
        pointer = D(self + 4);
        frame.state = 0xffffffffu;
        D(pointer + row * 4u) = eax;
    }
    allocation = cm_fn_00944cf1(0x25d);
    D(l) = allocation;
    frame.state = 7;
    if (allocation != 0) {
        nation = D(0x009cf5ec);
        title = D(0x009cfcb4);
        number = identifier++;
        eax = cm_fn_008af520(allocation, owner, number, title, 1000, 0,
                            nation, 1, 0, stream, 1);
    } else eax = 0;
    pointer = D(self + 4);
    frame.state = 0xffffffffu;
    D(pointer + 0x1c) = eax;

    allocation = cm_fn_00944cf1(0x2b);
    D(l) = allocation;
    frame.state = 8;
    if (allocation != 0) {
        nation = D(0x009cf5ec);
        title = D(0x009cfb8c);
        number = identifier++;
        eax = cm_fn_0092fe80(allocation, owner, number, title, 5000, 2500,
                            nation, 1, 0, 1, stream, 1);
    } else eax = 0;
    pointer = D(self + 4);
    frame.state = 0xffffffffu;
    D(pointer + 0x20) = eax;

    allocation = cm_fn_00944cf1(0x25d);
    D(l) = allocation;
    frame.state = 9;
    if (allocation != 0) {
        nation = D(0x009cf5f0);
        title = D(0x009cfcb8);
        eax = cm_fn_008af520(allocation, owner, identifier, title, 500, 0,
                            nation, 1, 0, stream, 1);
    } else eax = 0;
    pointer = D(self + 4);
    D(pointer + 0x24) = eax;
done:
    u32 predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return eax;
}
