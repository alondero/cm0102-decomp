#include "cmre.h"

extern s32 CM_CDECL cm_fn_0090cfc0(s32 value);
extern u32 CM_CDECL cm_fn_00545880(u32 argument);
extern s32 CM_CDECL cm_fn_00951c50(const volatile u8 *first,
                                   const volatile u8 *second);

u32 CM_CDECL cm_fn_0041cad0(u32 argument, u32 lane)
{
    u32 global_count_guard = CM_GLOBAL(volatile u32, 0x00989858u);
    u32 table_guard;
    u32 selected;

    if (global_count_guard == 0) {
        return 0;
    }
    table_guard = CM_GLOBAL(volatile u32, 0x00989870u);
    if (table_guard == 0) {
        return 0;
    }

    if (argument == 0) {
        s16 count = CM_GLOBAL(volatile s16, 0x0098986eu);
        selected = (u32)cm_fn_0090cfc0((s32)count);
    } else {
        u32 index = 0;
        s32 last_match = -1;
        s16 count = CM_GLOBAL(volatile s16, 0x0098986eu);
        if (count <= 0) {
            return 0;
        }
        for (;;) {
            u32 returned = cm_fn_00545880(argument);
            u32 row_base = CM_GLOBAL(volatile u32, 0x00989870u);
            u32 signed_row_offset = (u32)(s32)(s16)(u16)index * 0x10u;
            u32 row_string = *(volatile u32 *)(row_base + signed_row_offset);
            if (cm_fn_00951c50((const volatile u8 *)row_string,
                               (const volatile u8 *)returned) == 0) {
                last_match = (s32)index;
            }
            index++;
            {
                u16 index_word = (u16)index;
                s16 live_count = CM_GLOBAL(volatile s16, 0x0098986eu);
                if ((s16)index_word >= live_count) {
                    break;
                }
            }
        }
        selected = (u32)last_match;
    }

    if ((u16)selected == 0xffffu) {
        return 0;
    }
    {
        s32 row_index = (s32)(s16)(u16)selected;
        u32 row_offset = (u32)row_index << 4;
        if (lane != 0) {
            u32 table_base = CM_GLOBAL(volatile u32, 0x00989870u);
            u32 nested = *(volatile u32 *)(table_base + row_offset + 0x0cu);
            s16 value;
            s32 nested_index;
            u32 *nested_after;
            if (nested == 0) {
                return 0;
            }
            value = *(volatile s16 *)(table_base + row_offset + 6u);
            nested_index = cm_fn_0090cfc0((s32)value);
            nested_index = (s32)(s16)(u16)nested_index;
            table_base = CM_GLOBAL(volatile u32, 0x00989870u);
            nested_after = (u32 *)(*(volatile u32 *)(table_base +
                                                      row_offset + 0x0cu));
            return *(volatile u32 *)((u32)nested_after +
                                     (u32)nested_index * 4u);
        } else {
            u32 table_base = CM_GLOBAL(volatile u32, 0x00989870u);
            u32 nested = *(volatile u32 *)(table_base + row_offset + 8u);
            s16 value;
            s32 nested_index;
            u32 *nested_after;
            if (nested == 0) {
                return 0;
            }
            value = *(volatile s16 *)(table_base + row_offset + 4u);
            nested_index = cm_fn_0090cfc0((s32)value);
            nested_index = (s32)(s16)(u16)nested_index;
            table_base = CM_GLOBAL(volatile u32, 0x00989870u);
            nested_after = (u32 *)(*(volatile u32 *)(table_base +
                                                      row_offset + 8u));
            return *(volatile u32 *)((u32)nested_after +
                                     (u32)nested_index * 4u);
        }
    }
}
