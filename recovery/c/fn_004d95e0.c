#include "cmre.h"

/* Stack layout (B = esp after the four pushes, L = B+0x10 is the base of the
   0x138-byte local area; the array below starts at L):
   frame word 0 (B+0x10): result of the index-4 call of 0x00785830
   frame word 1 (B+0x14): result of the index-5 call
   frame word 2 (B+0x18): result of the index-6 call; its low byte is tested
   bytes 0x0c..0x137 (B+0x1c..B+0x147): 300-byte output buffer (callee writes)
   Arguments: arg1 (B+0x14c) is the object for the 0x00785830 calls, arg2
   (B+0x150) is tested against zero, arg3 (B+0x154) is the object for the
   0x004ae660 and 0x004ae8a0 calls. */

extern unsigned long long CM_THISCALL cm_fn_00785830(u32 self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(u32 self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, u32 p1, u32 p2, u32 p3);
extern u32 CM_CDECL cm_fn_0053e140(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5, u32 p6, u32 p7);
extern u32 CM_CDECL cm_fn_0053e990(u32 p1, u32 p2, u32 p3, u32 p4, u32 p5);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

#define CM_DST_ADDR 0x00de1f64u
#define CM_BUF_OFF 0x0cu

void CM_STDCALL cm_fn_004d95e0(u32 arg1, u32 arg2, u32 arg3)
{
    u32 frame[78];
    u32 idx0;
    u32 idx2;
    u32 idx3;
    u32 idx7;
    u32 buf;
    u32 off;
    u32 base;
    u32 n;
    u32 bv;
    u32 ret;
    u32 r4;
    u32 r5;
    u8 al;
    s32 sval;

    idx0 = (u32)cm_fn_00785830(arg1, 0u);
    idx2 = (u32)cm_fn_00785830(arg1, 2u);
    idx3 = (u32)cm_fn_00785830(arg1, 3u);
    frame[0] = (u32)cm_fn_00785830(arg1, 4u);
    frame[1] = (u32)cm_fn_00785830(arg1, 5u);
    frame[2] = (u32)cm_fn_00785830(arg1, 6u);
    idx7 = (u32)cm_fn_00785830(arg1, 7u);
    buf = (u32)&frame[0] + CM_BUF_OFF;

    if (arg2 == 0u) {
        r4 = frame[0];
        r5 = frame[1];
        (void)cm_fn_0053e140(idx3, r4, r5, buf, 0x12cu, 5u, idx2);
        (void)cm_fn_0066f4e0(CM_DST_ADDR, 0x009ad5dcu, buf);
        (void)cm_fn_004ae660(arg3, CM_DST_ADDR);
        bv = CM_GLOBAL(volatile u32, 0x00ae23bc);
        n = idx2 * 110u;
        ret = cm_fn_0053e990(n + bv, idx3, r4, r5, 0u);
        if (ret != 0u) {
            bv = CM_GLOBAL(volatile u32, 0x00ae23bc);
            n = n + bv;
            (void)cm_fn_004ae8a0(arg3, buf, 0x7d7u, n);
        }
        return;
    }

    r4 = frame[0];
    r5 = frame[1];
    (void)cm_fn_0053e140(idx3, r4, r5, buf, 0x12cu, 1u, idx2);
    al = (u8)frame[2];

    if ((al & 1u) != 0u) {
        off = idx0 * 0x245u;
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        sval = (s32)CM_GLOBAL(volatile s8, base + off + 0x37u);
        (void)cm_fn_0066f4e0(CM_DST_ADDR, 0x009ad794u,
                             (u32)sval, (u32)sval, (u32)sval, (u32)sval,
                             buf, base + off + 4u, base + off + 4u);
    }
    else if ((al & 0x14u) != 0u && idx7 != 0u) {
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        off = idx0 * 0x245u;
        sval = (s32)CM_GLOBAL(volatile s8, base + off + 0x37u);
        (void)cm_fn_0066f4e0(CM_DST_ADDR, 0x009ad6b4u,
                             (u32)sval, (u32)sval, buf, base + off + 4u);
    }
    else {
        base = CM_GLOBAL(volatile u32, 0x00ae23b4);
        off = idx0 * 0x245u;
        sval = (s32)CM_GLOBAL(volatile s8, base + off + 0x37u);
        (void)cm_fn_0066f4e0(CM_DST_ADDR, 0x009ad608u,
                             (u32)sval, (u32)sval, buf, base + off + 4u);
    }

    (void)cm_fn_004ae660(arg3, CM_DST_ADDR);
    base = CM_GLOBAL(volatile u32, 0x00ae23b4) + off;
    (void)cm_fn_004ae8a0(arg3, base + 4u, 0x7d5u, base);

    n = idx2 * 110u;
    r5 = frame[1];
    bv = CM_GLOBAL(volatile u32, 0x00ae23bc);
    r4 = frame[0];
    ret = cm_fn_0053e990(n + bv, idx3, r4, r5, 0u);
    if (ret != 0u) {
        bv = CM_GLOBAL(volatile u32, 0x00ae23bc);
        n = n + bv;
        (void)cm_fn_004ae8a0(arg3, buf, 0x7d7u, n);
    }
}
