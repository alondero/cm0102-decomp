#include "cmre.h"

/* Object fields by byte offset.
   self (ECX) is the object this routine fills from the source record.
   dst (first stack argument) and src (second stack argument) are records of
   the same layout. Offsets 0x00 and 0x08..0xed are copied field by field
   below; 0x00 is the vtable pointer, written twice. */
#define SELF_U8(off) (*(volatile u8 *)(self + (off)))
#define SELF_U16(off) (*(volatile u16 *)(self + (off)))
#define SELF_U32(off) (*(volatile u32 *)(self + (off)))
#define SRC_U8(off) (*(volatile u8 *)(src + (off)))
#define SRC_U16(off) (*(volatile u16 *)(src + (off)))
#define SRC_U32(off) (*(volatile u32 *)(src + (off)))
#define DST_U8(off) (*(volatile u8 *)(dst + (off)))
#define DST_U16(off) (*(volatile u16 *)(dst + (off)))
#define DST_U32(off) (*(volatile u32 *)(dst + (off)))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern u32 CM_FASTCALL cm_fn_00687710(u32 src);

void CM_THISCALL cm_fn_00682380(u32 self, u32 dst, u32 src)
{
    u32 i;
    u32 size;
    u32 buffer;
    u32 count;
    u32 tail;
    u32 offset;
    u32 k;

    /* Copy 0xec bytes as dwords, then one word, from src into self. */
    for (i = 0u; i < 0x3bu; i++) {
        *(volatile u32 *)(self + 4u * i) = *(volatile u32 *)(src + 4u * i);
    }
    SELF_U16(0xec) = SRC_U16(0xec);

    size = (u32)(s32)(s16)SELF_U16(0xa9) * 65u;
    buffer = (u32)cm_fn_00944e46(size);
    SELF_U32(0xba) = buffer;

    buffer = SRC_U32(0xba);
    size = (u32)(s32)(s16)SELF_U16(0xa9) * 65u;
    count = size >> 2;
    tail = size & 3u;
    for (k = 0u; k < count; k++) {
        *(volatile u32 *)(SELF_U32(0xba) + 4u * k) = *(volatile u32 *)(buffer + 4u * k);
    }
    offset = 4u * count;
    for (k = 0u; k < tail; k++) {
        *(volatile u8 *)(SELF_U32(0xba) + offset + k) = *(volatile u8 *)(buffer + offset + k);
    }

    SELF_U32(0xb1) = cm_fn_00687710(src);

    SELF_U32(0x18) = 0u;
    SELF_U32(0xea) = 0u;
    SELF_U32(0x14) = 0u;
    SELF_U16(0x38) = 0u;
    SELF_U16(0x36) = 0u;

    DST_U32(0x04) = SELF_U32(0x04);
    DST_U32(0x08) = SELF_U32(0x08);
    DST_U32(0x0c) = SELF_U32(0x0c);
    DST_U32(0x10) = SELF_U32(0x10);
    DST_U32(0x14) = SELF_U32(0x14);
    DST_U32(0x18) = SELF_U32(0x18);
    DST_U32(0x1c) = SELF_U32(0x1c);
    DST_U32(0x20) = SELF_U32(0x20);
    DST_U32(0x24) = SELF_U32(0x24);
    DST_U32(0x28) = SELF_U32(0x28);
    DST_U32(0x2c) = SELF_U32(0x2c);
    DST_U32(0x30) = SELF_U32(0x30);
    DST_U16(0x34) = SELF_U16(0x34);
    DST_U16(0x36) = SELF_U16(0x36);
    DST_U16(0x38) = SELF_U16(0x38);
    DST_U16(0x3a) = SELF_U16(0x3a);
    DST_U16(0x3c) = SELF_U16(0x3c);
    DST_U16(0x3e) = SELF_U16(0x3e);
    DST_U16(0x40) = SELF_U16(0x40);
    DST_U8(0x42) = SELF_U8(0x42);
    DST_U8(0x43) = SELF_U8(0x43);
    DST_U8(0x44) = SELF_U8(0x44);
    DST_U32(0x45) = SELF_U32(0x45);
    DST_U8(0x49) = SELF_U8(0x49);
    DST_U8(0x4a) = SELF_U8(0x4a);
    DST_U8(0x4b) = SELF_U8(0x4b);
    DST_U32(0x4c) = SELF_U32(0x4c);
    DST_U8(0x50) = SELF_U8(0x50);
    DST_U8(0x51) = SELF_U8(0x51);
    DST_U8(0x52) = SELF_U8(0x52);
    for (i = 0u; i < 0x14u; i++) {
        *(volatile u32 *)(dst + 0x53u + 4u * i) = *(volatile u32 *)(self + 0x53u + 4u * i);
    }
    DST_U32(0x00) = 0x009689e8u;
    DST_U32(0xa3) = SELF_U32(0xa3);
    DST_U16(0xa7) = SELF_U16(0xa7);
    DST_U16(0xa9) = SELF_U16(0xa9);
    DST_U16(0xab) = SELF_U16(0xab);
    DST_U32(0xad) = SELF_U32(0xad);
    DST_U32(0xb1) = SELF_U32(0xb1);
    DST_U8(0xb5) = SELF_U8(0xb5);
    DST_U16(0xb6) = SELF_U16(0xb6);
    DST_U16(0xb8) = SELF_U16(0xb8);
    DST_U32(0xba) = SELF_U32(0xba);
    DST_U32(0xbe) = SELF_U32(0xbe);
    DST_U8(0xc2) = SELF_U8(0xc2);
    DST_U8(0xc3) = SELF_U8(0xc3);
    DST_U8(0xc4) = SELF_U8(0xc4);
    DST_U32(0xc5) = SELF_U32(0xc5);
    DST_U32(0xc9) = SELF_U32(0xc9);
    DST_U32(0xcd) = SELF_U32(0xcd);
    DST_U32(0xd1) = SELF_U32(0xd1);
    DST_U32(0xd5) = SELF_U32(0xd5);
    DST_U16(0xd9) = SELF_U16(0xd9);
    DST_U16(0xdb) = SELF_U16(0xdb);
    DST_U32(0xdd) = SELF_U32(0xdd);
    DST_U8(0xe1) = SELF_U8(0xe1);
    DST_U16(0xe2) = SELF_U16(0xe2);
    DST_U16(0xe4) = SELF_U16(0xe4);
    DST_U32(0xe6) = SELF_U32(0xe6);
    DST_U32(0xea) = SELF_U32(0xea);
    DST_U32(0x00) = 0x0096d020u;
}
