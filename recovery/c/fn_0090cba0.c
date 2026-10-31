#include "cmre.h"

extern u32 CM_CDECL cm_fn_009469e0(u32 *stream);

u32 CM_CDECL cm_fn_0090cba0(u32 *stream, u32 output_base)
{
    u32 cursor = 0;
    u8 ch = (u8)cm_fn_009469e0(stream);

    for (;;) {
        if (ch == 0xff) {
            *(volatile u8 *)(u32)(output_base + cursor) = 0;
            return cursor != 0;
        }

        s32 signed_ch = (s8)ch;
        u32 adjusted = (u32)(signed_ch - 9);
        if (adjusted <= 0x1au) {
            u8 target_index = *(volatile u8 *)(0x0090cc34u + adjusted);
            u32 target = *(volatile u32 *)(0x0090cc24u + (u32)target_index * 4u);

            if (target == 0x0090cbebu) {
                if (cursor != 0) {
                    *(volatile u8 *)(u32)(output_base + cursor) = 0;
                    return 1;
                }
            } else if (target == 0x0090cbf1u) {
                if (cursor != 0) {
                    *(volatile u8 *)(u32)(output_base + cursor) = ch;
                    ++cursor;
                }
            } else if (target == 0x0090cbd4u) {
                do {
                    ch = (u8)cm_fn_009469e0(stream);
                } while (ch != 0x0a && ch != 0xff);
            } else if (target == 0x0090cbf5u) {
                *(volatile u8 *)(u32)(output_base + cursor) = ch;
                ++cursor;
            }
        } else {
            *(volatile u8 *)(u32)(output_base + cursor) = ch;
            ++cursor;
        }

        ch = (u8)cm_fn_009469e0(stream);
    }
}
