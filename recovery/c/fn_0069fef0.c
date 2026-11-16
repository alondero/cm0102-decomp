/* fn_0069fef0 @ preferred-image VA 0x0069fef0 (image base 0x00400000).
   Reconstructed from the original instruction listing only.  Three stack
   arguments (ret 0xc), no incoming ECX.

   Frame (E = entry esp).  The initial `push 0` is the index-0 argument of the
   first cm_fn_00785830 call, so between calls esp = L = E-0x1b8 (after
   sub esp,0x1a8 and the four saved registers); every reference below is that
   L-relative offset, i.e. L+n == E-0x1b8+n.
     E-0x1a4  dword slot: low byte = cm_fn_00785830(self,4); later the whole
              dword receives cm_fn_0053ad00's EAX result (mov [esp+0x24],eax
              with esp=L-0x10 -> L+0x14)
     E-0x1a5  byte passed as cm_fn_0053ad00's output (lea [esp+0x13] with
               esp=L -> L+0x13); it immediately precedes the dword slot above
               and this routine never initializes it
     E-0x1a0, E-0x19c, E-0x198, E-0x194  dword slots
     E-0x190  0x190-byte buffer handed to cm_fn_0053e140 / cm_fn_0066f4e0
   Arg1 is the getter receiver, arg2 selects the path ([esp+8] at entry) and
   arg3 ([esp+0x1c4] / [esp+0x1dc]) is the receiver of cm_fn_004ae660 and
   cm_fn_004ae8a0.
   The byte at E-0x1a5 stays uninitialized exactly as in the original: it is
   only written by cm_fn_0053ad00, which returns without storing to it when its
   selector (the low byte of the E-0x1a4 slot) is 18 (0053ad00 -> 0053b37e).
   It is carried as a real uninitialized object reached through plain C
   addresses, never via fixed stack offsets and never initialized by hand. */

#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern char *CM_CDECL cm_fn_0053ad00(u8 param_1, s32 param_2, s32 param_3, u8 *param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output,
                                   u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char *CM_CDECL cm_fn_0066f4e0(u32 dest, char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 *CM_CDECL cm_fn_00945a60(u32 *param_1, char *param_2);

#define DAT_00ae23b4 CM_GLOBAL(u32, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(u32, 0x00ae23bc)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)

struct fn_69fef0_frame {
    /* CM0102 3.9.68 SHA-256
       0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
       Preferred-image VAs: 0x006a00d5 addresses L+0x13; the store at
       0x006a00e4 addresses L+0x14..0x17, excluding that output byte.
       0x006a0125 reads L+0x13 after two formatter pushes. The same
       geometry holds at 0x006a018a/0199/01da, 0x006a0243/0252/0293,
       and 0x006a035c/036b/03ac. Keep all objects in one allocation. */
    u8 unused[3];                   /* E-0x1a8..E-0x1a6 */
    u8 output;                      /* E-0x1a5, uninitialized */
    union { u32 d; u8 b[4]; } r4;   /* E-0x1a4 */
    u32 r1;                         /* E-0x1a0 */
    u32 r2;                         /* E-0x19c */
    u32 r3;                         /* E-0x198 */
    u32 r0;                         /* E-0x194 */
    u8 buf[0x190];                  /* E-0x190 */
};

void CM_STDCALL cm_fn_0069fef0(void *arg1, u32 arg2, u32 arg3)
{
    struct fn_69fef0_frame F;
    u32 g0, g1, g2, g3, g5, g6;
    u32 r;
    s32 b, c;
    s16 sel;
    char *p;
    u32 *q;

    if (arg2 == 0) {
        g0 = (u32)cm_fn_00785830(arg1, 0);
        g1 = (u32)cm_fn_00785830(arg1, 1);
        g2 = (u32)cm_fn_00785830(arg1, 2);
        F.r1 = g2;
        g3 = (u32)cm_fn_00785830(arg1, 3);
        F.r4.d = g3;
        g6 = (u32)cm_fn_00785830(arg1, 6);
        cm_fn_0053e140(g1, g2, F.r4.d, (u32)F.buf, 0x190, 5, g0);
        g6 = g6 * 0x245u;
        b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 + 0x52);
        cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a058ac, b, b, (u32)F.buf,
                       DAT_00ae23b4 + g6 + 0x38);
        cm_fn_004ae660((void *)arg3, (u32)0x00de1f64u);
        r = cm_fn_0053e990(g0 * 0x6e + DAT_00ae23bc, g1, F.r1, F.r4.d, 0);
        if (r != 0) {
            cm_fn_004ae8a0((int)arg3, F.buf, 0x7da, g0 * 0x6e + DAT_00ae23bc);
        }
        cm_fn_004ae8a0((int)arg3, (u8 *)(DAT_00ae23b4 + g6 + 4), 0x7d5,
                       DAT_00ae23b4 + g6);
        return;
    }

    g0 = (u32)cm_fn_00785830(arg1, 0);
    F.r0 = g0;
    g1 = (u32)cm_fn_00785830(arg1, 1);
    F.r1 = g1;
    g2 = (u32)cm_fn_00785830(arg1, 2);
    F.r2 = g2;
    g3 = (u32)cm_fn_00785830(arg1, 3);
    F.r3 = g3;
    F.r4.b[0] = (u8)cm_fn_00785830(arg1, 4);
    g5 = (u32)cm_fn_00785830(arg1, 5);
    g6 = (u32)cm_fn_00785830(arg1, 6);
    sel = (s16)(u32)cm_fn_00785830(arg1, 7);

    cm_fn_0053e140(F.r1, F.r2, F.r3, (u32)F.buf, 0x190, 1, F.r0);

    if (sel == 0) {
        if (F.r4.b[0] != 0) {
            if (g5 == g6) {
                p = cm_fn_0053ad00(F.r4.b[0], 0, 0, &F.output);
                b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
                cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a05408, b, b,
                               (s32)(s8)F.output, (s32)(s8)F.output,
                               DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf);
            }
            else {
                p = cm_fn_0053ad00(F.r4.b[0], 0, 0, &F.output);
                F.r4.d = (u32)p;
                c = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
                b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
                cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a0534c, b, b,
                               (s32)(s8)F.output, (s32)(s8)F.output, c, c,
                               DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf,
                               DAT_00ae23b4 + g6 * 0x245u + 4);
            }
        }
        else if (g5 == g6) {
            p = cm_fn_0053ad00(0, 0, 0, &F.output);
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a05408, b, b,
                           (s32)(s8)F.output, (s32)(s8)F.output,
                           DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf);
        }
        else {
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a052d4, b, b, 100, 100,
                           (u32)F.buf, DAT_00ae23b4 + g6 * 0x245u + 4);
        }
    }
    else if (sel == 1) {
        if (F.r4.b[0] != 0) {
            p = cm_fn_0053ad00(F.r4.b[0], 0, 0, &F.output);
            F.r4.d = (u32)p;
            c = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a05548, b, b,
                           (s32)(s8)F.output, (s32)(s8)F.output, c, c,
                           DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
        else {
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a054b8, b, b, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
    }
    else if (sel == 2) {
        if (F.r4.b[0] != 0) {
            p = cm_fn_0053ad00(F.r4.b[0], 0, 0, &F.output);
            F.r4.d = (u32)p;
            c = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a05690, b, b,
                           (s32)(s8)F.output, (s32)(s8)F.output, c, c,
                           DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
        else {
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a0561c, b, b, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
    }
    else {
        if (F.r4.b[0] != 0) {
            p = cm_fn_0053ad00(F.r4.b[0], 0, 0, &F.output);
            F.r4.d = (u32)p;
            c = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g5 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a057d8, b, b,
                           (s32)(s8)F.output, (s32)(s8)F.output, c, c,
                           DAT_00ae23b4 + g5 * 0x245u + 4, (u32)p, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
        else {
            b = (s32)(s8)*(u8 *)(DAT_00ae23b4 + g6 * 0x245u + 0x37);
            cm_fn_0066f4e0((u32)0x00de1f64u, (char *)0x00a0574c, b, b, (u32)F.buf,
                           DAT_00ae23b4 + g6 * 0x245u + 4);
        }
    }

    cm_fn_004ae660((void *)arg3, (u32)0x00de1f64u);
    r = cm_fn_0053e990(DAT_00ae23bc + F.r0 * 0x6e, F.r1, F.r2, F.r3, 0);
    if (r != 0) {
        cm_fn_004ae8a0((int)arg3, F.buf, 0x7da, DAT_00ae23bc + F.r0 * 0x6e);
    }
    q = cm_fn_00945a60(&DAT_00de1f64,
                      (char *)(DAT_00ae23b4 + g5 * 0x245u + 4));
    if (q != 0) {
        cm_fn_004ae8a0((int)arg3, (u8 *)(DAT_00ae23b4 + g5 * 0x245u + 4), 0x7d5,
                       DAT_00ae23b4 + g5 * 0x245u);
    }
    cm_fn_004ae8a0((int)arg3, (u8 *)(DAT_00ae23b4 + g6 * 0x245u + 4), 0x7d5,
                   DAT_00ae23b4 + g6 * 0x245u);
}
