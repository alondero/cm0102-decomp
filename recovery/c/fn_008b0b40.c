/* Preferred-image VA 008b0b40, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   008b394d and 008b227d consume only the mode argument's low byte.
   The original's unwritten/preserved upper argument bits are never consumed
   or forwarded by either callee; these declarations name the observed width. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_008b3930(u32 self, u32 value, u8 mode);
extern void CM_THISCALL cm_fn_008b2250(u32 self, u32 player, u8 mode);
extern u32 CM_THISCALL cm_fn_008b23b0(u32 self, u32 value, u32 source);

u32 CM_THISCALL cm_fn_008b0b40(u32 self)
{
    u32 source = self + 0xfau;
    u32 owner, value, manager, destination, from, word, entry, left;
    u8 mode = 0;
    do {
        owner = D(self + 0x806u);
        value = D(owner + 0xcfu);
        manager = D(0x00de1f44u);
        destination = cm_fn_008b3930(manager, value, mode);
        from = source;
        ++mode;
        left = 0x26;
        do {
            word = D(from);
            D(destination) = word;
            from += 4u;
            destination += 4u;
        } while (--left != 0);
        word = W(from);
        W(destination) = (u16)word;
        from += 2u;
        destination += 2u;
        source += 0x9bu;
        word = B(from);
        B(destination) = (u8)word;
    } while (mode < 0xa);

    entry = self;
    left = 0x32;
    do {
        value = D(entry);
        if (value != 0) {
            mode = B(entry + 4u);
            manager = D(0x00de1f44u);
            cm_fn_008b2250(manager, value, mode);
        }
        entry += 5u;
    } while (--left != 0);
    owner = D(self + 0x806u);
    value = D(owner + 0xcfu);
    manager = D(0x00de1f44u);
    (void)cm_fn_008b23b0(manager, value, self + 0x7a3u);
    return 1;
}
