#include "cmre.h"
/* Preferred-image VA0057bd80/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full boolean EAX. Scratch208: second outputL0, firstL4,
   diagnosticsL8/L108. 5fa730's successful AL paths write both DWORDs;
   its year parameter observes only the lowWORD. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
#define B(a) CM_GLOBAL(volatile u8, (u32)(a))
extern u32 CM_CDECL cm_fn_00945501(u32,u32);
extern u8 CM_THISCALL cm_fn_005fa730(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static __attribute__((always_inline)) inline void diagnostic(u32 first, u32 second, u32 line)
{
    u32 version;
    cm_fn_00944cff(0x009c4638, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
}

u32 CM_THISCALL cm_fn_0057bd80(u32 self)
{
    u8 scratch[0x208];
    u32 l = (u32)scratch, allocation, year, nation, receiver, index;
    u32 table, id, base, value, row, address;
    u16 count;
    u8 found;
    W(self + 0x36) = 0;
    W(self + 0x38) = 16;
    allocation = cm_fn_00945501(16, 6);
    D(self + 0x14) = allocation;
    if (!allocation) {
        diagnostic(l + 0x108, l + 8, 0x32d);
        return 0;
    }
    year = (allocation & 0xffff0000u) | W(self + 0x40);
    nation = D(0x009cf7a4);
    receiver = D(0x00b5e134);
    found = cm_fn_005fa730(receiver, nation, year, l + 4, l, 1);
    if ((s8)found > 0) {
        index = (u32)(s32)(s16)W(self + 0x36);
        table = D(self + 0x14);
        B(table + index * 6u + 4) = 1;
        id = D(l + 4);
        base = D(0x00ae23a8);
        value = cm_fn_0053b3d0(base + id * 290u);
        index = (u32)(s32)(s16)W(self + 0x36);
        table = D(self + 0x14);
        D(table + index * 6u) = value;
        W(self + 0x36) = (u16)(W(self + 0x36) + 1u);
        count = W(self + 0x36);
        if ((s8)found > 1) {
            table = D(self + 0x14);
            index = (u32)(s32)(s16)count;
            B(table + index * 6u + 4) = 1;
            id = D(l);
            base = D(0x00ae23a8);
            value = cm_fn_0053b3d0(base + id * 290u);
            index = (u32)(s32)(s16)W(self + 0x36);
            table = D(self + 0x14);
            D(table + index * 6u) = value;
            W(self + 0x36) = (u16)(W(self + 0x36) + 1u);
        }
    } else diagnostic(l + 8, l + 0x108, 0x345);
    for (row = 0; row < 14; ++row) {
        switch (row) {
        case 0: address = 0x009cf360; break;
        case 1: address = 0x009cf2fc; break;
        case 2: address = 0x009cf488; break;
        case 3: address = 0x009cf45c; break;
        case 4: address = 0x009cf444; break;
        case 5: address = 0x009cf314; break;
        case 6: address = 0x009cf300; break;
        case 7: address = 0x009cf510; break;
        case 8: address = 0x009cf2e4; break;
        case 9: address = 0x009cf2c0; break;
        case 10: address = 0x009cf228; break;
        case 11: address = 0x009cf2ac; break;
        case 12: address = 0x009cf268; break;
        default: address = 0x009cf408; break;
        }
        id = D(address);
        base = D(0x00ae23a8);
        value = cm_fn_0053b3d0(base + id * 290u);
        table = D(self + 0x14);
        D(table + 0xc + row * 6u) = value;
    }
    return 1;
}
