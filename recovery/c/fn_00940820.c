#include "cmre.h"

/* Callee boundary: 0x00940290 is cdecl with five stack arguments.

   Frame: the listing's 0x1c4-byte frame is one local array. frame[0] is the
   count slot (B+0). Records start at frame[1] (B+4, the listing's esi base)
   and take seven DWORDs each: P, Q, R, S, arg_6, arg_7, count.

   Object field: DWORD at byte offset 0x21fc from arg_1 is the threshold.

   Return: the listing returns with EAX equal to the stack address of the
   record base, so the function is declared void. */

extern void CM_CDECL cm_fn_00940290(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5);

void CM_CDECL cm_fn_00940820(u32 arg_1, s32 arg_2, s32 arg_3, s32 arg_4, s32 arg_5, s32 arg_6, s32 arg_7)
{
    s32 frame[0x1c4 / 4];
    s32 a;
    s32 b;
    s32 count;
    s32 threshold;
    s32 n;
    s32 p;
    s32 q;
    s32 r;
    s32 s;

    a = (s32)((u32)arg_2 - (u32)arg_4 * 2u + (u32)arg_6) >> 2;
    if (a < 0) {
        a = (s32)(0u - (u32)a);
    }
    b = (s32)((u32)arg_3 - (u32)arg_5 * 2u + (u32)arg_7) >> 2;
    if (b < 0) {
        b = (s32)(0u - (u32)b);
    }
    if (b > a) {
        a = b;
    }

    threshold = (s32)*(volatile u32 *)(arg_1 + 0x21fc);
    count = 0;
    if (a > threshold) {
        do {
            a = a >> 2;
            count = count + 1;
        } while (a > threshold);
    }

    n = 1;
    for (;;) {
        if (count > 0) {
            p = (s32)((u32)arg_2 + (u32)arg_4 * 2u + 2u + (u32)arg_6) >> 2;
            q = (s32)((u32)arg_3 + (u32)arg_5 * 2u + 2u + (u32)arg_7) >> 2;
            r = (s32)((u32)arg_6 + (u32)arg_4 + 1u) >> 1;
            s = (s32)((u32)arg_5 + (u32)arg_7 + 1u) >> 1;
            frame[n + 0] = p;
            frame[n + 1] = q;
            frame[n + 2] = r;
            frame[n + 3] = s;
            frame[n + 4] = arg_6;
            frame[n + 5] = arg_7;
            count = count - 1;
            frame[n + 6] = count;
            n = n + 7;
            arg_4 = (s32)((u32)arg_2 + (u32)arg_4 + 1u) >> 1;
            arg_5 = (s32)((u32)arg_3 + (u32)arg_5 + 1u) >> 1;
            arg_6 = p;
            arg_7 = q;
            continue;
        }

        cm_fn_00940290(arg_1, (u32)arg_2, (u32)arg_3, (u32)arg_6, (u32)arg_7);
        if (n <= 1) {
            break;
        }
        count = frame[n - 1];
        arg_7 = frame[n - 2];
        arg_6 = frame[n - 3];
        arg_5 = frame[n - 4];
        arg_4 = frame[n - 5];
        arg_3 = frame[n - 6];
        arg_2 = frame[n - 7];
        n = n - 7;
    }
}
