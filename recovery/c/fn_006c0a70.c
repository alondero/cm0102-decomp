#include "cmre.h"

/* 0x006c0aa5 sets ECX=ESI before CALL at 0x006c0aa7, with no
   stack operands (same pinned image identified below). */
extern u32 CM_THISCALL cm_fn_00717410(u32);

void CM_THISCALL cm_fn_006c0a70(u32 self, s8 index, s16 value)
{
    u32 edx = (u32)(s32)index;
    u32 eax = edx * 1115u;
    u32 p = self + eax * 8u + 0x4796u;
    unsigned int n;
    for (n = 0; n != 20; ++n, p += 0x1beu) {
        u8 b = *(volatile u8 *)(p + 0x19u);
        eax = (eax & 0xffffff00u) | b;
        if ((s8)b >= 0) {
            u32 flags = cm_fn_00717410(p);
            u32 a = 0;
            eax = flags;
            if (flags & 1) a = 0x96d330;
            else if (flags & 0x1e) a = 0x968968;
            else if (flags & 0x20) a = 0x969510;
            else if (flags & 0x40) a = 0x968918;
            if (a) {
                long double t = (long double)value * *(volatile double *)a;
                /* Preferred-image VA 0x006c0b4e calls __ftol (0x00945640).
                   Pinned 3.9.68 SHA-256:
                   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
                   C.md's compiler-helper lowering uses a long-double-to-i64
                   cast, not an ordinary extern call losing the ST0 operand.
                   The original FILD/FMUL sites are 0x006c0ab7/0x006c0abb,
                   with equivalent sequences through 0x006c0b44/0x006c0b48.
                   Control-word equivalence still requires gate evidence. */
                u16 r = (u16)(long long)t;
                *(volatile u16 *)(p + 0x35u) += r;
                eax = r;
            }
        }
    }
}
