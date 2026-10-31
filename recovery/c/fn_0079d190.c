#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX:
   0x00 base pointer B of a table whose entries are 17 bytes (index * 17) apart.
   Each non-null call clears, for the index read from *arg1, the byte at entry +0x0f,
   the words at +0x08, +0x0a and +0x0c, and the dword at +0x04. The original re-reads
   the index and the base before every store, so each store does the same. */

/* One stack argument (ret 4): a pointer to a u32 index; null does nothing. */
void CM_THISCALL cm_fn_0079d190(u32 self, u32 arg1)
{
    u32 index;
    u32 base;

    if (arg1 == 0) {
        return;
    }

    index = *(volatile u32 *)arg1;
    base = *(volatile u32 *)self;
    *(volatile u8 *)(base + index * 17u + 0x0f) = 0;

    index = *(volatile u32 *)arg1;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + index * 17u + 0x08) = 0;

    index = *(volatile u32 *)arg1;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + index * 17u + 0x0a) = 0;

    index = *(volatile u32 *)arg1;
    base = *(volatile u32 *)self;
    *(volatile u16 *)(base + index * 17u + 0x0c) = 0;

    index = *(volatile u32 *)arg1;
    base = *(volatile u32 *)self;
    *(volatile u32 *)(base + index * 17u + 0x04) = 0;
}
