#include "cmre.h"
/* Preferred-image VA004c6430, base00400000; CM0102 3.9.68 SHA256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   THIS2/RET8/fullEAX0 (final table-loop decrement), scratch204 countL0,
   diagnosticsL4/L104. Preserve unsigned year guard, signed BYTE quotas,
   wrapping club addresses, reloads and the distinct BYTE/DWORD store orders.
   4c6730 receives six stack words/RET24; retain physical ECX as set. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define G(a) CM_GLOBAL(volatile u32, a)
extern u32 CM_CDECL cm_fn_004afd80(u32, u32);
extern u32 CM_THISCALL cm_fn_004c6730(u32, u32, u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_004c6430(u32 self, volatile u32 pool_arg,
                              volatile u32 count_arg)
{
    u8 scratch[0x204];
    u32 l = (u32)scratch, id, base, eax, tag, row, first, second, count, version;
    u8 selected;
#define FIND(address, mode) do { \
        id = G(address); base = G(0x00ae23d0); \
        eax = cm_fn_004afd80(base + id * 107u, mode); \
    } while (0)
    FIND(0x009cf63c, 0);
    if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
        tag = G(0x009cf63c);
        B(eax + 0x1df) = 1;
        D(eax + 0x1db) = tag;
    } else {
        cm_fn_00944cff(0x009a7ed8, 0, 0, l + 0x104, l + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, version, l + 0x104, l + 4, 0x114);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        G(0x00b67a34) = 0;
    }
    row = self;
    D(l) = 11;
    do {
        id = D(row); tag = G(0x009cf260);
        selected = 0;
        if (id == tag) {
            if (W(0x00ae2c92) > 0x7d2) {
                FIND(0x009cf9e8, 0);
                if (eax == 0 || D(eax + 0x1db) != 0xffffffffu) {
                    FIND(0x009cf9e8, 1);
                    if (eax == 0 || D(eax + 0x1db) != 0xffffffffu) goto extra_club;
                }
                tag = G(0x009cf63c);
                D(eax + 0x1db) = tag; B(eax + 0x1df) = 2;
                selected = 1;
extra_club:
                id = D(self + 0x71);
                if (id != 0xffffffffu) {
                    base = G(0x00ae23b4); eax = base + id * 581u;
                    if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                        tag = G(0x009cf63c);
                        D(eax + 0x1db) = tag; B(eax + 0x1df) = 2;
                        ++selected;
                    }
                }
            } else {
                id = G(0x009d0274); base = G(0x00ae23b4);
                eax = base + id * 581u;
                if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                    tag = G(0x009cf63c);
                    B(eax + 0x1df) = 2; D(eax + 0x1db) = tag;
                    selected = 1;
                }
                id = G(0x009d026c); base = G(0x00ae23b4);
                eax = base + id * 581u;
                if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                    tag = G(0x009cf63c);
                    D(eax + 0x1db) = tag; B(eax + 0x1df) = 2;
                    ++selected;
                }
            }
            FIND(0x009cf9ec, 0);
            if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                tag = G(0x009cf63c);
                D(eax + 0x1db) = tag; B(eax + 0x1df) = 2;
                ++selected;
            }
            FIND(0x009cf9ec, 1);
            if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                tag = G(0x009cf63c);
                D(eax + 0x1db) = tag; B(eax + 0x1df) = 2;
                ++selected;
            }
        }
        if ((s8)selected < (s8)B(row + 8)) {
            do {
                tag = D(row); second = count_arg; first = pool_arg;
                eax = cm_fn_004c6730(self, first, second, tag, 1, 1, 0);
                if (eax != 0 && D(eax + 0x1db) == 0xffffffffu) {
                    tag = G(0x009cf63c);
                    B(eax + 0x1df) = 2; D(eax + 0x1db) = tag;
                }
                selected = (u8)(selected + 1u);
            } while ((s8)selected < (s8)B(row + 8));
        }
        count = D(l); row += 10; --count; D(l) = count;
    } while (count != 0);
#undef FIND
    return count;
}
