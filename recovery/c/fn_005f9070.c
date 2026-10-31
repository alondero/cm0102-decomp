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

/* Preferred-image VA005f9070/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX. Full independent root review, including
   all ten constructor tuples. SEH95af1c/scratch204: allocationL0,
   diagnosticsL4/L104. Rows3/7 allocate25d; all others2b. Identifiers
   advance only for nonnull allocations, except the final row. Original
   constructor boundaries remain explicit; SEH dispatch is not established. */
u32 CM_THISCALL cm_fn_005f9070(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x204 / 4];
    u32 l = (u32)scratch, owner, eax, allocation, pointer, version;
    u32 identifier = 0, number, stream, row, nation, title, high, low;
    u32 first_flag, third_flag, large;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095af1c;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 10;
    eax = cm_fn_00944e46(40);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x009cf014, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x67);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    allocation = cm_fn_00944cf1(0x2b);
    D(l) = allocation;
    stream = input;
    for (row = 0; row != 10; ++row) {
        large = row == 3 || row == 7;
        if (row != 0) {
            allocation = cm_fn_00944cf1(large ? 0x25d : 0x2b);
            D(l) = allocation;
        }
        frame.state = row;
        if (allocation != 0) {
            if (row == 0) {
                title = D(0x009cfc00);
                identifier = 1;
                eax = cm_fn_0092fe80(allocation, owner, 0, title, 6000, 1500,
                                    0xffffffffu, 1, 0, 0, stream, 0);
            } else {
                switch (row) {
                case 1: nation = 0xffffffffu; title = D(0x009cfb10); high = 5000; low = 1500; break;
                case 2: nation = 0xffffffffu; title = D(0x009cfb14); high = 6000; low = 2500; break;
                case 3: nation = D(0x009cf568); title = D(0x009cfcd4); high = 0; low = 0; break;
                case 4: nation = D(0x009cf568); title = D(0x009cfb18); high = 6000; low = 2500; break;
                case 5: nation = D(0x009cf568); title = D(0x009cfb1c); high = 6000; low = 2500; break;
                case 6: nation = D(0x009cf568); title = D(0x009cfb20); high = 6000; low = 2500; break;
                case 7: nation = D(0x009cf56c); title = D(0x009cfcd8); high = 0; low = 0; break;
                case 8: nation = D(0x009cf56c); title = D(0x009cfb24); high = 4000; low = 1000; break;
                default: nation = D(0x009cf56c); title = D(0x009cfb2c); high = 4000; low = 1000; break;
                }
                number = identifier;
                if (row != 9) ++identifier;
                if (large)
                    eax = cm_fn_008af520(allocation, owner, number, title, high, low,
                                        nation, 1, 0, stream, 1);
                else {
                    first_flag = row == 4 || row == 5 || row == 8;
                    third_flag = row == 5;
                    eax = cm_fn_0092fe80(allocation, owner, number, title, high, low,
                                        nation, first_flag, 0, third_flag, stream, 1);
                }
            }
        } else eax = 0;
        pointer = D(self + 4);
        if (row != 9) frame.state = 0xffffffffu;
        D(pointer + row * 4u) = eax;
    }
done:
    u32 predecessor = frame.prev;
    CM_FS(u32, 0) = predecessor;
    return eax;
}
