#include "cmre.h"

/* Callee at 0x00417c10: thiscall. ECX still holds the destination object when it is
   called; its two stack arguments are the 8-byte scratch buffer and the source. */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

/* Copies the record at the source pointer (argument at entry+4) into the object in
   ECX, field by field at the listed offsets and widths, then returns the object.
   Offsets: 0x08 dword, 0x0c word, 0x0e word, 0x10 byte, 0x11 dword, 0x15 dword,
   0x19 byte, 0x1a byte, 0x1b dword. */
u32 CM_THISCALL cm_fn_00448110(u32 self, u32 src)
{
    u32 scratch[2];

    cm_fn_00417c10((u32 *)self, scratch, (u16 *)src);

    *(volatile u32 *)(self + 0x08u) = *(volatile u32 *)(src + 0x08u);
    *(volatile u16 *)(self + 0x0cu) = *(volatile u16 *)(src + 0x0cu);
    *(volatile u16 *)(self + 0x0eu) = *(volatile u16 *)(src + 0x0eu);
    *(volatile u8 *)(self + 0x10u) = *(volatile u8 *)(src + 0x10u);
    *(volatile u32 *)(self + 0x11u) = *(volatile u32 *)(src + 0x11u);
    *(volatile u32 *)(self + 0x15u) = *(volatile u32 *)(src + 0x15u);
    *(volatile u8 *)(self + 0x19u) = *(volatile u8 *)(src + 0x19u);
    *(volatile u8 *)(self + 0x1au) = *(volatile u8 *)(src + 0x1au);
    *(volatile u32 *)(self + 0x1bu) = *(volatile u32 *)(src + 0x1bu);

    return self;
}
