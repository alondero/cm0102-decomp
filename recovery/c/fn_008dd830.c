/* fn_008dd830 - preferred-image VA 0x008dd830, image base 0x00400000.
   Translated from the original listing carried in
   CM0102/Analysis/cmre/farm/20261115T1250Z-sol-commandcode-supervised/bundle/fn_008dd830.md
   (CM 01/02 3.9.68, cm0102.exe SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86).

   ABI read from the listing only:
   - `ret 0xc` pops 12 bytes, i.e. the callee removes its own three stack
     arguments, so the entry is stdcall with three arguments:
       arg1 = [S+0x04] (mov ebx,[esp+0x140] at 0x008dd837) - the receiver used
              for the cm_fn_00785830 index lookups,
       arg2 = [S+0x08] (mov ecx,[esp+0x150] at 0x008dd878) - branch selector,
       arg3 = [S+0x0c] (mov ecx,[esp+0x188] at 0x008dd907,
              [esp+0x170] at 0x008da04, [esp+0x154]/[esp+0x158] at
              0x008dd96b/0x008dd955/0x008da4e/0x008da66) - the ECX receiver of
              cm_fn_004ae660 and cm_fn_004ae8a0.
     The function returns the EAX of the last cm_fn_004ae8a0 call (u32).
   - Callee stack arguments, read from the pushes and the single `add esp`
     after each call:
       cm_fn_00785830: 1 stack arg, callee pops 4 (ret 4).
       cm_fn_0053e140:  7 stack args, caller pops 0x1c (cdecl).
       cm_fn_0053e990:  5 stack args, caller pops 0x14 (cdecl).
       cm_fn_004ae660:  1 stack arg, callee pops 4 (ret 4).
       cm_fn_004ae8a0:  3 stack args, callee pops 12 (ret 0xc): the epilogue
              pops at 0x008dd9a8..0x008dd9ab land exactly on the saved-register
              block, so the callee must have removed 12 bytes.
       cm_fn_0066f4e0:  13 stack args on the main path (caller pops 0x34 at
              0x008dd90e) and 7 on the other path (caller pops 0x1c at
              0x008da0b) - cdecl, both shapes come from the listing.
   - Only EAX of each cm_fn_00785830 result is consumed (mov esi/edi/ebp,eax),
     so the 64-bit result is narrowed exactly as the original does.

   Frame built by `sub esp,0x138` (base L), as recovered from the listing:
     L+0x00 dword   index-5 result (mov [esp+0x10],eax at 0x008dd87f)
     L+0x04 dword   index-4 result (mov [esp+0x18],eax at 0x008dd86f)
     L+0x08 dword   index-1 result * 0x245 (mov [esp+0x18],esi at 0x008dd8d3)
     L+0x0c 300     scratch buffer (lea eax,[esp+0x24] at 0x008dd8a1 and
                    lea edx,[esp+0x1c] at 0x008dd94e; 0x12c bytes end the
                    frame exactly at L+0x138).

   Acceptance comes only from `cmre test --c` and `cmre accept --c`. */
#include "cmre.h"

#define DAT_00ae23b4 CM_GLOBAL(volatile int, 0x00ae23b4)
#define DAT_00ae23bc CM_GLOBAL(volatile int, 0x00ae23bc)
#define DAT_00de1f64 CM_GLOBAL(u32, 0x00de1f64)
#define STR_00a98e14 ((char *)0x00a98e14)
#define STR_00a98e74 ((char *)0x00a98e74)

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* legacy callee; the original pushes 13 dwords on the main path (caller pops
   0x34 at 0x008dd90e) and 7 on the other one (caller pops 0x1c at
   0x008da0b), so the prototype has to carry both shapes: a variadic cdecl
   declaration keeps every argument on the stack and lets the caller pop
   exactly what the original pops. */
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

typedef struct cm_frame_s {
    u32 v00;      /* L+0x00 */
    u32 v04;      /* L+0x04 */
    u32 v08;      /* L+0x08 */
    u8  buf[300]; /* L+0x0c */
} cm_frame_s;

u32 CM_STDCALL cm_fn_008dd830(u32 param_1, u32 param_2, u32 param_3)
{
    cm_frame_s fr;
    u8 *buf = fr.buf;
    u32 r0, r1, r2, r3, r4, r5;
    u32 rec1, rec0;
    u32 b_rec1, b_rec0;
    u32 base;
    u32 person;
    u32 res;

    /* 0x008dd841 - 0x008dd87f: five index lookups; only EAX of each result
       is kept by the original (mov esi/edi/ebp,eax). */
    r1 = (u32)cm_fn_00785830((void *)param_1, 1);
    r2 = (u32)cm_fn_00785830((void *)param_1, 2);
    r3 = (u32)cm_fn_00785830((void *)param_1, 3);
    r4 = (u32)cm_fn_00785830((void *)param_1, 4);
    r5 = (u32)cm_fn_00785830((void *)param_1, 5);
    fr.v04 = r4; /* 0x008dd86f */
    fr.v00 = r5; /* 0x008dd87f */

    if (param_2 != 0) { /* 0x008dd883 (je 0x008dd9b5 when zero) */
        r0 = (u32)cm_fn_00785830((void *)param_1, 0); /* 0x008dd88f */

        /* 0x008dd8ae: edi=r2, 1, 0x12c, buffer, r5, r4, r3 pushed */
        cm_fn_0053e140(r3, r4, r5, (u32)buf, 0x12c, 1, r2);
        base = (u32)DAT_00ae23b4;

        /* 0x008dd8b3 - 0x008dd8c4: esi = esi + (esi*9 shl 4 + esi)*4 */
        rec1 = r1 * 0x245u;
        /* 0x008dd8c7 - 0x008dd8d8: ebx = ebx + (ebx*9 shl 4 + ebx)*4 */
        rec0 = r0 * 0x245u;
        fr.v08 = rec1; /* 0x008dd8d3 */

        /* 0x008dd8dc / 0x008dd8e1: movsx from the two records */
        b_rec1 = (u32)(s32)*(volatile s8 *)(rec1 + base + 0x52);
        b_rec0 = (u32)(s32)*(volatile s8 *)(rec0 + base + 0x52);

        /* 0x008dd902: 13 pushed dwords, caller pops 0x34 */
        cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, STR_00a98e74, 0x64,
                       (int)b_rec0, (int)b_rec0,
                       (int)b_rec1, (int)b_rec1, (int)b_rec1, (int)b_rec1,
                       (u32)buf,
                       rec0 + base + 4,
                       rec1 + base + 4,
                       rec1 + base + 4);

        /* 0x008dd916: ecx = [esp+0x188] = arg3 */
        cm_fn_004ae660((void *)param_3, 0x00de1f64);

        /* 0x008dd91b - 0x008dd93c: esi = edi*110, person = esi + [0xae23bc] */
        person = r2 * 0x6eu + (u32)DAT_00ae23bc;
        if (cm_fn_0053e990(person, r3, r4, r5, 0) != 0) { /* 0x008dd93c */
            /* 0x008dd948: the global is re-read after the call and the
               refreshed value is the one pushed at 0x008dd954. */
            person = r2 * 0x6eu + (u32)DAT_00ae23bc;
            /* 0x008dd964: buffer, 0x7d7, person */
            cm_fn_004ae8a0(param_3, (const char *)buf, 0x7d7, person);
        }

        /* 0x008dd98b: rec1+[0xae23b4]+0x38, 0x7d5, rec1+[0xae23b4] */
        base = (u32)DAT_00ae23b4 + rec1;
        cm_fn_004ae8a0(param_3,
                       (const char *)(base + 0x38),
                       0x7d5,
                       base);

        /* 0x008dd9a3: rec0+[0xae23b4]+0x38, 0x7d5, rec0+[0xae23b4] */
        base = (u32)DAT_00ae23b4 + rec0;
        res = cm_fn_004ae8a0(param_3,
                             (const char *)(base + 0x38),
                             0x7d5,
                             base);
    } else {
        /* 0x008dd9b5: ebx = [esp+0x14] = index-4 result */
        /* 0x008dd9c9: ebp=r3, ebx=r4, eax=r5, buffer, 0x12c, 5, edi=r2 */
        cm_fn_0053e140(r3, r4, r5, (u32)buf, 0x12c, 5, r2);
        base = (u32)DAT_00ae23b4;

        /* 0x008dd9d4 - 0x008dd9df */
        rec1 = r1 * 0x245u;
        /* 0x008dd9e6 */
        b_rec1 = (u32)(s32)*(volatile s8 *)(rec1 + base + 0x52);

        /* 0x008dd9ff: 7 pushed dwords, caller pops 0x1c */
        cm_fn_0066f4e0((int)(u32)&DAT_00de1f64, STR_00a98e14, 0x64,
                       (int)b_rec1, (int)b_rec1,
                       (u32)buf,
                       rec1 + base + 0x38);

        /* 0x008da13: ecx = [esp+0x170] = arg3 */
        cm_fn_004ae660((void *)param_3, 0x00de1f64);

        /* 0x008da18 - 0x008da36 */
        person = r2 * 0x6eu + (u32)DAT_00ae23bc;
        if (cm_fn_0053e990(person, r3, r4, r5, 0) != 0) { /* 0x008da36 */
            /* 0x008da42: the global is re-read after the call and the
               refreshed value is the one pushed at 0x008da55. */
            person = r2 * 0x6eu + (u32)DAT_00ae23bc;
            /* 0x008da5c: buffer, 0x7d7, person */
            cm_fn_004ae8a0(param_3, (const char *)buf, 0x7d7, person);
        }

        /* 0x008da79: rec1+[0xae23b4]+0x38, 0x7d5, rec1+[0xae23b4] */
        base = (u32)DAT_00ae23b4 + rec1;
        res = cm_fn_004ae8a0(param_3,
                             (const char *)(base + 0x38),
                             0x7d5,
                             base);
    }

    return res;
}
