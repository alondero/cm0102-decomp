#include "cmre.h"
extern u32 CM_FASTCALL cm_fn_00717410(int);
void CM_THISCALL cm_fn_006c06b0(int self, int index, int value)
{
    u32 p = (u32)self + 0x4796u + (u32)((signed char)index * 0x22d8);
    int i;
    for (i = 0; i < 20; ++i, p += 0x1beu) {
        if (*(signed char *)(p + 0x19u) >= 0) {
            u32 flags = cm_fn_00717410((int)p);
            if (!(flags & 1)) {
                long double t;
                if (flags & 0x2) t = (long double)(short)value * CM_GLOBAL(double, 0x968918);
                else if (flags & 0x4) t = (long double)(short)value * CM_GLOBAL(double, 0x968f20);
                else if (flags & 0x8) t = (long double)(short)value * CM_GLOBAL(double, 0x96d328);
                else if (flags & 0x10) t = (long double)(short)value * CM_GLOBAL(double, 0x969040);
                else if (flags & 0x20) t = (long double)(short)value * CM_GLOBAL(double, 0x968978);
                else if (flags & 0x40) t = (long double)(short)value * CM_GLOBAL(double, 0x968f88);
                else continue;
                /* Original call to __ftol at 0x006c077c. */
                *(u16 *)(p + 0x35u) += (u16)(long long)t;
            }
        }
    }
}

