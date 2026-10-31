#include "cmre.h"

/* Record fields by byte offset from an element pointer (ESI in the listing):
   0x39 dword compared with the outer element, 0x61 pointer to a sub-record whose
   0x2f byte is summed (signed), and whose 0x44 byte (of the CALL result) is weighted.
   Element table: the 50 dword slots at element+0xd7. */
extern int CM_THISCALL cm_fn_004cf5f0(int *param_1, int *param_2, int *param_3);
extern u32 CM_CDECL cm_fn_0053b900(u32 *input);
extern u32 CM_CDECL cm_fn_00544710(u32 arg_04, u32 arg_08, u32 arg_0c);
extern int cm_fn_0090cfc0(int param_1);

void cm_fn_005ce0b0(void)
{
    s32 count;
    u32 elem;
    u32 end;
    u32 slot;
    u32 cand;
    u32 owner;
    u32 hitp;
    u32 ptr;
    s32 sum;
    s32 acc;
    s32 hits;
    s32 i;
    u8 local19;
    s8 cVar2;
    s32 dividend;
    s32 divisor;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 q2;

    count = CM_GLOBAL(volatile s32, 0x00ae235c);
    elem = CM_GLOBAL(volatile u32, 0x00ae23b4);
    end = elem + (u32)count * 0x245u;
    if (elem >= end) {
        return;
    }

    do {
        slot = elem + 0xd7;
        if (cm_fn_0053b900((u32 *)elem) == 0 && cm_fn_0090cfc0(7) == 0) {
            sum = 0;
            acc = 0;
            hits = 0;
            ptr = slot;
            for (i = 0; i < 50; i++) {
                cand = *(volatile u32 *)ptr;
                if (cand != 0
                    && (s32)*(volatile u32 *)cand < (s32)CM_GLOBAL(volatile s32, 0x00ae2364)
                    && *(volatile u32 *)(cand + 0x39) == elem
                    && *(volatile u32 *)(cand + 0x61) != 0) {
                    hitp = (u32)cm_fn_004cf5f0((int *)CM_GLOBAL(volatile u32, 0x00ae1050),
                                               (int *)cand, (int *)elem);
                    if (hitp != 0) {
                        s32 prod;
                        s32 w;

                        owner = *(volatile u32 *)(cand + 0x61);
                        prod = (s32)(s8)*(volatile u8 *)(hitp + 0x44);
                        w = (s32)(s8)*(volatile u8 *)(owner + 0x2f);
                        prod = prod * w;
                        sum = sum + w;
                        acc = acc + prod;
                        hits = hits + 1;
                    }
                }
                ptr = ptr + 4;
            }
            if (hits < 16) {
                sum = sum + (16 - hits) * 10;
            }

            if (sum != 0) {
                ptr = slot;
                for (i = 0; i < 50; i++) {
                    cand = *(volatile u32 *)ptr;
                    if (cand != 0
                        && (s32)*(volatile u32 *)cand < (s32)CM_GLOBAL(volatile s32, 0x00ae2364)
                        && *(volatile u32 *)(cand + 0x39) == elem
                        && *(volatile u32 *)(cand + 0x61) != 0) {
                        hitp = (u32)cm_fn_004cf5f0((int *)CM_GLOBAL(volatile u32, 0x00ae1050),
                                                   (int *)cand, (int *)elem);
                        if (hitp == 0) {
                            local19 = 0;
                        } else {
                            local19 = *(volatile u8 *)(hitp + 0x44);
                        }
                        owner = *(volatile u32 *)(cand + 0x61);
                        dividend = (s32)((u32)acc * 2u);
                        divisor = (s32)(s8)*(volatile u8 *)(owner + 0x2f) + sum;
                        r1 = cm_fn_0090cfc0(dividend / divisor);
                        q2 = (s32)(s8)local19 / 12;
                        r2 = cm_fn_0090cfc0(q2);
                        cVar2 = (s8)((u8)r1 + (u8)r2);
                        if (cVar2 < 0) {
                            r3 = cm_fn_0090cfc0((s32)cVar2);
                            r4 = cm_fn_0090cfc0(-5);
                            if (r3 < r4) {
                                cm_fn_00544710(cand, 0xffffffffu, 1u);
                            }
                        } else if (cVar2 > 0) {
                            r5 = cm_fn_0090cfc0((s32)cVar2);
                            r6 = cm_fn_0090cfc0(5);
                            if (r5 > r6) {
                                cm_fn_00544710(cand, 1u, 1u);
                            }
                        }
                    }
                    ptr = ptr + 4;
                }
            }
        }
        elem = elem + 0x245u;
    } while (elem < end);
}
