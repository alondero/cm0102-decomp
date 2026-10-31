#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 index);

void CM_THISCALL cm_fn_0048cc70(volatile u8 *self, u32 search,
                               volatile u16 *out_word,
                               volatile u16 *out_extra,
                               volatile u8 *out_byte)
{
    s32 list_index = *(volatile s32 *)(self + 0x30);
    if (list_index < -1) return;
    for (;;) {
        volatile u8 *obj = self;
        if (list_index > -1) {
            u32 list = *(volatile u32 *)(self + 0x0c);
            obj = (volatile u8 *)*(volatile u32 *)(list + (u32)list_index * 4);
        }
        if (*(volatile u16 *)(obj + 0x3a) != 0x03fc) {
            if (*(volatile u8 *)(obj + 0x43) == 2) {
                s8 slot = 0;
                if (*(volatile s16 *)(obj + 0x3e) > 0) {
                    do {
                        u32 address = cm_fn_006826d0((void *)obj,
                                                    (u32)(s32)slot);
                        u32 found = *(volatile u32 *)address;
                        if (found == search) {
                            u16 word = *(volatile u16 *)(obj + 0x3a);
                            *(volatile u16 *)out_word = word;
                            address = cm_fn_006826d0((void *)obj,
                                                    (u32)(s32)slot);
                            u8 value = *(volatile u8 *)(address + 0x37);
                            *(volatile u8 *)out_byte = value;
                            return;
                        }
                        ++slot;
                    } while ((s16)slot < *(volatile s16 *)(obj + 0x3e));
                }
            } else {
                s16 i = 0;
                s16 found_index = -1;
                if (*(volatile s16 *)(obj + 0x3e) > 0) {
                    do {
                        u32 table = *(volatile u32 *)(obj + 0xa7);
                        s32 offset = (s32)i * 6;
                        u32 entry = *(volatile u32 *)(table + (u32)offset);
                        if (entry == search) {
                            volatile u16 *dst = (volatile u16 *)out_word;
                            u16 word = *(volatile u16 *)(obj + 0x3a);
                            *dst = word;
                            table = *(volatile u32 *)(obj + 0xa7);
                            found_index = i;
                            u8 value = *(volatile u8 *)(table + (u32)offset + 5);
                            *(volatile u8 *)out_byte = value;
                        }
                        ++i;
                    } while (i < *(volatile s16 *)(obj + 0x3e));
                    if (found_index != -1) {
                        s32 outer = (s32)*(volatile s8 *)(obj + 0xab);
                        --outer;
                        if ((s16)outer >= 0) {
                            for (;;) {
                                s32 row = (s16)outer;
                                u32 record_offset = (u32)(row * 0x68);
                                u32 records = *(volatile u32 *)(obj + 0xa3);
                                s16 count = *(volatile s16 *)(records + record_offset + 0x1a);
                                if (count > 0) {
                                    u32 rows = *(volatile u32 *)(obj + 0xad);
                                    u32 row_ptr = *(volatile u32 *)(rows + (u32)row * 4);
                                    s16 n = 0;
                                    do {
                                        u32 off = (u32)(s32)n * 0x35;
                                        if (*(volatile s16 *)(row_ptr + off) == found_index ||
                                            *(volatile s16 *)(row_ptr + off + 2) == found_index) {
                                            records = *(volatile u32 *)(obj + 0xa3);
                                            u16 extra = *(volatile u16 *)(records + record_offset + 7);
                                            *(volatile u16 *)out_extra = extra;
                                            return;
                                        }
                                        ++n;
                                    } while (n < count);
                                }
                                --outer;
                                if ((s16)outer < 0) break;
                            }
                        }
                    }
                }
            }
        }
        --list_index;
        if (list_index < -1) return;
    }
}
