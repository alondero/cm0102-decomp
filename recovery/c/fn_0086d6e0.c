#include "cmre.h"

extern u32 CM_THISCALL cm_fn_005ef330(void *self, u32 a, u32 b, u32 c);

/* Thiscall (ECX = self) with one stack argument A (arg_04 = [entry+4]),
   `ret 4`, no return value. Field offsets: A+0 is a pointer X (its byte at
   +0x11c gates the 0x5ef330 calls), A+4 a pointer B whose dword is compared
   with the global at 0x009cfa10. self+8 is a table base T; the records are
   0xd2 bytes apart starting at T+0xba; each record's dwords at +0, +4, +8, +0xc
   and the dword at -0x34 (an index a) feed the call. The table count is the
   global at 0x00ae2368 plus 16 (re-read each pass), and the record array base
   for 0x5ef330 is the global at 0x00ae23bc plus 132 * a. The callee object is
   the global at 0x00b5ded8 (thiscall with three stack arguments: record base
   pointer, value v, and a record dword). v is [X] when X is non-null, else -1.
   The early branches use the same 0x5ef330 callee on the second match path. */
void CM_THISCALL cm_fn_0086d6e0(void *self, u32 arg_04)
{
    volatile u8 *base = (volatile u8 *)self;
    u32 a;
    u32 b;
    u32 x;
    u32 v;
    u32 t;
    u32 rec;
    u32 idx;
    u32 n;
    u32 i;
    u32 f0;
    u32 f4;
    u32 f8;
    u32 fc;
    u32 sum;
    u32 q;

    a = arg_04;
    b = *(volatile u32 *)(a + 4);
    if (b != 0) {
        x = *(volatile u32 *)b;
        if (x == *(volatile u32 *)0x009cfa10)
            goto check_x;
    }
    if (*(volatile u32 *)a == 0)
        return;

check_x:
    x = *(volatile u32 *)a;
    if (x == 0)
        v = 0xffffffffu;
    else
        v = *(volatile u32 *)x;

    t = *(volatile u32 *)(base + 8);
    n = *(volatile s32 *)0x00ae2368;
    if ((s32)(n + 16u) <= 0)
        return;

    rec = t + 0xba;
    i = 0;
    do {
        f8 = *(volatile u32 *)(rec + 8);
        if (f8 == v) {
            fc = *(volatile u32 *)(rec + 0xc);
            if (fc == v) {
                f4 = *(volatile u32 *)(rec + 4);
                f0 = *(volatile u32 *)rec;
                sum = f0 + f4;
                *(volatile u32 *)(rec + 0xc) = 0xffffffffu;
                *(volatile u32 *)rec = sum;
                *(volatile u32 *)(rec + 4) = 0;
            }
            x = *(volatile u32 *)a;
            if (x != 0 && *(volatile u8 *)(x + 0x11c) != 0) {
                idx = *(volatile u32 *)(rec - 0x34);
                q = *(volatile u32 *)0x00ae23bc + idx * 132u;
                f0 = *(volatile u32 *)rec;
                cm_fn_005ef330((void *)*(volatile u32 *)0x00b5ded8, q, v, f0);
            }
            *(volatile u32 *)rec = 0;
            *(volatile u32 *)(rec + 8) = 0xffffffffu;
        } else {
            fc = *(volatile u32 *)(rec + 0xc);
            if (fc == v) {
                x = *(volatile u32 *)a;
                if (x != 0 && *(volatile u8 *)(x + 0x11c) != 0) {
                    f4 = *(volatile u32 *)(rec + 4);
                    idx = *(volatile u32 *)(rec - 0x34);
                    q = *(volatile u32 *)0x00ae23bc + idx * 132u;
                    cm_fn_005ef330((void *)*(volatile u32 *)0x00b5ded8, q, v, f4);
                }
                *(volatile u32 *)(rec + 4) = 0;
                *(volatile u32 *)(rec + 0xc) = 0xffffffffu;
            }
        }
        i++;
        rec += 0xd2u;
    } while ((s32)i < (s32)(*(volatile s32 *)0x00ae2368 + 16));
}
