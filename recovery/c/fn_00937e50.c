/* Preferred-image VA 0x00937E50; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_009446e0(u32 factor, u32 value);
extern s32 CM_CDECL cm_fn_009447e0(u32 factor, u16 value);
extern s32 CM_CDECL cm_fn_00944840(u32 first, u32 second);

void CM_CDECL cm_fn_00937e50(u32 context, u32 table, u32 index, u32 factor)
{
    u16 value40 = *(volatile u16 *)(context + 0x40);
    u16 value18 = *(volatile u16 *)(context + 0x18);
    u16 value1a = *(volatile u16 *)(context + 0x1a);
    u32 index_offset = index * 4u;

    if (value40 == 0x4000) {
        if (value18 != 0) {
            u32 base = *(volatile u32 *)table;
            u32 slot = base + index_offset;
            s32 contribution = cm_fn_009447e0(factor, value18);
            u32 current = *(volatile u32 *)slot;
            *(volatile u32 *)slot = current + (u32)contribution;

            base = *(volatile u32 *)(table + 0x28);
            {
                u32 bitmap = base + index;
                u8 old_value = *(volatile u8 *)bitmap;
                *(volatile u8 *)bitmap = (u8)(old_value | 1u);
            }
        }
        if (value1a != 0) {
            u32 base = *(volatile u32 *)(table + 4);
            u32 slot = base + index_offset;
            s32 contribution = cm_fn_009447e0(factor, value1a);
            u32 current = *(volatile u32 *)slot;
            *(volatile u32 *)slot = current + (u32)contribution;

            base = *(volatile u32 *)(table + 0x28);
            *(volatile u8 *)(base + index) |= 2u;
        }
    } else {
        if (value18 != 0) {
            u32 base = *(volatile u32 *)table;
            u32 slot = base + index_offset;
            s32 ratio = cm_fn_00944840((u32)(s32)(s16)value18,
                                       (u32)(s32)(s16)value40);
            s32 contribution = cm_fn_009446e0(factor, (u32)ratio);
            u32 current = *(volatile u32 *)slot;
            *(volatile u32 *)slot = current + (u32)contribution;

            base = *(volatile u32 *)(table + 0x28);
            {
                u32 bitmap = base + index;
                u8 old_value = *(volatile u8 *)bitmap;
                *(volatile u8 *)bitmap = (u8)(old_value | 1u);
            }
        }
        if (value1a != 0) {
            u32 base = *(volatile u32 *)(table + 4);
            u32 slot = base + index_offset;
            s32 ratio = cm_fn_00944840((u32)(s32)(s16)value1a,
                                       (u32)(s32)(s16)value40);
            s32 contribution = cm_fn_009446e0(factor, (u32)ratio);
            u32 current = *(volatile u32 *)slot;
            *(volatile u32 *)slot = current + (u32)contribution;

            base = *(volatile u32 *)(table + 0x28);
            *(volatile u8 *)(base + index) |= 2u;
        }
    }
}
