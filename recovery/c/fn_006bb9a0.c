#include "cmre.h"

/* Object (ECX) fields by byte offset: 0x475f word, 0x4761 dword (also the
   guard: stores happen only while it is zero), 0x4765 dword, 0x4769 byte,
   0x475a..0x475e bytes. Arguments are nine stack slots (ret 0x24): slot 0x04
   low word, slot 0x08 and 0x0c dwords, slots 0x10..0x24 low bytes. */

void CM_THISCALL cm_fn_006bb9a0(u32 self, u16 a04, u32 a08, u32 a0c,
                                u8 a10, u8 a14, u8 a18, u8 a1c,
                                u8 a20, u8 a24)
{
    if (*(volatile u32 *)(self + 0x4761u) == 0u) {
        *(volatile u16 *)(self + 0x475fu) = a04;
        *(volatile u32 *)(self + 0x4761u) = a08;
        *(volatile u32 *)(self + 0x4765u) = a0c;
        *(volatile u8 *)(self + 0x4769u) = a10;
        *(volatile u8 *)(self + 0x475au) = a14;
        *(volatile u8 *)(self + 0x475bu) = a18;
        *(volatile u8 *)(self + 0x475cu) = a1c;
        *(volatile u8 *)(self + 0x475du) = a20;
        *(volatile u8 *)(self + 0x475eu) = a24;
    }
}
