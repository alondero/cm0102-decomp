/* WIP: void semantic test passes, but acceptance awaits the forwarded EAX
   contract: caller006c54a0 returns it to006b9dd1. That outer caller overwrites
   AL, forwards EAX into byte arguments, and has further partial AX arithmetic.
   The last native007160e0 call clears AL only; upper EAX liveness is unproven.
   Preferred-image VA 006c50d0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_006ceb90(u32 self, u8 first, u8 second, u32 third);
extern unsigned long long CM_THISCALL cm_fn_006f0630(u32 self);
extern void CM_THISCALL cm_fn_006f46b0(u32 self, u8 first, u8 second);
extern void CM_THISCALL cm_fn_007160e0(u32 self, u32 zero);

void CM_THISCALL cm_fn_006c50d0(u32 self)
{
    u8 work[4];
    u32 selected, argument, row, first, second, remaining;
    u8 side, mode, value;
    side = B(self + 0x8eaeu);
    *(volatile u8 *)work = side;
    argument = *(volatile u32 *)work;
    selected = cm_fn_006ceb90(self, (u8)argument, 5, 0);
    cm_fn_006f0630(selected);
    mode = side != 0 ? 1 : 10;
    B(self + 0x8ea7u) = 4;
    B(self + 0x8ea8u) = mode;
    cm_fn_006f46b0(selected, 4, mode);
    row = self + 0x4899u;
    remaining = 20;
    do {
        first = row - 0x103u;
        if ((s8)B(row - 0xeau) >= 0 && first != selected) {
            if (first == D(self + 0xf59eu)) {
                cm_fn_006f46b0(first, 4, 0);
            } else {
                mode = B(row);
                if ((s8)mode < 2) {
                    value = B(row - 1u);
                    cm_fn_006f46b0(first, value, 2);
                } else if ((s8)mode > 9) {
                    value = B(row - 1u);
                    cm_fn_006f46b0(first, value, 9);
                }
            }
        }
        cm_fn_007160e0(first, 0);
        second = row + 0x21d5u;
        if ((s8)B(row + 0x21eeu) >= 0 && second != selected) {
            if (second == D(self + 0xf5a2u)) {
                cm_fn_006f46b0(second, 4, 11);
            } else {
                mode = B(row + 0x22d8u);
                if ((s8)mode < 2) {
                    value = B(row + 0x22d7u);
                    cm_fn_006f46b0(second, value, 2);
                } else if ((s8)mode > 9) {
                    value = B(row + 0x22d7u);
                    cm_fn_006f46b0(second, value, 9);
                }
            }
        }
        cm_fn_007160e0(second, 0);
        row += 0x1beu;
        remaining -= 1u;
    } while (remaining != 0);
}
