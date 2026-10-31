#include "cmre.h"

/* fn_005ae870: thiscall (ECX = object, read before written) with four stack arguments (ret 0x10).
   Preferred-image VA 0x005ae870, image base 0x00400000. Returns 1 or 0 in EAX.
   Scratch S (frame bytes L+0 .. L+0x13, with L = the base after `sub esp, 0x14`):
     S[2] byte local (value of the stack argument 4 low byte, or 0 when it is -1),
     S[3] byte loop index, S[4..7] saved ECX (the object), S[8..0x10] a 9-entry byte array
     (A[k] = S[8+k]); S[0x10] is the 0x0048df40 result, S[0x11..0x12] are 0xff.
   The byte array entries are compared pairwise, equal entries are overwritten with 0xff.
   The stack argument slots are written as locals: slot3 (argument 3) and slot4 (argument 4);
   only their low bytes change, and the full slot3 dword is pushed to 0x005abf10. */

extern u8 CM_CDECL cm_fn_0048df40(int param_1);
extern u32 CM_THISCALL cm_fn_005abf10(int param_1, u32 param_2, u32 param_3, u32 param_4, char param_5);
extern u32 CM_FASTCALL cm_fn_0068d1b0(int param_1);

u32 CM_THISCALL cm_fn_005ae870(u8 *self, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    u8 S[0x14];
    u32 slot3;
    u32 slot4;
    u32 edi0;
    u32 res;
    u32 r68;
    u32 v;
    s32 i;
    s32 n;
    s32 j0;
    s32 kk;
    s32 k2;
    u8 a;
    u8 b;
    u8 dl;

    if (arg1 == 0u) {
        return 0u;
    }

    *(u32 *)(S + 4) = (u32)self;
    S[8] = 0xffu; S[9] = 0xffu; S[10] = 0xffu; S[11] = 0xffu;
    S[8] = 0u; S[9] = 1u; S[10] = 2u; S[11] = 3u;
    S[12] = 0xffu; S[13] = 0xffu; S[14] = 0xffu; S[15] = 0xffu;
    S[12] = 4u; S[13] = 5u; S[14] = 6u; S[15] = 7u;
    S[16] = 0xffu; S[17] = 0xffu; S[18] = 0xffu;
    S[16] = cm_fn_0048df40((int)arg1);

    for (i = 0; i < 8; i++) {
        if (i + 1 < 9) {
            j0 = i + 1;
            n = 9 - j0;
            for (kk = j0; kk < j0 + n; kk++) {
                a = S[8 + i];
                if ((s8)a > -1 && a == S[8 + kk]) {
                    S[8 + kk] = 0xffu;
                }
            }
        }
    }

    slot4 = arg4;
    if ((u8)slot4 == 0xffu) {
        S[2] = 0u;
        slot4 = (slot4 & 0xffffff00u) | 2u;
    } else {
        S[2] = (u8)slot4;
    }

    slot3 = arg3;
    edi0 = arg3;

    S[3] = 0u;
    for (;;) {
        k2 = S[3];
        b = S[8 + k2];
        if (b != 0xffu) {
            dl = (u8)slot4;
            slot3 = (slot3 & 0xffffff00u) | S[2];
            if ((s8)S[2] <= (s8)dl) {
                do {
                    res = cm_fn_005abf10((int)*(u32 *)(S + 4), edi0, arg2, slot3, (char)b);
                    if (res != 0u) {
                        *(volatile u32 *)(res + 0x0au) = *(volatile u32 *)(res + 0x02u);
                        r68 = cm_fn_0068d1b0((int)res);
                        while (r68 != 0u) {
                            v = *(volatile u32 *)(r68 + 0x18u);
                            if (v != 0u && v == arg1) {
                                return 1u;
                            }
                            r68 = cm_fn_0068d1b0((int)res);
                        }
                    }
                    a = (u8)slot3 + 1u;
                    slot3 = (slot3 & 0xffffff00u) | a;
                } while ((s8)a <= (s8)(u8)slot4);
            }
        }
        k2 = S[3] + 1;
        S[3] = (u8)k2;
        if (k2 >= 9) {
            break;
        }
    }

    return 0u;
}
