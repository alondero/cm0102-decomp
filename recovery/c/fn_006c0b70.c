#include "cmre.h"

u32 CM_FASTCALL cm_fn_00717410(void *row);

void CM_THISCALL cm_fn_006c0b70(int self, int index, int value)
{
    s8 idx = (s8)index;
    s16 v = (s16)(u16)value;
    u32 row = (u32)self + ((u32)idx * 223u) * 5u * 8u + 0x4796u;
    int n = 20;

    do {
        if (*(volatile s8 *)(row + 0x19u) >= 0) {
            u8 f = (u8)cm_fn_00717410((void *)row);
            volatile long double t;

            if (f & 1u) {
                t = (long double)(s32)v;
                t = t + t;
            } else if (f & 2u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x009689e0);
            } else if (f & 4u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x00968918);
            } else if (f & 8u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x00968f20);
            } else if (f & 0x10u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x00967888);
            } else if (f & 0x20u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x00968a98);
            } else if (f & 0x40u) {
                t = (long double)(s32)v;
                t *= CM_GLOBAL(double, 0x00968a98);
            } else {
                row += 0x1beu;
                continue;
            }

            /* original conversion call VA 0x006c0c44 */
            long long q = (long long)t;
            *(volatile u16 *)(row + 0x35u) += (u16)q;
        }
        row += 0x1beu;
    } while (--n);
}
