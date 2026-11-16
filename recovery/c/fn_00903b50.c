#include "cmre.h"
extern u32 CM_CDECL cm_fn_0053b3d0(u32);
extern unsigned long long CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
#define M(t,a) (*(volatile t *)(a))

void CM_THISCALL cm_fn_00903b50(u32 self)
{
    u8 dl = 0, bl = 0;
    if (M(s8, self + 0x438) > 0) {
        do {
            u32 entry = self + (u32)((s32)(s8)dl * 18);
            u16 year = CM_GLOBAL(volatile u16, 0x00ae2c92);
            u32 obj = M(u32, entry + 4);
            if (year != 0x7d1) {
                unsigned i;
                for (i = 0; i < 10; ++i) {
                    u32 v = M(u32, obj + 0xe8 + i * 4);
                    M(u32, obj + 0xe0 + i * 4) = v;
                    bl = (u8)v;
                }
                bl = M(u8, entry + 0x11);
                if (bl) {
                    s32 numerator = M(s16, entry + 0xf);
                    s32 denominator = (s8)bl;
                    M(double, obj + 0x108) = (double)((long double)numerator / (long double)denominator);
                    bl = 0;
                } else {
                    bl = 0;
                    M(u32, obj + 0x108) = 0;
                    M(u32, obj + 0x10c) = 0;
                }
            }
            {
                long double sum = M(double, obj + 0x100);
                sum += M(double, obj + 0xf8);
                dl++;
                sum += M(double, obj + 0xf0);
                sum += M(double, obj + 0xe8);
                sum += M(double, obj + 0xe0);
                M(float, entry + 8) = (float)sum;
            }
            M(u8, entry + 0x11) = bl;
            M(u16, entry + 0xf) = bl;
        } while ((s8)dl < M(s8, self + 0x438));
    }
    cm_fn_00945333(self, (u32)(s32)M(s8, self + 0x438), 0x12, 0x905cb0);
    if (M(s8, self + 0x438) > (s8)bl) {
        do {
            u32 entry = self + (u32)((s32)(s8)bl * 18);
            u32 obj = cm_fn_0053b3d0(M(u32, entry + 4));
            M(u8, obj + 0x1df) = bl;
            {
                s8 count = M(s8, self + 0x438);
                bl++;
                if ((s8)bl >= count) break;
            }
        } while (1);
    }
}
