#include "cmre.h"

/* Thiscall copy: ECX is the destination object and the single stack argument
   (ret 4) is the source object. Returns the destination in EAX.
   Object fields by byte offset, the same layout in source and destination:
   0x00..0x0b dwords, 0x0c dword, 0x10 and 0x18 sub-objects passed to 0x00417c10,
   0x20 byte, 0x21 dword, 0x25 dword, 0x29..0x2c bytes. */
#define FIELD_U32(base, off) (*(volatile u32 *)((base) + (off)))
#define FIELD_U8(base, off) (*(volatile u8 *)((base) + (off)))

/* Callee pops its two stack arguments (thiscall, ret 8). */
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);

u32 CM_THISCALL cm_fn_007af580(u32 self, u32 src)
{
    /* 8-byte scratch at the `sub esp, 8` frame, shared by both sub-object calls. */
    u32 scratch[2];

    FIELD_U32(self, 0x00) = FIELD_U32(src, 0x00);
    FIELD_U32(self, 0x04) = FIELD_U32(src, 0x04);
    FIELD_U32(self, 0x08) = FIELD_U32(src, 0x08);
    FIELD_U32(self, 0x0c) = FIELD_U32(src, 0x0c);

    cm_fn_00417c10((u32 *)(self + 0x10), scratch, (u16 *)(src + 0x10));

    cm_fn_00417c10((u32 *)(self + 0x18), scratch, (u16 *)(src + 0x18));

    FIELD_U8(self, 0x20) = FIELD_U8(src, 0x20);
    FIELD_U32(self, 0x21) = FIELD_U32(src, 0x21);
    FIELD_U32(self, 0x25) = FIELD_U32(src, 0x25);
    FIELD_U8(self, 0x29) = FIELD_U8(src, 0x29);
    FIELD_U8(self, 0x2a) = FIELD_U8(src, 0x2a);
    FIELD_U8(self, 0x2b) = FIELD_U8(src, 0x2b);
    FIELD_U8(self, 0x2c) = FIELD_U8(src, 0x2c);

    return self;
}
