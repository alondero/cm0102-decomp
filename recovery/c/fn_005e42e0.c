#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e3680(s32 a1, s32 a2, s32 a3, s32 a4, s32 *out);
typedef s32 (CM_STDCALL *cm_slot64_fn)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);
typedef s32 (CM_STDCALL *cm_slot80_fn)(u32 self, u32 a1);

void CM_CDECL cm_fn_005e42e0(u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    volatile u8 scratch[0x94];
    u32 gate = CM_GLOBAL(volatile u32, 0x00aebe0c);
    u32 interface_ptr;
    u32 result;

    if (gate != 0) goto done;
    interface_ptr = CM_GLOBAL(volatile u32, 0x00aebde4);
    if (interface_ptr == 0) goto done;
    result = cm_fn_005e3680((s32)arg1, (s32)arg2, (s32)arg3, (s32)arg4, (s32 *)&scratch[8]);
    if (result == 0) goto done;
    if (CM_GLOBAL(volatile u16, 0x00b0be2e) == 0) {
        u32 blue_mask = CM_GLOBAL(volatile u32, 0x00ae3170);
        u32 green_mask = CM_GLOBAL(volatile u32, 0x00ae316c);
        u32 red_mask = CM_GLOBAL(volatile u32, 0x00ae3168);
        u32 pixel = 0;
        u32 ecx = 0x00aebe30;
        *(volatile u32 *)&scratch[0] = 0;
        *(volatile u32 *)&scratch[4] = ecx;
        for (;;) {
            u32 live_gate = CM_GLOBAL(volatile u32, 0x00aebe0c);
            u32 color;
            if (live_gate != 0) {
                color = 0;
            } else {
                u32 c = *(volatile u32 *)&scratch[0] & 0xffff;
                u32 red = ((c & red_mask) << 8) / (red_mask + 1);
                *(volatile u8 *)&scratch[0x20] = (u8)red;
                u32 green = ((c & green_mask) << 8) / (green_mask + 1);
                *(volatile u8 *)&scratch[0x18] = (u8)green;
                u32 blue = ((c & blue_mask) << 8) / (blue_mask + 1);
                *(volatile u8 *)&scratch[0x24] = (u8)blue;
                s32 r = (((u32)*(volatile u32 *)&scratch[0x20] & 0xff) * 60) / 100;
                s32 g = (((u32)*(volatile u32 *)&scratch[0x18] & 0xff) * 60) / 100;
                *(volatile s32 *)&scratch[0x1c] = r;
                s32 b = (((u32)*(volatile u32 *)&scratch[0x24] & 0xff) * 60) / 100;
                if (r >= 0xff) r = 0xff;
                if (g >= 0xff) g = 0xff;
                if (b >= 0xff) b = 0xff;
                if (green_mask == 0x7e0) {
                    color = ((((u32)r & 0xf8) << 5) | ((u32)g & 0xfffc)) << 3;
                } else {
                    color = ((((u32)r & 0xf8) << 5) | ((u32)g & 0xfff8)) << 2;
                }
                color |= ((u32)b >> 3);
                ecx = *(volatile u32 *)&scratch[4];
            }
            pixel = *(volatile u32 *)&scratch[0];
            *(volatile u16 *)ecx = (u16)color;
            ecx += 2;
            ++pixel;
            *(volatile u32 *)&scratch[0] = pixel;
            *(volatile u32 *)&scratch[4] = ecx;
            if (ecx > 0x00b0be2e) break;
        }
    }

    interface_ptr = CM_GLOBAL(volatile u32, 0x00aebde4);
    {
        *(volatile u32 *)&scratch[0x28] = 0x6c;
        u32 vtable = *(volatile u32 *)interface_ptr;
        u32 slot = *(volatile u32 *)(vtable + 0x64);
        s32 (CM_STDCALL *call64)(u32, u32, u32, u32, u32) = (s32 (CM_STDCALL *)(u32, u32, u32, u32, u32))slot;
        result = (u32)call64(interface_ptr, (u32)(scratch + 8),
                             (u32)(scratch + 0x28), 0x21, 0);
    }
    if ((s32)result != 0) goto done;
    {
        s32 y = *(volatile s32 *)&scratch[0x0c];
        s32 y_end = *(volatile s32 *)&scratch[0x14];
        u32 row = *(volatile u32 *)&scratch[0x4c];
        if (y <= y_end) {
            s32 x_end = *(volatile s32 *)&scratch[0x10];
            s32 x_start = *(volatile s32 *)&scratch[8];
            for (;;) {
                u32 x = (u32)x_start;
                if (x_start <= x_end) {
                    do {
                        u16 palette_index = *(volatile u16 *)row;
                        u16 mapped = *(volatile u16 *)(0x00aebe30 + (u32)palette_index * 2);
                        *(volatile u16 *)row = mapped;
                        row += 2;
                        x = x + 1u;
                        x_end = *(volatile s32 *)&scratch[0x10];
                    } while ((s32)x <= x_end);
                    x_start = *(volatile s32 *)&scratch[8];
                }
                {
                    s32 row_width = *(volatile s32 *)&scratch[0x38];
                    u32 adjusted = (u32)row_width - (row_width < 0 ? 0xffffffffu : 0u);
                    s32 half = (s32)adjusted;
                    half >>= 1;
                    u32 offset = (u32)half;
                    offset = offset - (u32)x_end + (u32)x_start - 1u;
                    row += offset * 2u;
                }
                y = (s32)((u32)y + 1u);
                y_end = *(volatile s32 *)&scratch[0x14];
                if (y > y_end) break;
            }
        }
    }
    interface_ptr = CM_GLOBAL(volatile u32, 0x00aebde4);
    {
        u32 vtable = *(volatile u32 *)interface_ptr;
        u32 slot = *(volatile u32 *)(vtable + 0x80);
        cm_slot80_fn call80 = (cm_slot80_fn)slot;
        (void)call80(interface_ptr, 0);
    }
done:
    return;
}
