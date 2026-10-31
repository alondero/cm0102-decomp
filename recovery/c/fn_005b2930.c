#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_005b2ba0(u32, u32, u32, u32);
extern u32 CM_THISCALL cm_fn_005b7160(u32);
extern u32 CM_CDECL cm_fn_0066f4e0(u32, u32, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* Original 005b2ba0 reads only the WORD of its second stack argument
   (VA005b2bce/005b2bea); the upper bits of the partial EDX/EAX pushes die. */
u32 CM_THISCALL cm_fn_005b2930(u32 self, u32 record, u32 mode)
{
    u8 scratch[0x200];
    u32 id, base, club, width, selected, index, value, source, destination, text;
    if (record == 0) {
        cm_fn_00944cff(0x009cc3dc, 0, 0, (u32)scratch + 0x100, (u32)scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x100, (u32)scratch, 0xb5);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return self;
    }
    id = (u8)mode == 0 ? D(record + 4) : D(record);
    base = D(0x00ae23b4);
    club = base + id * 581u;
    width = (u32)(s32)(s8)B(club + 0x52);
    cm_fn_0066f4e0(self, 0x009cc3b8, width, width, club + 0x38);
    B(self + 0x33) = 0;
    B(self + 0x65) = 0;
    D(self + 0x143) = 0;
    D(self + 0x147) = 0;
    W(self + 0x14b) = 0;
    B(self + 0x14d) = 0;
    selected = record + (u32)(s32)(s8)mode * 1573u;
    for (index = 0; index != 11; ++index) {
        value = W(selected + 0x154 + index * 2u);
        cm_fn_005b2ba0(self, index, value, 0);
        value = W(selected + 0x16a + index * 2u);
        cm_fn_005b2ba0(self, index, value, 1);
    }
    value = D(selected + 0x180);
    D(self + 0x584) = value;
    source = self + 0x188b;
    D(source) = 0x15552221;
    destination = source + 8;
    D(source + 4) = 10;
    /* Original forward MOVSD overlaps, repeating the first two DWORDs. */
    for (index = 0; index != 20; ++index) {
        value = D(source);
        D(destination) = value;
        source += 4;
        destination += 4;
    }
    D(self + 0x5b4) = 0x12121212;
    D(self + 0x5b8) = 0x12121212;
    W(self + 0x5bc) = 0x1212;
    B(self + 0x5be) = 0x12;
    for (index = 0; index != 11; ++index)
        D(self + 0x5bf + index * 4u) = 0xffffffffu;
    cm_fn_005b7160(self);
    D(self + 0x1883) = 0;
    D(self + 0x1887) = 0;
    D(self + 0x187f) = 1;
    return self;
}
