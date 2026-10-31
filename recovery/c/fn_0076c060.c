/* Preferred-image VA 0076c060, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))

extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern u32 CM_CDECL cm_fn_0053b4f0(u32 nation);

u32 CM_THISCALL cm_fn_0076c060(u32 self, u32 nation)
{
    u32 eax, current, later, scan, value;
    u8 pass = 0, index, next;
    (void)self;

    do {
        if (pass == 0)
            eax = cm_fn_0053b3d0(nation);
        else
            eax = cm_fn_0053b4f0(nation);
        current = eax + 0xd7u;
        later = eax + 0xdbu;
        index = 0;
        do {
            value = D(current);
            if (value != 0 && index != 0x32u) {
                next = (u8)(index + 1u);
                eax = (eax & 0xffffff00u) | next;
                scan = later;
                if ((s8)next < 0x32) {
                    eax = 0x32u - (u32)(s32)(s8)next;
                    do {
                        if (D(scan) == value)
                            D(scan) = 0;
                        scan += 4u;
                        --eax;
                    } while (eax != 0);
                }
            }
            index = (u8)(index + 1u);
            later += 4u;
            current += 4u;
        } while ((s8)index < 0x32);
        pass = (u8)(pass + 1u);
        eax = (eax & 0xffffff00u) | pass;
    } while ((s8)pass < 2);
    return eax;
}
