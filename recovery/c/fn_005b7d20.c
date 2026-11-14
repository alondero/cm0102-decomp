/* candidate-005b7d20.c — plain-C translation of original fn_005b7d20.
 *
 * Original: CM 01/02 3.9.68, preferred-image VA 0x005b7d20 (image base
 * 0x00400000), body 315 bytes, module e:\dev\cm3\cm3 00-01\cm3\code\formation.cpp.
 * SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
 *
 * Physical register contract (coordinator mask-contract.txt):
 *   regparm(2) prefix = incoming EAX, incoming EDX; one stack argument.
 *   Return value carries the full physical EDX:EAX (low 32 = EAX, high 32 = EDX).
 *   This is a physical projection of the original registers, not a claim about
 *   the original source-level 64-bit type.
 *
 * Original stack frame: sub esp,0x204 / push esi / push edi / (push ebx).
 *   scratch = frame base; native ESP+0x0C = scratch+4, ESP+0x10C = scratch+0x104
 *   (non-null path pushes ebx first, so ESP+0x0C/0x0D/0x0E/0x0F become
 *   scratch+0/1/2/3 and hold the cached person bytes 12/15/10/11).
 */

#include "cmre.h"

/* Original boundaries, unchanged. 9cc3dc is the path string, 9870e8 the
 * formatter format string, 9870e0 the message-box title, ae24d0 the message
 * buffer. cm_fn_005e8290 keeps its original code; the unsigned-long-long
 * return is the physical EDX:EAX projection mandated by the contract. */
extern u32 CM_CDECL cm_fn_00944cff(u32 arg1, u32 arg2, u32 arg3, void *arg4, void *arg5);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern char *cm_fn_0090d130(void);
extern unsigned long long CM_CDECL cm_fn_005e8290(const char *title, const char *message, int flags);

unsigned long long CM_CDECL __attribute__((regparm(2)))
cm_fn_005b7d20(u32 incomingEAX, u32 incomingEDX, const u8 *person)
{
    u8 scratch[0x204];            /* one uninitialized scratch, both paths */
    u32 eax = incomingEAX;
    u32 edx = incomingEDX;

    if (person == 0) {
        char *text;
        unsigned long long box;

        cm_fn_00944cff(0x009cc3dc, 0, 0, scratch + 0x104, scratch + 0x04);

        text = cm_fn_0090d130();

        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text,
                       scratch + 0x104, scratch + 0x04, 0xeb4);

        box = cm_fn_005e8290((const char *)0x009870e0, (const char *)0x00ae24d0, 0);

        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;

        eax = ((u32)box) & 0xffff0000u;   /* xor ax,ax: only low AX cleared */
        edx = (u32)(box >> 32);           /* EDX untouched by the original */
    } else {
        const volatile u8 *p = person;
        u32 si = 0;
        u32 edi = 0;
        s8 threshold;
        u8 v_11, v_10, v_0f, v_12, v_15;
        u8 v_17, v_18, v_19;

        /* original load order 11, 10, 0F, 12, 15; cached in scratch+3,+2,AL,+0,+1 */
        v_11 = p[0x11]; scratch[0x03] = v_11;
        v_10 = p[0x10]; scratch[0x02] = v_10;
        v_0f = p[0x0f];
        v_12 = p[0x12]; scratch[0x00] = v_12;
        v_15 = p[0x15]; scratch[0x01] = v_15;

        threshold = 15;                   /* DL = 0x0F */

        for (;;) {
            if ((s8)v_0f          >= (s8)threshold) si |= 0x001u;
            if ((s8)scratch[0x03] >= (s8)threshold) si |= 0x004u;
            if ((s8)scratch[0x02] >= (s8)threshold) si |= 0x002u;
            if ((s8)scratch[0x00] >= (s8)threshold) si |= 0x008u;
            else if ((s8)p[0x14]   >= (s8)threshold) si |= 0x020u;
            else if ((s8)p[0x13]   >= (s8)threshold) si |= 0x010u;
            if ((s8)scratch[0x01] >= (s8)threshold) si |= 0x040u;

            threshold = (s8)(threshold - 1);   /* dec dl */
            if (threshold < 10) break;         /* cmp dl,0xa / jl */
            if (si != 0) break;                /* test si,si / je loop */
        }

        /* original reads 17, 18, 19 exactly once, in this order */
        v_17 = p[0x17];
        v_18 = p[0x18];
        v_19 = p[0x19];
        edx = (edx & 0xffffff00u) | (u32)v_17;   /* DL = person+0x17 */

        {
            s8 th2 = 15;                     /* AL = 0x0F */

            for (;;) {
                if ((s8)v_17 >= 15) { si |= 0x800u; edi = 1u; }
                if ((s8)v_18 >= 15) { si |= 0x080u; edi = 1u; }
                if ((s8)v_19 >= 15) { si |= 0x200u; edi = 1u; }

                th2 = (s8)(th2 - 1);          /* dec al */
                if (th2 < 10) break;          /* cmp al,0xa / jl */
                if (edi != 0) break;          /* test edi,edi / je loop */
            }
        }

        eax = (eax & 0xffff0000u) | (si & 0xffffu);   /* mov ax,si */
    }

    return ((unsigned long long)edx << 32) | (unsigned long long)eax;
}
