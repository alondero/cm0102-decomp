#include "cmre.h"
/* Preferred-image VA0092a8b0, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/fullEAX0or1, scratch208: lookup DWORDs L0/L4, diagnostics L8/L108.
   Lookup success writes both DWORDs; failure explicitly fills L0 with RNG.
   Keep all twelve calls, exact global reloads and physical THIS5/RET20 ABI. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern u8 CM_THISCALL cm_fn_005fa730(u32, u32, u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_0092ae60(u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);

u32 CM_THISCALL cm_fn_0092a8b0(u32 self)
{
    u8 scratch[0x208];
    u32 l = (u32)scratch, eax, p, id, owner, year, array, name, extra;
    D(self + 0xf2) = 12;
    eax = (u32)cm_fn_00944e46(0x30);
    D(self + 0xee) = eax;
    if (eax == 0) {
        cm_fn_00944cff(0x00ad5764, 0, 0, l + 0x108, l + 8);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax, l + 0x108, l + 8, 0x3fb);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        G(0x00b67a34) = 0;
        return 0;
    }
    year = (eax & 0xffff0000u) | W(self + 0x40);
    p = D(self + 4); id = D(p); owner = G(0x00b5e134);
    if (cm_fn_005fa730(owner, id, year, l, l + 4, 1) == 0) {
        cm_fn_00944cff(0x00ad5764, 0, 0, l + 8, l + 0x108);
        eax = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, eax, l + 8, l + 0x108, 0x402);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        p = G(0x00ae2350);
        G(0x00b67a34) = 0;
        eax = cm_fn_0090cfc0(p);
        D(l) = eax;
    }
#define ROW(index, address) do { \
        array = D(self + 0xee); name = G(address); \
        eax = cm_fn_0092ae60(self, name, 0xffffffffu, array, index, 0xffffffffu); \
        array = D(self + 0xee); D(array + (index) * 4u) = eax; \
    } while (0)
    ROW(0, 0x009cfa08);
    ROW(1, 0x009cfa08);
    ROW(2, 0x009cfa0c);
    ROW(3, 0x009cfa0c);
    ROW(4, 0x009cfa10);
    extra = G(0x009cf6f0);
    array = D(self + 0xee); name = G(0x009cfa10);
    eax = cm_fn_0092ae60(self, name, 0xffffffffu, array, 5, extra);
    array = D(self + 0xee); D(array + 0x14) = eax;
    ROW(6, 0x009cfa14);
    ROW(7, 0x009cfa14);
    ROW(8, 0x009cfa18);
    ROW(9, 0x009cfa1c);
    ROW(10, 0x009cfa1c);
#undef ROW
    array = D(self + 0xee); id = D(l);
    eax = cm_fn_0092ae60(self, 0xffffffffu, id, array, 11, 0xffffffffu);
    array = D(self + 0xee); D(array + 0x2c) = eax;
    return 1;
}
