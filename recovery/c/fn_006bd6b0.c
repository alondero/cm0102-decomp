#include "cmre.h"

extern s32 cm_fn_0090cfc0(s32 param_1);

/* Thiscall with one stack argument, a byte in the low 8 bits (ret 4). ECX (param_1) is the
   base address C. The argument's stack slot is overwritten with the pointer
   p = (s8)param_2 + C + 6 before any use, and the final store goes through that slot.
   The accumulator is 16 bits wide (the original reads only SI at the end), and the
   byte-table scans use the offsets listed below. */
void CM_THISCALL cm_fn_006bd6b0(u32 param_1, s8 param_2)
{
    u32 slot;
    u32 p;
    u32 acc;
    u32 edi;
    u32 ptr;
    s8 bl;
    s16 cnt;
    int n;
    s32 r;
    s32 t;

    p = (u32)(s32)param_2 + param_1 + 6U;
    slot = p;
    *(u8 *)p = 0;
    acc = 0;
    if (param_2 == 0) {
        bl = 5;
        edi = param_1 + 0x2379U;
        do {
            cnt = (s16)(6 - bl);
            ptr = edi;
            for (n = 9; n != 0; n--) {
                acc += (u32)(s32)((s16)(*(s8 *)ptr) * cnt);
                ptr += 0x438U;
            }
            bl--;
            edi -= 0x5aU;
        } while (bl >= 0);
    } else {
        bl = 6;
        edi = param_1 + 0x23d2U;
        do {
            cnt = (s16)(bl - 5);
            ptr = edi;
            for (n = 9; n != 0; n--) {
                acc += (u32)(s32)((s16)(*(s8 *)ptr) * cnt);
                ptr += 0x438U;
            }
            bl++;
            edi += 0x5aU;
        } while (bl < 12);
    }
    r = cm_fn_0090cfc0(0xf);
    t = (s32)((u32)r + 0x1eU);
    if ((s32)(s16)acc > t) {
        *(u8 *)slot = 1;
    }
}
