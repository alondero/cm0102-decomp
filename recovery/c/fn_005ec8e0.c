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

/* Preferred-image VA005ec8e0/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX. All ten tuples independently checked:
   rows2/6 allocate25d, other rows2b; titles9cfea0..9cfec4. Rows4/8
   have final flag0, rows5/9 have third flag1. SEH95ab5c/scratch204:
   allocationL0, diagnosticsL4/L104. Only nonnull allocations advance the
   identifier, except the last row. Callees stay original boundaries. */
u32 CM_THISCALL cm_fn_005ec8e0(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x204 / 4];
    u32 l = (u32)scratch, owner, eax, allocation, pointer, version;
    u32 identifier = 0, number, stream, row, nation, title, high, low, large;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095ab5c;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 10;
    eax = cm_fn_00944e46(40);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x009cdb90, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x68);
        eax = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        goto done;
    }
    allocation = cm_fn_00944cf1(0x2b);
    D(l) = allocation;
    stream = input;
    for (row = 0; row != 10; ++row) {
        large = row == 2 || row == 6;
        if (row != 0) {
            allocation = cm_fn_00944cf1(large ? 0x25d : 0x2b);
            D(l) = allocation;
        }
        frame.state = row;
        if (allocation != 0) {
            if (row == 0) {
                identifier = 1;
                title = D(0x009cfea0);
                eax = cm_fn_0092fe80(allocation, owner, 0, title, 4000, 2000,
                                    0xffffffffu, 1, 0, 0, stream, 1);
            } else {
                if (row == 1) nation = 0xffffffffu;
                else nation = D(row < 6 ? 0x009cf86c : 0x009cf870);
                title = D(0x009cfea0 + row * 4u);
                number = identifier;
                if (row != 9) ++identifier;
                if (large)
                    eax = cm_fn_008af520(allocation, owner, number, title, 0, 0,
                                        nation, 1, 0, stream, 1);
                else {
                    high = row == 1 ? 4000u : row < 6 ? 2000u : 1000u;
                    low = row == 1 ? 2000u : row < 6 ? 1000u : 500u;
                    eax = cm_fn_0092fe80(allocation, owner, number, title, high, low,
                                        nation, 1, 0, row == 5 || row == 9, stream,
                                        row != 4 && row != 8);
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
