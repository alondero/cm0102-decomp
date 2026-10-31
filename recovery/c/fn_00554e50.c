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

/* Preferred-image VA00554e50/base00400000, CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS1/RET4/full incidental EAX. SEH957c5c/scratch204: allocationL0,
   diagnosticsL4/L104. Independently checked ten original argument tuples.
   Three 25d-byte objects occur at rows1/4/7; the remaining seven use2b.
   Identifiers advance only after nonnull allocation; final row does not
   advance. All helpers stay explicit original boundaries. */
u32 CM_THISCALL cm_fn_00554e50(u32 self, volatile u32 input)
{
    volatile struct { u32 prev, handler, state; } frame;
    u32 scratch[0x204 / 4];
    u32 l = (u32)scratch, owner, eax, allocation, pointer, version;
    u32 identifier = 0, number, stream, row, nation, title;
    u32 nation_address, title_address, high, low;
    frame.prev = CM_FS(u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x00957c5c;
    CM_FS(u32, 0) = (u32)&frame;
    owner = D(self + 0x13);
    W(self + 8) = 10;
    eax = cm_fn_00944e46(40);
    D(self + 4) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x009b8ee0, 0, 0, l + 0x104, l + 4);
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
        u32 large = row == 1 || row == 4 || row == 7;
        if (row != 0) {
            allocation = cm_fn_00944cf1(large ? 0x25d : 0x2b);
            D(l) = allocation;
        }
        frame.state = row;
        if (allocation != 0) {
            if (row == 0) {
                identifier = 1;
                title = D(0x009cfab4);
                eax = cm_fn_0092fe80(allocation, owner, 0, title, 6000, 2500,
                                    0xffffffffu, 0, 0, 0, stream, 1);
            } else {
                switch (row) {
                case 1: nation_address = 0x009cf5ac; title_address = 0x009cfcec; high = 0; low = 0; break;
                case 2: nation_address = 0x009cf5ac; title_address = 0x009cfab8; high = 5000; low = 1500; break;
                case 3: nation_address = 0x009cf5ac; title_address = 0x009cfabc; high = 4500; low = 1000; break;
                case 4: nation_address = 0x009cf5b0; title_address = 0x009cfcf0; high = 0; low = 0; break;
                case 5: nation_address = 0x009cf5b0; title_address = 0x009cfac0; high = 3000; low = 0; break;
                case 6: nation_address = 0x009cf5b0; title_address = 0x009cfac4; high = 2000; low = 0; break;
                case 7: nation_address = 0x009cf5b4; title_address = 0x009cfcf4; high = 0; low = 0; break;
                case 8: nation_address = 0x009cf5b4; title_address = 0x009cfacc; high = 2000; low = 0; break;
                default: nation_address = 0x009cf5b4; title_address = 0x009cfad4; high = 1000; low = 0; break;
                }
                nation = D(nation_address);
                title = D(title_address);
                number = identifier;
                if (row != 9) ++identifier;
                if (large)
                    eax = cm_fn_008af520(allocation, owner, number, title, high, low,
                                        nation, 1, 0, stream, 1);
                else
                    eax = cm_fn_0092fe80(allocation, owner, number, title, high, low,
                                        nation, 1, 0, 0, stream, 1);
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
