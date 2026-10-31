#include "cmre.h"

/* Object fields by byte offset from the object pointer in ECX (dst) and the
   stack argument (src). Accesses stay volatile so the copy order matches the
   listing even if the two pointers alias. */
#define DST_U32(off) (*(volatile u32 *)(self + (off)))
#define SRC_U32(off) (*(volatile u32 *)(src + (off)))

/* Thiscall callee: ECX is the object, two stack arguments popped by the callee. */
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_00448550(u32 self, u32 src)
{
    /* 8-byte scratch at the base of the routine's `sub esp, 8` frame. */
    u32 scratch[2];

    DST_U32(0x00) = SRC_U32(0x00);
    DST_U32(0x04) = SRC_U32(0x04);
    DST_U32(0x08) = SRC_U32(0x08);
    DST_U32(0x0c) = SRC_U32(0x0c);
    DST_U32(0x10) = SRC_U32(0x10);
    cm_fn_00417c10(self + 0x14, (u32)scratch, src + 0x14);
    DST_U32(0x1c) = SRC_U32(0x1c);
    cm_fn_00417c10(self + 0x20, (u32)scratch, src + 0x20);
    *(volatile u8 *)(self + 0x28) = *(volatile u8 *)(src + 0x28);
    return self;
}
