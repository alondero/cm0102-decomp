/* fn_008cc5b0 - transfer_manager.cpp, preferred-image VA 0x008cc5b0.
   Recovered from the original instruction listing; original callees stay
   explicit boundaries. */

#include "cmre.h"

#define DAT_00ae23b4 CM_GLOBAL(int, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(int, 0x00ae23bc)
#define DAT_00ae2d0c CM_GLOBAL(int, 0x00ae2d0c)
#define DAT_00ae1080 CM_GLOBAL(u32, 0x00ae1080)
#define DAT_00ae1050 CM_GLOBAL(u32, 0x00ae1050)
#define DAT_00b67a34 CM_GLOBAL(u32, 0x00b67a34)

extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 argument);
extern u32 CM_THISCALL cm_fn_004cf5f0(u32 self, volatile u32 person, u32 club);
extern u32 CM_CDECL cm_fn_0053b620(u32 arg);
extern void cm_fn_005e8290(const char *param_1, const char *param_2, int param_3);
extern long double CM_THISCALL cm_fn_00846920(u32 self, u32 param_1, u32 param_2,
                                             u32 param_3, u32 param_4, u32 param_5,
                                             u32 param_6);
extern u32 CM_THISCALL cm_fn_008e8ed0(u32 self);
extern u32 CM_CDECL cm_fn_0090cfc0(u32 bound);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4,
                                   void *arg5);

void CM_THISCALL cm_fn_008cc5b0(u32 self, u32 a1, u8 *out_a, u8 *out_b)
{
    u8 scratch[0x200];              /* parent buffer at +0, second at +0x100 */
    u32 iVar9;
    u32 iVar10;
    u32 ebp;
    u32 r;
    u32 recv;
    u32 saved20;
    u32 idx;
    u32 ptr;
    u32 key;
    u32 e;
    u32 v;
    s32 ebx2;
    s32 q;
    s32 s;
    s8 c;
    u16 count;
    long double rating;
    long double t;

    iVar9 = (u32)DAT_00ae23b4 + (u32)*(s32 *)(a1 + 8) * 0x245u;

    r = cm_fn_004cf5a0((u32)DAT_00ae1050,
                       (u32)DAT_00ae23bc +
                           (u32)*(s32 *)(a1 + 0xc) * 0x6eu);
    DAT_00ae1080 = r;
    if (r == 0) {
        iVar10 = 0;
    }
    else if ((*(u8 *)(r + 0x35) & 0x3f) == 5) {
        iVar10 = 0;
    }
    else {
        iVar10 = (u32)DAT_00ae23b4 + (u32)*(s32 *)(r + 4) * 0x245u;
    }

    ebp = (u32)DAT_00ae23bc + (u32)*(s32 *)(a1 + 0xc) * 0x6eu;
    saved20 = cm_fn_004cf5f0((u32)DAT_00ae1050, ebp, iVar10);

    count = 0x16d;
    key = *(u32 *)(iVar9 + 0x53);
    c = 0;
    idx = self + 0x8ac;
    do {
        ptr = *(volatile u32 *)idx;
        if (*(volatile u32 *)(ptr + 0xa) == key) {
            ptr = *(volatile u32 *)idx;
            if (ptr != 0) {
                count = (u16)cm_fn_008e8ed0(ptr);
            }
            goto search_done;
        }
        c = (s8)(c + 1);
        idx += 4;
    } while (c < 0x1c);

search_done:
    if (*(u32 *)(a1 + 4) != 0) {
        key = *(u32 *)(iVar10 + 0x53);
        c = 0;
        idx = self + 0x8ac;
        do {
            ptr = *(volatile u32 *)idx;
            if (*(volatile u32 *)(ptr + 0xa) == key) {
                ptr = *(volatile u32 *)idx;
                if (ptr != 0) {
                    u32 r1 = cm_fn_008e8ed0(ptr);
                    if ((u16)count >= (u16)r1) {
                        u32 r2 = cm_fn_008e8ed0(ptr);
                        count = (u16)r2;
                    }
                }
                break;
            }
            c = (s8)(c + 1);
            idx += 4;
        } while (c < 0x1c);
    }

    if (count < 3) {
        *out_a = 0;
        *out_b = 1;
        return;
    }
    if (count < 6) {
        *out_a = 1;
        goto random_path_3;
    }
    if (count < 0xb) {
        *out_a = 2;
        goto random_path_3;
    }

    /* 0x008cc744 */
    if (saved20 != 0) {
        c = *(s8 *)(saved20 + 0x3d);
    }
    else {
        c = (s8)0x9c;
    }

    if (cm_fn_0053b620((u32)(u8)*(u8 *)(a1 + 0x11)) != 0) {
        if (*(volatile u32 *)(ebp + 0x61) == 0) {
            cm_fn_00944cff(0x00a8d514u, 0, 0, (void *)(scratch + 0x100),
                           (void *)scratch);
            v = cm_fn_0090d130();
            cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, v,
                           (void *)(scratch + 0x100), (void *)scratch, 0x29c3u);
            cm_fn_005e8290((const char *)0x009870e0u,
                           (const char *)0x00ae24d0u, 0);
            DAT_00b67a34 = 0;
        }
    }

    if (iVar10 != 0) {
        if (cm_fn_0053b620((u32)(u8)*(u8 *)(a1 + 0x11)) != 0) {
            u32 club = *(volatile u32 *)(ebp + 0x61);
            s16 si = *(s16 *)(club + 7);
            if (si > 0x82) {
                q = (s32)*(s16 *)(iVar10 + 0x80) / 0x3c;
                if ((s32)si > q) {
                    if (*(s8 *)(ebp + 0x18) < 0x1a) {
                        u8 nib = *(u8 *)((u32)DAT_00ae2d0c +
                                        (u32)*(u32 *)ebp * 0x4fu + 0xbu);
                        if ((nib & 0xf) < 3) {
                            if (c > 0) {
                                recv = *(u32 *)(self + 0x856);
                                rating = cm_fn_00846920(recv, ebp, iVar9, 0, 0, 1, 0);
                                if (rating >
                                    (long double)CM_GLOBAL(volatile double, 0x968e58)) {
                                    v = (u32)count;
                                    t = (long double)(s32)v *
                                        (long double)CM_GLOBAL(volatile double, 0x968e18);
                                    if (CM_GLOBAL(volatile double, 0x968dd8) < t) {
                                        t = (long double)CM_GLOBAL(volatile double, 0x968dd8);
                                    }
                                    /* original __ftol CALL at 0x00945640 */
                                    c = (s8)(u8)(u32)(long long)t;
                                    goto convert_3;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /* 0x008cc8ac */
    q = (s32)c / 10;
    v = (u32)(q + 0xc);
    e = cm_fn_0090cfc0(v);
    ebx2 = (s32)(s8)(u8)e;          /* 0x008cc8ca movsx ebx, al (first call) */
    v = cm_fn_0090cfc0(3);
    v = v + 1;
    if ((s32)v > ebx2) {
        v = cm_fn_0090cfc0(3);
        c = (s8)(u8)((u8)v + 1);
    }
    else {
        c = (s8)(u8)e;              /* 0x008cc8e8 mov al, bl */
    }

convert_3:
    s = (s32)c;
    *out_a = (u8)(s / 3);
    *out_b = (u8)(s % 3);
    return;

random_path_3:
    /* 0x008cc710: push 3; call 0x0090cfc0; inc al */
    e = cm_fn_0090cfc0(3);
    *out_b = (u8)((u8)e + 1);
    return;
}
