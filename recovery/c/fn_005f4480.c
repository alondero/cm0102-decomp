/* Translation of preferred-image VA 0x005f4480 (comp/cups/hol_cup.cpp).
   Pinned binary: CM0102 3.9.68 SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   All VAs below are preferred-image VAs (image base 0x00400000). */
#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

extern u32 CM_THISCALL cm_fn_0051c410(u32 self, u32 index);
extern u32 CM_THISCALL cm_fn_00522e00(u32 self, u32 owner, s16 count, u32 clubs, s16 rounds, u32 competition, u32 schedule, u16 year, u8 flag_a, u8 flag_b, u16 round_flag, u8 flag_c, u32 builder, u32 numbered, u32 value_38, u32 club_flags);
extern void cm_fn_005e8290(const char *p1, const char *p2, int p3);
extern u32 CM_THISCALL cm_fn_006826d0(void *self, u32 arg);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
extern char * cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 dest, u32 fmt, ...);
extern unsigned long long CM_CDECL cm_fn_00944cf1(u32 size);
extern u32 CM_CDECL cm_fn_00944cff(u32 a1, u32 a2, u32 a3, void *a4, void *a5);
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg);

#define G_ERRBUF CM_GLOBAL(u8, 0x00ae24d0)
#define G_FLAG   CM_GLOBAL(u32, 0x00b67a34)

typedef u32 (CM_THISCALL *cm_vt_fn)(u32 self, u32 a1, u32 a2, u32 a3, u32 a4);

void CM_FASTCALL cm_fn_005f4480(u32 *this_ptr)
{
    /* One allocation keeps the original adjacency: scratch S+0..S+0x418, then
       the linked SEH frame prev/handler/state at S+0x418 (P-0x0c..P-0x04).
       005f4486..005f448e: state=-1, handler=0x0095acfe, prev=old fs:[0]. */
    struct {
        u32 w424;               /* S+0x00 write pointer into the 0xb8 buffer */
        u32 w420;               /* S+0x04 second 0xb8 allocation */
        u32 w41c;               /* S+0x08 loop counter */
        u32 w418;               /* S+0x0c element count */
        u32 w414;               /* S+0x10 pointer passed to indirect call */
        u32 w410;               /* S+0x14 value passed to 0x00522e00 */
        u8  a[256];             /* S+0x18 */
        u8  b[256];             /* S+0x118 */
        u8  c[256];             /* S+0x218 */
        u8  d[256];             /* S+0x318 */
        void *prev;             /* S+0x418 = P-0x0c */
        void *handler;          /* S+0x41c = P-0x08 */
        volatile u32 state;     /* S+0x420 = P-0x04 */
    } f;
    u32 *p1, *p2, *wp, *wp2, *recv;
    u32 tmp, idx, off, n, tab, i, vt, slot;
    u32 rnd_a, rnd_b, coin, write_val;
    u32 *zero_a, *zero_b;
    s16 a_idx, b_idx;
    s8 r;
    u32 ret_val, vt_result, alloc_result;
    cm_vt_fn vt_fn;

    f.prev = CM_FS(void *, 0);
    f.handler = (void *)0x0095acfe;
    f.state = 0xffffffffu;
    CM_FS(void *, 0) = &f.prev;

    /* 005f44a0 */
    p1 = (u32 *)(u32)(cm_fn_00944e46(0xb8));
    off = 0;
    if (p1 == NULL) {
        /* 005f44b5: buffer A first, then buffer B */
        cm_fn_00944cff((u32)0x009cedd4, 0, 0, (void *)f.a, (void *)f.b);
        tmp = (u32)cm_fn_0090d130();
        cm_fn_00944c9f((u32)&G_ERRBUF, (u32)0x009870e8, tmp, (u32)f.a, (u32)f.b, 0x2b9);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_ERRBUF, 0);
        G_FLAG = 0;
        CM_FS(void *, 0) = f.prev;
        return;
    }

    /* 005f44e5 */
    p2 = (u32 *)(u32)(cm_fn_00944e46(0xb8));
    f.w420 = (u32)p2;
    if (p2 == NULL) {
        /* 005f44fa: buffer B first, then buffer A */
        cm_fn_00944cff((u32)0x009cedd4, 0, 0, (void *)f.b, (void *)f.a);
        tmp = (u32)cm_fn_0090d130();
        cm_fn_00944c9f((u32)&G_ERRBUF, (u32)0x009870e8, tmp, (u32)f.b, (u32)f.a, 0x2c1);
        cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_ERRBUF, 0);
        G_FLAG = 0;
        CM_FS(void *, 0) = f.prev;
        return;
    }

    /* 005f455b */
    {
        u32 *src = (u32 *)(u32)(*(u32 *)((u8 *)this_ptr + 0xb1));
        u32 base;

        *p1 = *src;
        wp = p1 + 1;
        do {
            base = *(volatile u32 *)((u8 *)this_ptr + 0xc);
            recv = (u32 *)((u8 *)base + off);
            tmp = (u32)cm_fn_006826d0((void *)*recv, 0);
            off += 4;
            *wp = *(u32 *)tmp;
            wp++;
        } while (off < 0x4c);

        /* 005f4585 */
        f.w424 = (u32)(p1 + 0x15);
        idx = 0x15;
        off = 0;
        p1[0x14] = *(u32 *)((u8 *)(*(u32 *)((u8 *)this_ptr + 0xb1)) + 0x3b);
        do {
            base = *(volatile u32 *)((u8 *)this_ptr + 0xc);
            recv = (u32 *)((u8 *)base + off);
            tmp = *recv;
            if (*(u16 *)((u8 *)tmp + 0x3e) == 4) {
                tmp = (u32)cm_fn_006826d0((void *)tmp, 1);
                idx++;
                *(u32 *)f.w424 = *(u32 *)tmp;
                f.w424 += 4;
            }
            off += 4;
        } while (off < 0x4c);

        /* 005f45cb */
        r = (s8)(u8)cm_fn_0090cfc0(0x14);
        tmp = p1[r];
        wp = p1 + r;
        p1[idx] = tmp;
        n = *(u32 *)((u8 *)this_ptr + 0xf2);
        tab = *(u32 *)((u8 *)this_ptr + 0xee);
        idx++;
        f.w41c = 0x14;
        *wp = *(u32 *)((u8 *)tab + n * 4 - 0x1c);
        f.w418 = idx;

        /* 005f4600: reset write pointer to the second allocation */
        f.w424 = f.w420;
        do {
            /* 005f4608 */
            rnd_a = cm_fn_0090cfc0(0x14);
            rnd_b = cm_fn_0090cfc0(0x14) + 0x14;
            a_idx = (s16)(u16)rnd_a;
            b_idx = (s16)(u16)rnd_b;

            if (p1[a_idx] == 0) {
                do {
                    a_idx++;
                    if (a_idx > 0x13) a_idx = 0;
                } while (p1[a_idx] == 0);
            }
            if (p1[b_idx] == 0) {
                do {
                    b_idx++;
                    if (b_idx > 0x27) b_idx = 0x14;
                } while (p1[b_idx] == 0);
            }

            coin = cm_fn_0090cfc0(2);
            zero_a = p1 + a_idx;
            zero_b = p1 + b_idx;
            if (coin != 0) {
                *(u32 *)f.w424 = *zero_a;
                write_val = *zero_b;
            } else {
                *(u32 *)f.w424 = *zero_b;
                write_val = *zero_a;
            }
            *((u32 *)f.w424 + 1) = write_val;
            *zero_b = 0;
            *zero_a = 0;
            f.w424 += 8;
            f.w41c--;
        } while (f.w41c != 0);

        /* 005f46c3: copy the last six table entries */
        n = *(u32 *)((u8 *)this_ptr + 0xf2);
        wp2 = (u32 *)(f.w420 + f.w418 * 4);
        i = n - 6;
        if ((s32)i < (s32)n) {
            do {
                *wp2 = *(u32 *)((u8 *)(*(u32 *)((u8 *)this_ptr + 0xee)) + i * 4);
                i++;
                wp2++;
            } while ((s32)i < (s32)*(volatile u32 *)((u8 *)this_ptr + 0xf2));
        }

        /* 005f46f5: indirect call through vtable slot 0x3c */
        vt = *(u32 *)(u32)this_ptr;
        slot = *(u32 *)(vt + 0x3c);
        vt_fn = (cm_vt_fn)slot;
        /* 005f46f8/005f46fe: lea ecx,[esp+0x20], lea edx,[esp+0x28] - the
           vtable call receives addresses of the two stack slots. */
        vt_result = vt_fn((u32)this_ptr, 0x13, (u32)&f.w410, (u32)&f.w414, 0);

        alloc_result = (u32)(cm_fn_00944cf1(0xb2));
        f.state = 0;
        if (alloc_result == 0) {
            ret_val = 0;
        } else {
            ret_val = cm_fn_00522e00(alloc_result, (u32)this_ptr, 0x2e,
                                    f.w420, (s16)f.w410,
                                    *(u32 *)((u8 *)this_ptr + 4), vt_result,
                                    (u16)(*(u16 *)((u8 *)this_ptr + 0x40)),
                                    0x13, 1, (u16)f.w414, 0x14, 0, 0, 0, 0);
        }
        f.state = 0xffffffffu;

        *(u32 *)((u8 *)(*(u32 *)((u8 *)this_ptr + 0xc)) + 0x4c) = ret_val;
        cm_fn_009452ca(f.w420);
        cm_fn_009452ca((u32)p1);
        cm_fn_009452ca(vt_result);

        base = *(volatile u32 *)((u8 *)this_ptr + 0xc);
        ret_val = *(volatile u32 *)(base + 0x4c);
        if (ret_val == 0) {
            cm_fn_00944cff((u32)0x009cedd4, 0, 0, (void *)f.d, (void *)f.c);
            tmp = (u32)cm_fn_0090d130();
            cm_fn_00944c9f((u32)&G_ERRBUF, (u32)0x009870e8, tmp, (u32)f.d, (u32)f.c, 0x30e);
            cm_fn_005e8290((const char *)0x009870e0, (const char *)&G_ERRBUF, 0);
            G_FLAG = 0;
            CM_FS(void *, 0) = f.prev;
            return;
        }
        cm_fn_0051c410(ret_val, 0);
        CM_FS(void *, 0) = f.prev;
    }
}
