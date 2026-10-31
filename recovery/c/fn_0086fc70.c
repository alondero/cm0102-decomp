#include "cmre.h"

/* Clears a 210-byte (0xd2) record and stamps -1 into six dword fields.
   The record is the element at index (index - count + G2 + 16) of the table
   at self+0x08 when the index is within [count-16, count), otherwise the
   record is the argument pointer itself. Stride 210 bytes; G2 is the dword at
   0x00ae2368; count is the signed dword at 0x00ae2364. */
#define OBJ_U32(self, off) (*(volatile u32 *)((self) + (off)))

void CM_THISCALL cm_fn_0086fc70(u32 self, u32 arg)
{
    u32 count;
    u32 index;
    u32 origin;
    u32 table;
    u32 dest;
    u32 i;

    count = CM_GLOBAL(volatile u32, 0x00ae2364);
    index = *(volatile u32 *)arg;
    if ((s32)index >= (s32)(count - 0x10u) && (s32)index < (s32)count) {
        origin = CM_GLOBAL(volatile u32, 0x00ae2368);
        table = OBJ_U32(self, 0x08);
        dest = table + ((index - count + origin + 0x10u) * 210u);
    } else {
        dest = arg;
    }

    for (i = 0u; i < 0x34u; i++) {
        *(volatile u32 *)(dest + i * 4u) = 0u;
    }
    *(volatile u16 *)(dest + 0xd0u) = 0u;

    *(volatile u32 *)(dest + 0x86u) = 0xffffffffu;
    *(volatile u32 *)(dest + 0xc2u) = 0xffffffffu;
    *(volatile u32 *)(dest + 0xc6u) = 0xffffffffu;
    *(volatile u32 *)(dest + 0x4au) = 0xffffffffu;
    *(volatile u32 *)(dest + 0x5eu) = 0xffffffffu;
    *(volatile u32 *)(dest + 0x72u) = 0xffffffffu;
}
