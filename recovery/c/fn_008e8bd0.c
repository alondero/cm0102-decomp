/* Recovered C for preferred-image VA 0x008e8bd0 (image base 0x00400000).
   Checked statement by statement against the Ghidra listing for
   fn_008e8bd0 (109 bytes, `ret 0x1c` -> __stdcall, 7 stack words). */
#include "cmre.h"

#ifndef CM_STDCALL_FN
#define CM_STDCALL_FN CM_STDCALL
#endif

/* Word global at 0x00ae2c92; the byte at 0x00ae2c90 is only used as an
   address (ECX for the legacy fastcall callee), never loaded here. */
#define G_WORD_00ae2c92 CM_GLOBAL(u16, 0x00ae2c92)

extern int CM_FASTCALL cm_fn_0054cfb0(short *param_1);
extern CM_STDCALL_FN u32 cm_fn_004d45f0(u32 object);

void CM_STDCALL cm_fn_008e8bd0(u32 param_1, u32 param_2, u8 *param_3,
                               u16 *param_4, u8 *param_5, u16 *param_6,
                               u32 param_7)
{
    int iVar1;
    u32 uVar2;

    (void)param_2;
    (void)param_7;

    /* 008e8bd2: mov ecx, 0xae2c90  (fastcall first argument) */
    iVar1 = cm_fn_0054cfb0((short *)0x00ae2c90);

    /* 008e8be4: inc al ; 008e8be6: mov byte ptr [esi], al */
    param_3[0] = (u8)(iVar1 + 1);
    /* 008e8be8/008e8bee: 16-bit copy of the global word */
    param_4[0] = G_WORD_00ae2c92;

    /* 008e8bf1-008e8bfa: signed compare of the byte against 0x0b */
    if ((s8)param_3[0] > 11) {
        param_3[0] = 0;
        param_4[0] = (u16)(param_4[0] + 1);
    }

    /* 008e8bfd-008e8c08: argument pushed, ECX = dword ptr [0x00ae1050] */
    uVar2 = cm_fn_004d45f0(param_1);

    /* 008e8c0d/008e8c11: low byte of the result stored */
    param_5[0] = (u8)uVar2;

    /* 008e8c13-008e8c3a: signed compare of the byte against AL */
    if ((s8)param_3[0] < (s8)(u8)uVar2) {
        /* 008e8c19-008e8c25: copy the (possibly bumped) word */
        param_6[0] = param_4[0];
    } else {
        /* 008e8c28-008e8c3a: global word + 1 */
        param_6[0] = (u16)(G_WORD_00ae2c92 + 1);
    }
}
