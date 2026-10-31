#include "cmre.h"

extern unsigned long long cm_fn_00945667(u32 arg8, u32 argC);
extern void cm_fn_00945333(u32 param_1, u32 param_2, u32 param_3, void *param_4);

/* No arguments, no ECX read: plain cdecl, `ret 0`.
   Copies each 26-byte row (0x1a) of the table at the pointer in global 0x00ae23d4 whose
   count is in global 0x00ae237c, when the row's link at +4 and the chained pointer at +0x5d
   allow it (see the guards below), into a new buffer. The new buffer comes from the callee
   at 0x00945667 (size = copied rows * 0x1a), and the callee at 0x00945333 gets the copied
   count. Callee 0x00945667 takes two cdecl arguments and returns 64 bits; only EAX is used. */
void cm_fn_00539ae0(void)
{
    s32 count;
    u32 p;
    u32 copied;
    s32 index;
    u32 offset;
    u32 row;
    u32 q;
    u32 r;
    u32 dst;
    u32 newbuf;
    s32 base;
    u32 delta;
    int k;

    copied = 0;
    index = 0;
    offset = 0;
    count = CM_GLOBAL(s32, 0x00ae237c);
    p = CM_GLOBAL(u32, 0x00ae23d4);
    row = p;
    if (count > 0) {
        for (;;) {
            q = *(u32 *)(row + 4);
            if (q != 0) {
                r = *(u32 *)(q + 0x5d);
                if (r == 0 || *(u8 *)(r + 0x11c) != 0) {
                    dst = p + offset;
                    for (k = 0; k < 6; k++) {
                        *(u32 *)(dst + 4 * k) = *(u32 *)(row + 4 * k);
                    }
                    *(u16 *)(dst + 24) = *(u16 *)(row + 24);
                    copied++;
                    p = CM_GLOBAL(u32, 0x00ae23d4);
                    offset += 0x1aU;
                }
            }
            count = CM_GLOBAL(s32, 0x00ae237c);
            index++;
            row += 0x1aU;
            if (index >= count) {
                break;
            }
        }
    }
    newbuf = (u32)cm_fn_00945667(p, copied * 26U);
    base = CM_GLOBAL(s32, 0x00ae2384);
    delta = copied - (u32)base;
    CM_GLOBAL(u32, 0x00ae23d4) = newbuf;
    CM_GLOBAL(u32, 0x00ae237c) = copied;
    CM_GLOBAL(u32, 0x00ae23dc) = newbuf + delta * 26U;
    cm_fn_00945333(newbuf, copied, 0x1aU, (void *)0x00540780);
}
