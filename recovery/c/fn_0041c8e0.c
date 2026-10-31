#include "cmre.h"
/* Preferred-image VA 0x0041c8e0, base 0x00400000, CM0102 3.9.68;
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))

extern void CM_CDECL cm_fn_0041bfb0(void);
extern u32 CM_FASTCALL cm_fn_007fee80(u32 param_1);
extern void CM_THISCALL cm_fn_007feeb0(u32 receiver, u32 param_2);
extern u32 CM_CDECL cm_fn_0090bde0(u32 arg1, u32 arg2);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 param_1);

u32 CM_CDECL cm_fn_0041c8e0(u32 arg1, u32 arg2)
{
    u32 eax, ecx, edx, edi, esi;

    eax = cm_fn_007fee80(0x00b74450);
    if (B(eax) != 0x5c) {
        eax = cm_fn_007fee80(0x00b74450);
        if (B(eax) != 0) {
            return 0;
        }
    }
    /* 0x0041c905 */
    if (D(0x00989858) == 0) {
        cm_fn_007feeb0(0x00b74450, 0x00989adc);
        return 1;
    }
    edi = arg1 * 10u;
    eax = D(edi + 0x00989866);
    if (eax != 0) {
        eax = (u32)cm_fn_0090cfc0((s32)(s16)W(edi + 0x00989864));
        if ((u16)eax != 0xffff) {
            ecx = D(edi + 0x00989866);
            esi = (u32)(s32)(s16)eax << 4;
            eax = esi + ecx;
            ecx = D(esi + ecx + 8);
            if (ecx != 0) {
                edx = (u32)(s32)(s16)W(eax + 4);
                eax = (u32)cm_fn_0090cfc0((s32)edx);
                ecx = D(edi + 0x00989866);
                edx = (u32)(s32)(s16)eax;
                eax = D(esi + ecx + 8);
                eax = D(eax + edx * 4);
                if (eax != 0) {
                    eax = cm_fn_0090bde0(0x009899e0, eax);
                    cm_fn_007feeb0(0x00b74450, eax);
                    return 1;
                }
            }
        }
    }
    /* 0x0041c9a7 */
    if (arg2 != 0) {
        return 0;
    }
    cm_fn_0041bfb0();
    return 1;
}
