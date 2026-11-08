/* Preferred-image VA 0x009531a6, image base 0x00400000; CM 01/02 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both indexed callers consume the modified record and discard incidental EAX.
   Integer operations implement the original table-driven record arithmetic;
   the table contents and multiplication helper remain original boundaries. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_00952f86(u32 left, u32 right);

void CM_CDECL cm_fn_009531a6(u32 record, u32 exponent, u32 preserve)
{
    u32 table = 0x00ad9938u;
    volatile u32 scratch[3];

    if (exponent == 0)
        return;
    if ((s32)exponent < 0) {
        exponent = 0u - exponent;
        table = 0x00ad9a98u;
    }
    if (preserve == 0)
        CM_GLOBAL(volatile u16, record) = 0;
    while (exponent != 0) {
        u32 index = exponent & 7u;
        u32 element;
        table += 0x54u;
        exponent = (u32)((s32)exponent >> 3);
        if (index == 0)
            continue;
        element = table + index * 12u;
        if (CM_GLOBAL(volatile u16, element) >= 0x8000u) {
            u32 address = (u32)scratch;
            scratch[0] = CM_GLOBAL(volatile u32, element);
            scratch[1] = CM_GLOBAL(volatile u32, element + 4u);
            scratch[2] = CM_GLOBAL(volatile u32, element + 8u);
            CM_GLOBAL(volatile u32, address + 2u) =
                CM_GLOBAL(volatile u32, address + 2u) - 1u;
            element = address;
        }
        (void)cm_fn_00952f86(record, element);
    }
}
