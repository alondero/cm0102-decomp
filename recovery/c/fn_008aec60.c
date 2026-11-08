#include "cmre.h"

extern int CM_STDCALL cm_fn_00944c3a(u32 s);

void CM_THISCALL cm_fn_008aec60(u8 *self)
{
    volatile u32 *p = (volatile u32 *)(self + 0x288u);
    u32 n = 0x14u;

    do {
        u32 v = *p;
        if (v != 0xffffffffu) {
            cm_fn_00944c3a(v);
            *p = 0xffffffffu;
        }
        p++;
    } while (--n != 0);
}
