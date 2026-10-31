#include "cmre.h"

/* Thiscall (ECX = object), three stack arguments, ret 0xc, u8 result.
   Object fields by byte offset from ECX: 0x00 dword table base, 0x04 signed
   16-bit entry count. Table entries are 0x22 bytes apart: 0x00 dword key,
   0x06 dword, 0x0a dword, 0x1e byte tag. Stack arguments: arg1 (key) at entry
   [esp+4], arg2 (first output pointer) at [esp+8], arg3 (second output pointer)
   at [esp+0xc]. */
#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U16(off) (*(volatile u16 *)(self + (off)))

u8 CM_THISCALL cm_fn_005fa7d0(u32 self, u32 key, u32 first_out, u32 second_out)
{
    u32 base;
    s16 count;
    u16 index;
    u32 cursor;
    u32 found_value;
    u32 v1;
    u8 tag;
    s32 idx;
    u32 stored;
    u32 sum;
    u8 matched;

    base = OBJ_U32(0x00);
    count = (s16)OBJ_U16(0x04);
    matched = 0;
    found_value = 0;
    index = 0;
    cursor = base;

    if (count > 0) {
        do {
            if (*(volatile u32 *)cursor == key) {
                tag = *(volatile u8 *)(cursor + 0x1e);
                if ((s8)tag >= 0) {
                    /* movsx index from the tag; the byte is read again for the
                       second scaled load, as in the listing. */
                    idx = (s8)*(volatile u8 *)(cursor + 0x1e);
                    v1 = *(volatile u32 *)(cursor + (u32)idx * 8u + 6u);
                    *(volatile u32 *)first_out = v1;
                    idx = (s8)*(volatile u8 *)(cursor + 0x1e);
                    found_value = *(volatile u32 *)(cursor + (u32)idx * 8u + 0xau);
                    matched = 1;
                    break;
                }
                if (tag == 0xfd) {
                    v1 = *(volatile u32 *)(cursor + 6u);
                    *(volatile u32 *)first_out = v1;
                    found_value = *(volatile u32 *)(cursor + 0xau);
                    matched = 1;
                    break;
                }
            }
            index = index + 1;
            cursor = cursor + 0x22u;
        } while ((s16)index < count);
    }

    if (!matched) {
        return 0;
    }

    *(volatile u32 *)second_out = found_value;
    stored = *(volatile u32 *)first_out;
    sum = (stored != 0xffffffffu) + (found_value != 0xffffffffu);
    return (u8)sum;
}
