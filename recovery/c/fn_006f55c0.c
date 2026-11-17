#include "cmre.h"
#include "cmre_x87.h"

/* Preferred-image VAs below refer to CM0102 3.9.68, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   006f55c2/c4 forwards ECX; 006f55c9/cb pushes the RNG bound. */
extern u32 CM_THISCALL cm_fn_006f44a0(u32 self);
/* 0090cfc0-c2 saves two registers then reads the sole stack argument
   at ESP+0xc. For bound 11, 0090cfcf-d028 overwrites incoming EAX/EDX.
   Thus neither is an input at this call boundary. */
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);

void CM_THISCALL cm_fn_006f55c0(u32 self)
{
    cm_fn_006f44a0(self);
    u32 random = cm_fn_0090cfc0(11);
    if ((u32)random == 0) {
        s32 a = *(volatile s8 *)(self + 0x100);
        u32 p = *(volatile u32 *)(self + 0x6d);
        s32 b = *(volatile s8 *)(p + 0x25);
        s8 c = *(volatile s8 *)(self + 0xff);
        if (c > (a / 2 + b + 10) / 4)
            *(volatile u8 *)(self + 0xff) = (u8)(c - 1);
        {
            long double v = *(volatile float *)(self + 0x4d);
            v += *(volatile float *)(self + 0x59);
            u32 bits = *(volatile u32 *)(self + 0x59);
            *(volatile float *)(self + 0x4d) = (float)v;
            if (bits == 0x3f800000)
                *(volatile u32 *)(self + 0x59) = 0;
            else {
                v = *(volatile float *)(self + 0x59);
                v *= CM_GLOBAL(double, 0x968968);
                *(volatile float *)(self + 0x59) = (float)v;
            }
        }
        {
            u8 age = *(volatile u8 *)(self + 1);
            if (age > 0x32) {
                /* Original _CIpow call at preferred-image VA 0x006f5654. */
                long double v = cm_x87_CIpow((long double)(age - 0x32),
                                              (long double)CM_GLOBAL(double, 0x968928));
                v *= CM_GLOBAL(float, 0x96d22c);
                if (v > CM_GLOBAL(double, 0x968890))
                    v = CM_GLOBAL(float, 0x96af88);
                *(volatile float *)(self + 0x4d) =
                    (float)((long double)*(volatile float *)(self + 0x4d) - v);
            }
        }
        if (!(*(volatile float *)(self + 0x4d) >= CM_GLOBAL(double, 0x967890)))
            *(volatile float *)(self + 0x4d) = (float)CM_GLOBAL(double, 0x967890);
        else if (*(volatile float *)(self + 0x4d) > CM_GLOBAL(double, 0x968910))
            *(volatile float *)(self + 0x4d) = (float)CM_GLOBAL(double, 0x968910);
        else {
            long double v = *(volatile float *)(self + 0x4d);
            *(volatile float *)(self + 0x4d) = (float)v;
        }
    }
}
