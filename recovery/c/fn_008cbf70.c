/* Plain C recovery of the routine at preferred-image VA 0x008cbf70 (image base 0x00400000).
   Thiscall receiver in ECX (self); one stack argument (arg, ret 4).
   Callees: 0x0068d1b0 (fastcall, one ECX argument, returns EAX) and
   0x008cbf40 (thiscall receiver ECX, one stack argument). */
#include "cmre.h"

extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);
extern int CM_THISCALL cm_fn_008cbf40(int param_1, int param_2);

u32 CM_THISCALL cm_fn_008cbf70(int param_1, int param_2)
{
    u32 self = (u32)param_1;
    u32 arg = (u32)param_2;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 edi;
    u8 dl;

    /* Clear bit 0x10 of the map byte at map + key * 8, key = arg->+0x0c. */
    eax = *(volatile u32 *)(arg + 0xc);
    ecx = *(volatile u32 *)(self + 0x86e);
    dl = *(volatile u8 *)(ecx + eax * 8);
    eax = ecx + eax * 8;
    dl = dl & 0xef;
    *(volatile u8 *)eax = dl;

    eax = *(volatile u32 *)(arg + 4);
    if (eax != 0) {
        edx = *(volatile u32 *)(eax + 2);
        *(volatile u32 *)(eax + 0xa) = edx;
        ecx = *(volatile u32 *)(arg + 4);
        eax = cm_fn_0068d1b0((int)ecx);
        if (eax != 0) {
            do {
                eax = *(volatile u32 *)eax;
                ecx = *(volatile u32 *)(self + 0x86e);
                dl = *(volatile u8 *)(ecx + eax * 8);
                eax = ecx + eax * 8;
                dl = dl & 0xef;
                *(volatile u8 *)eax = dl;
                ecx = *(volatile u32 *)(arg + 4);
                eax = cm_fn_0068d1b0((int)ecx);
            } while (eax != 0);
        }
    }

    edx = *(volatile u32 *)(self + 0x928);
    eax = 0;
    if ((s32)edx <= 0) {
        return eax;
    }
    ecx = *(volatile u32 *)(self + 0x920);
    edi = *(volatile u32 *)(arg + 0x20);
    ecx = ecx + 8;
    for (;;) {
        if (*(volatile u32 *)ecx == edi) {
            break;
        }
        eax = eax + 1;
        ecx = ecx + 0xc;
        if ((s32)eax >= (s32)edx) {
            return eax;
        }
    }

    /* Found at index eax: remove that element, shifting later elements down. */
    edx = edx - 1;
    ebx = eax;
    if ((s32)eax < (s32)edx) {
        edi = (eax + eax * 2) << 2;
        do {
            eax = *(volatile u32 *)(self + 0x920);
            edx = edi + eax + 0xc;
            ecx = edi + eax;
            cm_fn_008cbf40((int)ecx, (int)edx);
            eax = *(volatile u32 *)(self + 0x928);
            ebx = ebx + 1;
            edi = edi + 0xc;
            eax = eax - 1;
        } while ((s32)ebx < (s32)eax);
    }
    *(volatile u32 *)(self + 0x928) = *(volatile u32 *)(self + 0x928) - 1;
    return eax;
}
