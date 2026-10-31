#include "cmre.h"

/* Preferred-image VA 0x007a8470, image base 0x00400000, primary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Listing: thiscall (ECX is the object, read before written), one stack
   argument, `ret 4`. Same shape as fn_007a82b0 with the fields moved: dword
   +0x3c = base of 16-byte entries, dword +0x40 = entry count n. The entry dword
   at +0 is searched for the stack argument, scanning index 0 .. n-1 in order.
   If found at index i and i is below n-1, entries i+1 .. n-1 are copied down
   one slot (four dwords each). The copy can overwrite the object's own fields,
   so the base and the count are re-read from memory on every pass (volatile
   reads). The last slot then gets dwords 0..2 set to 0xffffffff (dword 3 is
   left alone), and the count is decremented. Nothing changes when n <= 0 or the
   value is absent. */
void CM_THISCALL cm_fn_007a8470(void *self, u32 key)
{
    u8 *obj = (u8 *)self;
    u8 *base;
    u8 *tail;
    volatile u32 *src;
    volatile u32 *dst;
    s32 n;
    s32 i;
    s32 j;
    s32 off;

    n = *(volatile s32 *)(obj + 0x40);
    i = 0;
    if (0 < n) {
        while (*(volatile u32 *)(*(u8 *volatile *)(obj + 0x3c) + i * 16) != key) {
            i = i + 1;
            if (n <= i) {
                return;
            }
        }
        if (i < n - 1) {
            off = i << 4;
            j = i;
            do {
                base = *(u8 *volatile *)(obj + 0x3c);
                src = (volatile u32 *)(base + off + 0x10);
                dst = (volatile u32 *)(base + off);
                j = j + 1;
                off = off + 0x10;
                *dst = *src;
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
            } while (j < *(volatile s32 *)(obj + 0x40) - 1);
        }
        tail = *(u8 *volatile *)(obj + 0x3c) + (*(volatile s32 *)(obj + 0x40) << 4);
        *(u32 *)(tail - 0x10) = 0xffffffff;
        *(u32 *)(tail - 0xc) = 0xffffffff;
        *(u32 *)(tail - 0x8) = 0xffffffff;
        *(volatile s32 *)(obj + 0x40) = *(volatile s32 *)(obj + 0x40) - 1;
    }
}
