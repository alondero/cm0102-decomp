#include "cmre.h"

/* Preferred-image VA 0x004a25f0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
#define ADD_BYTE(dst, src) do { value = B(src); old = D(dst); D(dst) = old + value; } while (0)
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_004a25f0(u32 self, u32 match)
{
    u8 scratch[0x200];
    u32 local = (u32)scratch, text, value, old, side, index, row;
    if (match == 0) {
        cm_fn_00944cff(0x0099ca94, 0, 0, local + 0x100, local);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x100, local, 0x707);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    if (B(match + 0x4c) != 0) return 0;
    value = D(self + 0x5ca);
    D(self + 0x5ca) = value + 1u;
    side = 0;
    do {
        index = 0;
        row = match + side * 0x625u + 0x1c2;
        do {
            if (D(row - 0x32) != 0xffffffffu && D(row - 0x2e) != 0xffffffffu &&
                (index <= 10 || B(row) != 0xff)) {
                ADD_BYTE(self + 0x56a, row + 12);
                ADD_BYTE(self + 0x56e, row + 13);
                ADD_BYTE(self + 0x572, row + 14);
                ADD_BYTE(self + 0x576, row + 15);
                ADD_BYTE(self + 0x57a, row + 16);
                ADD_BYTE(self + 0x57e, row + 17);
                ADD_BYTE(self + 0x582, row - 13);
                ADD_BYTE(self + 0x586, row - 12);
                ADD_BYTE(self + 0x58a, row - 17);
                if (side == 0) {
                    ADD_BYTE(self + 0x58e, row - 13);
                    ADD_BYTE(self + 0x592, row - 12);
                    ADD_BYTE(self + 0x596, row - 17);
                } else {
                    ADD_BYTE(self + 0x59a, row - 13);
                    ADD_BYTE(self + 0x59e, row - 12);
                    ADD_BYTE(self + 0x5a2, row - 17);
                }
                ADD_BYTE(self + 0x5a6, row - 11);
                ADD_BYTE(self + 0x5aa, row - 10);
                ADD_BYTE(self + 0x5ae, row + 21);
                ADD_BYTE(self + 0x5b2, row - 7);
                ADD_BYTE(self + 0x5b6, row - 9);
                ADD_BYTE(self + 0x5ba, row - 8);
                ADD_BYTE(self + 0x5be, row + 3);
                ADD_BYTE(self + 0x5c2, row - 6);
                ADD_BYTE(self + 0x5c6, row - 5);
            }
            ++index;
            row += 0x49;
        } while (index < 20);
        ++side;
    } while (side < 2);
    return 1;
}
