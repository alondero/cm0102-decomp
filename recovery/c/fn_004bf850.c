#include "cmre.h"

extern u32 CM_CDECL cm_fn_005e8290(const char *message, const char *buffer, int mode);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 format, u32 zero1, u32 zero2,
                                  void *buffer1, void *buffer2);

u32 CM_CDECL cm_fn_004bf850(s8 arg1, s8 arg2, s16 arg3, s16 arg4)
{
    u8 scratch[0x200];
    s16 divisor = arg4;
    s32 eax_bits;
    s8 value;

    if (divisor == 1) {
        s32 sum = (s32)arg1 + (s32)arg2;
        s32 adjusted = sum + (sum < 0 ? 1 : 0);
        eax_bits = (u32)adjusted;
        value = (s8)(u8)(adjusted >> 1);
    } else {
        s32 difference = (s32)arg2 - (s32)arg1;
        s32 product = (s32)((u32)difference * (u32)(s32)arg3);
        s32 signed_divisor = (s32)divisor - 1;
        eax_bits = product / signed_divisor;
        value = (s8)(u8)eax_bits;
    }

    if (value < 0 || value > 100) {
        char *text;
        (void)cm_fn_00944cff(0x0099ec20u, 0u, 0u,
                             scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u,
                             (u32)(unsigned long)text,
                             (u32)(unsigned long)(scratch + 0x100),
                             (u32)(unsigned long)scratch, 0x256eu);
        eax_bits = cm_fn_005e8290((const char *)0x009870e0u,
                                  (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0u;
        if (value < 0)
            return ((u32)eax_bits & 0xffffff00u);
        if (value > 100)
            return ((u32)eax_bits & 0xffffff00u) | 100u;
    }
    return ((u32)eax_bits & 0xffffff00u) | (u32)(u8)value;
}
