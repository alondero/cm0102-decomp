#include "cmre.h"
/* Preferred-image VA 0x006830b0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

extern u32 CM_STDCALL cm_fn_0085add0(u32 param_1, u32 param_2);
extern void CM_THISCALL cm_fn_004cd120(u32 receiver, u32 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_0069ba50(u32 receiver, u32 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_005a0590(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_00599e00(u32 self, u32 param_2, u32 param_3, u32 param_4, u8 param_5);

u32 CM_STDCALL cm_fn_006830b0(u32 arg1, u32 arg2, u32 arg3)
{
    u32 eax, ecx, edi, esi, key, record, p2, p3;
    u16 dx;

    ecx = D(0x00dd7ef0); /* 006830b0: loaded before the first call, unused */
    esi = arg1;
    eax = cm_fn_0085add0(esi, 0);
    ecx = D(esi + 0x57);
    D(esi + 0x5b) = ecx;
    D(esi + 0x57) = arg2;
    if (eax == 0) {
        ecx = D(0x00dd7ef0);
        eax = cm_fn_0085add0(esi, 0);
        if (eax != 0) {
            eax = 1;
        } else {
            eax = 0;
        }
    } else {
        eax = 0;
    }
    /* 0x006830ef */
    cm_fn_004cd120(D(0x00ae1050), esi, eax);
    eax = arg3;
    if (eax != 0) {
        eax = D(esi + 0xcf);
        if (eax == 0) {
            eax = cm_fn_0069ba50(D(0x00b67a44), esi, 0);
        }
    }
    /* 0x0068311c */
    if (arg2 == 0) {
        return eax;
    }
    edi = D(0x00adadfc);
    if (edi != 0) {
        key = D(arg2);
        eax = D(edi + key * 4);
        if (eax != 0 && D(esi + 0x69) != 0 && B(eax + 0x43) == 2) {
            dx = W(eax + 0xe2);
            if (dx != 0xffff) {
                record = eax;
                p2 = (u32)(s32)(s16)W(record + 0xe4);
                p3 = (u32)(s32)(s16)dx;
                eax = cm_fn_005a0590(D(0x00ae2a38), esi);
                cm_fn_00599e00(eax, p2, p3, key, 0);
                edi = D(0x00adadfc);
            }
        }
    }
    /* 0x00683177 */
    ecx = D(arg2);
    eax = D(edi + ecx * 4);
    if (eax != 0 && B(esi + 0x64) == 3) {
        B(esi + 0x64) = 2;
    }
    return eax;
}
