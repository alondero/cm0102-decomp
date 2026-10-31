#include "cmre.h"

extern void CM_CDECL cm_fn_009452ca(u32 value);

void CM_CDECL cm_fn_0041bad0(void)
{
    volatile u8 *cursor = (volatile u8 *)0x00989866u;
    u32 outer_left = 0x0b;
    do {
        if (*(volatile u32 *)cursor != 0) {
            s16 outer = 0;
            if (*(volatile s16 *)(cursor - 2) > 0) {
                do {
                    u32 outer_base = *(volatile u32 *)cursor;
                    u32 scaled = (u32)(s32)outer * 0x10u;
                    volatile u8 *record = (volatile u8 *)(outer_base + scaled);
                    u32 list = *(volatile u32 *)(record + 8);
                    if (list != 0) {
                        s16 count = *(volatile s16 *)(record + 4);
                        if (count > 0) {
                            s16 item = 0;
                            do {
                                u32 item_offset = (u32)(s32)item * 4u;
                                u32 current_list = *(volatile u32 *)(record + 8);
                                if (*(volatile u32 *)(current_list + item_offset) != 0) {
                                    u32 current_base = *(volatile u32 *)cursor;
                                    u32 current_record_list = *(volatile u32 *)((volatile u8 *)current_base + scaled + 8);
                                    u32 value = *(volatile u32 *)(current_record_list + item_offset);
                                    cm_fn_009452ca(value);
                                }
                                u32 loop_base = *(volatile u32 *)cursor;
                                item = (s16)(item + 1);
                                count = *(volatile s16 *)((volatile u8 *)loop_base + scaled + 4);
                                record = (volatile u8 *)(loop_base + scaled);
                            } while (item < count);
                        }
                        u32 base_for_free = *(volatile u32 *)cursor;
                        u32 list_for_free = *(volatile u32 *)((volatile u8 *)base_for_free + scaled + 8);
                        cm_fn_009452ca(list_for_free);
                        u32 clear_base = *(volatile u32 *)cursor;
                        *(volatile u32 *)((volatile u8 *)clear_base + scaled + 8) = 0;
                        u32 count_base = *(volatile u32 *)cursor;
                        *(volatile u16 *)((volatile u8 *)count_base + scaled + 4) = 0;
                    }

                    u32 second_base = *(volatile u32 *)cursor;
                    volatile u8 *second_record = (volatile u8 *)(second_base + scaled);
                    u32 second_list = *(volatile u32 *)(second_record + 0x0c);
                    if (second_list != 0) {
                        s16 count2 = *(volatile s16 *)(second_record + 6);
                        if (count2 > 0) {
                            s16 item2 = 0;
                            do {
                                u32 item_offset2 = (u32)(s32)item2 * 4u;
                                u32 current_list2 = *(volatile u32 *)(second_record + 0x0c);
                                if (*(volatile u32 *)(current_list2 + item_offset2) != 0) {
                                    u32 current_base2 = *(volatile u32 *)cursor;
                                    u32 current_record_list2 = *(volatile u32 *)((volatile u8 *)current_base2 + scaled + 0x0c);
                                    u32 value2 = *(volatile u32 *)(current_record_list2 + item_offset2);
                                    cm_fn_009452ca(value2);
                                }
                                u32 loop_base2 = *(volatile u32 *)cursor;
                                item2 = (s16)(item2 + 1);
                                count2 = *(volatile s16 *)((volatile u8 *)loop_base2 + scaled + 6);
                                second_record = (volatile u8 *)(loop_base2 + scaled);
                            } while (item2 < count2);
                        }
                        u32 free_base2 = *(volatile u32 *)cursor;
                        u32 list_free2 = *(volatile u32 *)((volatile u8 *)free_base2 + scaled + 0x0c);
                        cm_fn_009452ca(list_free2);
                        u32 clear_base2 = *(volatile u32 *)cursor;
                        *(volatile u32 *)((volatile u8 *)clear_base2 + scaled + 0x0c) = 0;
                        u32 count_base2 = *(volatile u32 *)cursor;
                        *(volatile u16 *)((volatile u8 *)count_base2 + scaled + 6) = 0;
                    }

                    u32 value_base = *(volatile u32 *)cursor;
                    u32 value = *(volatile u32 *)((volatile u8 *)value_base + scaled);
                    if (value != 0) cm_fn_009452ca(value);
                    outer = (s16)(outer + 1);
                } while (outer < *(volatile s16 *)(cursor - 2));
            }
            u32 final_value = *(volatile u32 *)cursor;
            cm_fn_009452ca(final_value);
            *(volatile u32 *)cursor = 0;
            *(volatile u16 *)(cursor - 2) = 0;
        }
        cursor += 0x0a;
        outer_left--;
    } while (outer_left != 0);
}
