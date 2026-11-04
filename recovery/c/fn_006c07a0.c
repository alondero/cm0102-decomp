#include "cmre.h"

extern u16 CM_FASTCALL cm_fn_00717410(void *row);

#define M(V) (t=(long double)(s32)(s16)(u16)value,t*=CM_GLOBAL(double,V))

/* original conversion call VA 0x006c086d */
void CM_THISCALL cm_fn_006c07a0(int self, int index, int value) {
    u32 row = (u32)self + 0x4796u + (u32)((s32)(s8)index * 0x22d8);
    for (int i = 0; i < 20; i++, row += 0x1beu) {
        volatile s8 *e = (volatile s8 *)(row + 0x19u);
        if (*e < 0)
            continue;

        u16 f = cm_fn_00717410((void *)row);
        volatile u16 *o = (volatile u16 *)(row + 0x35u);
        long double t;
        long long r;

        if (f & 1u)
            M(0x96d330);
        else if (f & 2u)
            M(0x9689b8);
        else if (f & 4u) {
            *o = (u16)(*o + (u16)value);
            continue;
        }
        else if (f & 8u)
            M(0x969520);
        else if (f & 0x10u)
            M(0x968e00);
        else if (f & 0x20u)
            M(0x968f20);
        else if (f & 0x40u)
            M(0x967888);
        else
            continue;

        r = (long long)t;
        *o = (u16)(*o + (u16)r);
    }
}
