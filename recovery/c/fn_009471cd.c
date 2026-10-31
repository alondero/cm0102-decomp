/* WIP: one failed invocation; cases 2, 44 and 87 have GetStringTypeA
   source-pointer detail mismatches. */
#include "cmre.h"

extern s32 CM_CDECL cm_fn_0094c0fe(u32 locale, u32 flags,
                                    const void *source, s32 count,
                                    void *destination, s32 capacity,
                                    u32 code_page, s32 option);
extern u32 CM_CDECL cm_fn_0094c322(u32 value, u32 mask);

u32 CM_CDECL cm_fn_009471cd(u32 value)
{
    u32 result = value;
    s32 code_page = (s32)CM_GLOBAL(volatile u32, 0x00de3048u);

    if (code_page == 0) {
        if ((s32)value >= 0x61 && (s32)value <= 0x7a)
            result = value - 0x20u;
        return result;
    }

    if ((s32)value < 0x100) {
        u32 classification;
        s32 class_mode = (s32)CM_GLOBAL(volatile u32, 0x00ad91dcu);
        if (class_mode <= 1) {
            u32 map = CM_GLOBAL(volatile u32, 0x00ad8fd0u);
            u32 index = value * 2u;
            classification = *(volatile u8 *)(map + index) & 2u;
        } else {
            classification = cm_fn_0094c322(value, 2u);
        }
        if (classification == 0u)
            return result;
    }

    {
        u8 low_byte = (u8)value;
        u32 map = CM_GLOBAL(volatile u32, 0x00ad8fd0u);
        u32 high_byte = (value >> 8) & 0xffu;
        u8 is_lead = *(volatile u8 *)(map + high_byte * 2u + 1u);
        volatile u8 *input = (volatile u8 *)&value;
        u32 input_count;
        u32 output;

        if ((is_lead & 0x80u) != 0u) {
            input[2] = 0;
            input[0] = (u8)high_byte;
            input[1] = low_byte;
            input_count = 2u;
        } else {
            input[1] = 0;
            input[0] = (u8)value;
            input_count = 1u;
        }

        code_page = (s32)CM_GLOBAL(volatile u32, 0x00de3048u);
        s32 converted = cm_fn_0094c0fe((u32)code_page, 0x200u,
                                       (const void *)&value,
                                       (s32)input_count, (void *)&output,
                                       3, 0u, 1);
        if (converted != 0) {
            volatile u8 *out_bytes = (volatile u8 *)&output;
            if (converted == 1)
                result = out_bytes[0];
            else
                result = ((u32)out_bytes[1] << 8) | out_bytes[0];
        }
    }
    return result;
}
