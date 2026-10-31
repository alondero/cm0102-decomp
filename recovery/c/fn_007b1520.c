#include "cmre.h"

/* Thiscall with one stack argument (ret 4). ECX is the object; the stack
   argument (entry ESP+4) is read as arg1 and tested against zero.
   Object fields by byte offset from the object pointer:
     +0x0a u8 mode (compared with 3), +0x1ee u32 record array base,
     +0x1f6 s32 record count.
   Record size 0x2d bytes; fields by offset within one record:
     +0x00/+0x04/+0x08 u32, +0x0c s32 index (the element index written back
     as the loop index k), +0x10 and +0x18 8-byte blocks used as copy targets
     of 0x00417c10, +0x21 and +0x25 unaligned u32, +0x20/+0x29/+0x2a/+0x2b/+0x2c
     single bytes.
   Element table: 0x00ae23bc u32 base, 0x00ae2364 s32 element count, element
   stride 0x6e (110) bytes; 0x00ae2c90 is passed by address to 0x00417c10;
   0x00b7434c is read before 0x007a7eb0 and its value is discarded (ECX is not
   an argument to that stdcall callee).
   Frame: scratch is one 0x18-byte area holding three 8-byte copy buffers at
   frame offsets +0x00, +0x08 and +0x10 (listing frame L+0x0c, L+0x14, L+0x1c,
   where L is the ESP after the 0x24-byte reservation). */

#define OBJ_U32(off) (*(volatile u32 *)(self + (off)))
#define OBJ_U8(off)  (*(volatile u8 *)(self + (off)))
#define GLOBAL_COUNT CM_GLOBAL(volatile s32, 0x00ae2364)
#define GLOBAL_BASE  CM_GLOBAL(volatile u32, 0x00ae23bc)

extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern u32 cm_fn_0053e990(int param_1, int param_2, int param_3, int param_4, int param_5);
extern u8 CM_STDCALL cm_fn_007a7eb0(s32 *param_1);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void CM_CDECL cm_fn_00945333(u32 *param_1, u32 *param_2, u32 param_3, void *param_4);

u32 CM_THISCALL cm_fn_007b1520(u32 self, u32 arg1)
{
    u8 scratch[0x18];
    s32 n;
    s32 i;
    s32 m;
    s32 k;
    s32 idx;
    u32 off;
    u32 rec;
    u32 base;
    u32 cur;
    u32 last;
    u32 a;
    u32 flag;
    u32 res;
    u32 v0;
    u32 v4;
    u32 v8;
    u32 v12;
    u32 v21;
    u32 v25;
    u32 v1;
    u32 v2;
    u8 mode;
    u8 b3;
    u8 b20;
    u8 b29;
    u8 b2a;
    u8 b2b;
    u8 b2c;
    u8 res8;

    n = (s32)OBJ_U32(0x1f6);
    if (n == 0) {
        return 1;
    }
    if (n < 0) {
        goto cleanup;
    }

    i = 0;
    off = 0;
    base = GLOBAL_BASE;
    do {
        rec = OBJ_U32(0x1ee) + off;
        if (arg1 != 0) {
            cm_fn_00417c10((u32 *)(rec + 0x18), (u32 *)&scratch[0], (u16 *)0x00ae2c90u);
            base = GLOBAL_BASE;
        }

        idx = (s32)*(volatile u32 *)(rec + 0x0c);
        if (idx < 0) {
            goto second;
        }
        m = GLOBAL_COUNT;
        if (idx >= (s32)((u32)m - 0x10u)) {
            goto second;
        }
        cur = base + (u32)idx * 110u;
        if (cur == 0) {
            goto second;
        }
        v8 = *(volatile u32 *)(rec + 0x08);
        v4 = *(volatile u32 *)(rec + 0x04);
        v0 = *(volatile u32 *)(rec + 0x00);
        res = cm_fn_0053e990((int)cur, (int)v0, (int)v4, (int)v8, (int)(rec + 0x10));
        if (res == 0) {
            goto l_15f5;
        }

        mode = OBJ_U8(0x0a);
        if (mode == 3) {
            v2 = *(volatile u32 *)(cur + 0x69);
            if (v2 == 0) {
                goto l_15f5;
            }
            b3 = *(volatile u8 *)(cur + 0x3d);
            if (b3 == 1 || b3 == 2 || b3 == 3) {
                goto l_15f5;
            }
        } else {
            v1 = *(volatile u32 *)(cur + 0x61);
            if (v1 == 0) {
                goto l_15f5;
            }
        }
        (void)CM_GLOBAL(volatile u32, 0x00b7434c);
        res8 = cm_fn_007a7eb0((s32 *)cur);
        if (res8 != 2) {
            goto l_1753;
        }

    l_15f5:
        base = GLOBAL_BASE;

    second:
        flag = 0;
        cur = 0;
        if (arg1 == 0) {
            goto l_16b8;
        }
        m = GLOBAL_COUNT;
        k = 0;
        if (m <= 0) {
            goto l_16b8;
        }
        do {
            if (flag != 0) {
                goto l_1759;
            }
            v8 = *(volatile u32 *)(rec + 0x08);
            v4 = *(volatile u32 *)(rec + 0x04);
            v0 = *(volatile u32 *)(rec + 0x00);
            res = cm_fn_0053e990((int)(base + cur), (int)v0, (int)v4, (int)v8, (int)(rec + 0x10));
            if (res == 0) {
                goto l_1695;
            }
            mode = OBJ_U8(0x0a);
            base = GLOBAL_BASE;
            if (mode != 3) {
                v1 = *(volatile u32 *)(base + cur + 0x61);
                if (v1 == 0) {
                    goto l_169b;
                }
            }
            if (mode == 3) {
                v2 = *(volatile u32 *)(base + cur + 0x69);
                if (v2 == 0) {
                    goto l_169b;
                }
                b3 = *(volatile u8 *)(base + cur + 0x3d);
                if (b3 == 1 || b3 == 2 || b3 == 3) {
                    goto l_169b;
                }
            }
            (void)CM_GLOBAL(volatile u32, 0x00b7434c);
            res8 = cm_fn_007a7eb0((s32 *)(base + cur));
            if (res8 == 2) {
                goto l_1695;
            }
            *(volatile u32 *)(rec + 0x0c) = (u32)k;
            flag = 1;
        l_1695:
            base = GLOBAL_BASE;
        l_169b:
            m = GLOBAL_COUNT;
            k = k + 1;
            cur = cur + 0x6e;
        } while (k < m);
        if (flag != 0) {
            goto l_1759;
        }

    l_16b8:
        n = (s32)OBJ_U32(0x1f6);
        a = OBJ_U32(0x1ee);
        last = a + (u32)n * 45u - 45u;
        v0 = *(volatile u32 *)(last + 0x00);
        *(volatile u32 *)(rec + 0x00) = v0;
        v4 = *(volatile u32 *)(last + 0x04);
        *(volatile u32 *)(rec + 0x04) = v4;
        v8 = *(volatile u32 *)(last + 0x08);
        *(volatile u32 *)(rec + 0x08) = v8;
        v12 = *(volatile u32 *)(last + 0x0c);
        *(volatile u32 *)(rec + 0x0c) = v12;
        cm_fn_00417c10((u32 *)(rec + 0x10), (u32 *)&scratch[8], (u16 *)(last + 0x10));
        cm_fn_00417c10((u32 *)(rec + 0x18), (u32 *)&scratch[0x10], (u16 *)(last + 0x18));
        b20 = *(volatile u8 *)(last + 0x20);
        *(volatile u8 *)(rec + 0x20) = b20;
        v21 = *(volatile u32 *)(last + 0x21);
        *(volatile u32 *)(rec + 0x21) = v21;
        v25 = *(volatile u32 *)(last + 0x25);
        *(volatile u32 *)(rec + 0x25) = v25;
        b29 = *(volatile u8 *)(last + 0x29);
        *(volatile u8 *)(rec + 0x29) = b29;
        b2a = *(volatile u8 *)(last + 0x2a);
        *(volatile u8 *)(rec + 0x2a) = b2a;
        b2b = *(volatile u8 *)(last + 0x2b);
        *(volatile u8 *)(rec + 0x2b) = b2b;
        b2c = *(volatile u8 *)(last + 0x2c);
        *(volatile u8 *)(rec + 0x2c) = b2c;
        n = (s32)OBJ_U32(0x1f6);
        OBJ_U32(0x1f6) = (u32)(n - 1);
        i = i - 1;
        off = off - 0x2d;

    l_1753:
        base = GLOBAL_BASE;

    l_1759:
        i = i + 1;
        off = off + 0x2d;
        n = (s32)OBJ_U32(0x1f6);
    } while (i < n);

cleanup:
    n = (s32)OBJ_U32(0x1f6);
    if (n != 0) {
        cm_fn_00945333((u32 *)OBJ_U32(0x1ee), (u32 *)(u32)n, 0x2d, (void *)0x007bce50u);
        return 1;
    }
    cm_fn_009452ca(OBJ_U32(0x1ee));
    OBJ_U32(0x1ee) = 0;
    OBJ_U32(0x1f2) = 0;
    return 1;
}
