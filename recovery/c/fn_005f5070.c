#include "cmre.h"

extern u32 CM_CDECL cm_fn_00945ce6(u32 destination, u32 size, u32 count, u32 stream);

/* Thiscall (ECX = self), one stack argument (`ret 4`), no return value.
   Same shape as fn_005e9fb0 with one difference: the second 0x945ce6 call
   is skipped when the table pointer at self+0xee is null (the listing tests
   it before loading the count and pushing). Field offsets from self: 0xee is
   a pointer to a table of u32 slots, 0xf2 is a signed s32 count, both read
   through volatile accesses. Loop 2 maps each slot value to a global-based
   offset (0x245 per unit, wrapping u32), with 0xffffffff becoming 0. */
void CM_THISCALL cm_fn_005f5070(void *self, u32 stream)
{
    volatile u8 *base = (volatile u8 *)self;
    s32 count;
    s32 index;
    u32 *table;
    u32 elem;

    cm_fn_00945ce6((u32)(base + 0xf2), 4, 1, stream);
    count = *(volatile s32 *)(base + 0xf2);
    index = 0;
    if (count > 0) {
        do {
            table = (u32 *)*(volatile u32 *)(base + 0xee);
            elem = table[index];
            if (elem == 0)
                table[index] = 0xffffffffu;
            else
                table[index] = *(u32 *)elem;
            count = *(volatile s32 *)(base + 0xf2);
            index++;
        } while (index < count);
    }

    {
        u32 table_bits = *(volatile u32 *)(base + 0xee);
        if (table_bits != 0) {
            count = *(volatile s32 *)(base + 0xf2);
            cm_fn_00945ce6(table_bits, 4, (u32)count, stream);
        }
    }

    count = *(volatile s32 *)(base + 0xf2);
    index = 0;
    if (count > 0) {
        do {
            table = (u32 *)*(volatile u32 *)(base + 0xee);
            elem = table[index];
            if (elem == 0xffffffffu)
                table[index] = 0;
            else
                table[index] = (u32)CM_GLOBAL(int, 0x00ae23b4) + elem * 0x245u;
            count = *(volatile s32 *)(base + 0xf2);
            index++;
        } while (index < count);
    }
}
