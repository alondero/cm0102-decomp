#include "cmre.h"
/* Preferred-image VA0058c6b0/base00400000, pinned 3.9.68 SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS0/full boolean EAX. Scratch208: output index L0, second output L4,
   diagnostic buffers L8/L108. Original 5fa730 reads only WORD arg2 at
   5fa745; successful AL!=0 paths write both output DWORDs before return. */
#define D(a) CM_GLOBAL(volatile u32, (u32)(a))
#define W(a) CM_GLOBAL(volatile u16, (u32)(a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u8 CM_THISCALL cm_fn_005fa730(u32,u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern u32 CM_CDECL cm_fn_004afd80(u32,u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_THISCALL cm_fn_0058ccb0(u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_00944cff(u32,u32,u32,u32,u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32,u32,...);
extern u32 CM_CDECL cm_fn_005e8290(u32,u32,u32);

static __attribute__((always_inline)) inline void diagnostic(u32 first, u32 second, u32 line)
{
    u32 version;
    cm_fn_00944cff(0x009c7474, 0, 0, first, second);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, first, second, line);
    cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
}

u32 CM_THISCALL cm_fn_0058c6b0(u32 self)
{
    u8 scratch[0x208];
    u32 l = (u32)scratch, eax, year, pointer, competition, receiver;
    u32 index, base, table, count, total, value, first, second, row, nation;
    u8 found;
    D(self + 0xf2) = 8;
    eax = cm_fn_00944e46(0x20);
    D(self + 0xee) = eax;
    if (!eax) {
        diagnostic(l + 0x108, l + 8, 0x35f);
        D(0x00b67a34) = 0;
        return 0;
    }
    year = (eax & 0xffff0000u) | W(self + 0x40);
    pointer = D(self + 4);
    competition = D(pointer);
    receiver = D(0x00b5e134);
    found = cm_fn_005fa730(receiver, competition, year, l, l + 4, 1);
    if (!found) {
        diagnostic(l + 8, l + 0x108, 0x366);
        count = D(0x00ae2350);
        D(0x00b67a34) = 0;
        index = cm_fn_0090cfc0(count);
        D(l) = index;
    } else index = D(l);
    base = D(0x00ae23a8);
    eax = cm_fn_0053b3d0(base + index * 290u);
    table = D(self + 0xee);
    D(table) = eax;
    competition = D(self + 4);
    eax = cm_fn_004afd80(competition, 0);
    table = D(self + 0xee);
    D(table + 4) = eax;
    table = D(self + 0xee);
    if (D(table + 4) == 0) {
        diagnostic(l + 8, l + 0x108, 0x371);
        count = D(0x00ae2350);
        total = D(0x00ae235c);
        D(0x00b67a34) = 0;
        value = cm_fn_0090cfc0(total - count * 2u);
        base = D(0x00ae23b4);
        table = D(self + 0xee);
        D(table + 4) = base + value * 581u;
    }
    table = D(self + 0xee);
    first = D(table);
    second = D(table + 4);
    if (first == second) {
        competition = D(self + 4);
        eax = cm_fn_004afd80(competition, 1);
        table = D(self + 0xee);
        D(table + 4) = eax;
        table = D(self + 0xee);
        if (D(table + 4) == 0) {
            diagnostic(l + 8, l + 0x108, 0x37c);
            count = D(0x00ae2350);
            total = D(0x00ae235c);
            D(0x00b67a34) = 0;
            value = cm_fn_0090cfc0(total - count * 2u);
            base = D(0x00ae23b4);
            table = D(self + 0xee);
            D(table + 4) = base + value * 581u;
        }
    }
    for (row = 2; row < 8; ++row) {
        table = D(self + 0xee);
        switch (row) {
        case 2: nation = D(0x009cf7a4); break;
        case 3: nation = 0xffffffffu; break;
        case 4: nation = D(0x009cf7a8); break;
        case 5: nation = D(0x009cf7ac); break;
        case 6: nation = D(0x009cf8a0); break;
        default: nation = D(0x009cf790); break;
        }
        eax = cm_fn_0058ccb0(self, nation, table, row);
        table = D(self + 0xee);
        D(table + row * 4u) = eax;
    }
    return 1;
}
