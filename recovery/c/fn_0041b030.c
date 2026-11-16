/* Reconstruction of preferred-image VA 0x0041b030 (module: unknown).
   Pinned CM0102 3.9.68 binary SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1c627a22c445d14d489bd9bff86.

   ABI, from the original instructions only:
   - 0x0041b1a0 / 0x0041b3c4 `ret 0xc` pop 12 bytes of stack arguments.
   - 0x0041b030..0x0041b035 (`mov eax,0x1b74` / `call 0x945610`) followed by
     `add esp,0x1b74` in both epilogues (0x0041b19a, 0x0041b3be) is MSVC's
      frame-allocation boundary. The supplied caller listing does not show the
      helper body and cannot establish its exact probing or return-word accesses.
      It does establish that the helper executes before the saved-register
      pushes at 0x0041b03a..0x0041b03c and the first body call at 0x0041b049.
      A plain C local below represents storage only, NOT equivalent compiler
      probing, return-word transport or fault ordering. Audit 1747 remains held:
      a passing semantic gate does not discharge this boundary. Resolving it
      requires original-helper instruction evidence and verified prologue
      transport support outside this source-only plain-C repair scope; adding
      an ordinary C call to the helper would not establish that transport.
      This source is not acceptance evidence for the prologue.
      The three stack words at
     0x0041b03d [esp+0x1b84], 0x0041b0bf [esp+0x1b8c] and 0x0041b136
     [esp+0x1b90] are this function's real stack arguments
     ([entry_esp+4], [+8], [+0xc]); `in_stack_00001b7c` is arg2, not a global.
    - ECX is never read before being written; ret 0xc therefore gives stdcall
      with three stack arguments, not an invented thiscall receiver.
   - Frame base B = entry_esp - 0x1b74.  After `push ebx/ebp/esi` and the ten
     `push k` / `call 0x785830` pairs (callee pops its argument), [esp+0x18],
     [esp+0x20], [esp+0x1c], [esp+0x14], [esp+0x28], [esp+0x24] name
      B+4, B+0xc, B+8, B+0, B+0x14, B+0x10, including four saved
      registers and the live argument push. 0x0041b0c8 writes B+0x18.
    - Only EAX from 0x785830 is consumed here; its wider return declaration
      is retained as an explicit boundary, not independently proved here. */
#include "cmre.h"

extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(void *self, u8 *param_2, u32 param_3, u32 param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
/* 0x66f4e0 receives 3, 7 or 8 pushed arguments at different sites
   (0x0041b112 `add esp,0x10`, 0x0041b12e `add esp,0xc`,
   0x0041b233 `add esp,0x20`, 0x0041b2e2 `add esp,0x1c`), so the trailing
   arguments are variadic. */
extern char * CM_CDECL cm_fn_0066f4e0(char *dest, const char *fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);

void CM_STDCALL cm_fn_0041b030(u32 arg1, u32 arg2, u32 arg3)
{
    u8 frame[0x1b74];
    u32 v0;
    u32 v1;
    u32 v6;
    u32 v9;
    u32 g;

    v0 = (u32)cm_fn_00785830((void *)arg1, 0);
    v1 = (u32)cm_fn_00785830((void *)arg1, 1);
    *(u32 *)(frame + 0x04) = (u32)cm_fn_00785830((void *)arg1, 2);
    *(u32 *)(frame + 0x0c) = (u32)cm_fn_00785830((void *)arg1, 3);
    *(u32 *)(frame + 0x08) = (u32)cm_fn_00785830((void *)arg1, 4);
    *(u32 *)(frame + 0x00) = (u32)cm_fn_00785830((void *)arg1, 5);
    v6 = (u32)cm_fn_00785830((void *)arg1, 6);
    *(u32 *)(frame + 0x14) = (u32)cm_fn_00785830((void *)arg1, 7);
    *(u32 *)(frame + 0x10) = (u32)cm_fn_00785830((void *)arg1, 8);
    v9 = (u32)cm_fn_00785830((void *)arg1, 9);
    *(u32 *)(frame + 0x18) = v9;

    if (arg2 == 0) {
        u32 f0;
        u32 f8;
        u32 fc;

        /* 0x0041b0d3..0x0041b0db */
        f0 = *(u32 *)(frame + 0x00);
        f8 = *(u32 *)(frame + 0x08);
        fc = *(u32 *)(frame + 0x0c);
        /* push edi (0x0041b0cc) is the 7th, person argument. */
        cm_fn_0053e140(fc, f8, f0, (u32)(frame + 0x1c), 0x190, 5, v0);

        /* 0x0041b0f3 */
        if (*(u32 *)(frame + 0x04) != 0) {
            cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x00989820u,
                           0x64u, (u32)(frame + 0x1c));
        }
        else {
            cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x009897ecu,
                           (u32)(frame + 0x1c));
        }

        /* 0x0041b136 */
        cm_fn_004ae660((void *)arg3, (u32)(frame + 0x7ec));

        /* 0x0041b14a..0x0041b167 */
        g = 110u * v0 + CM_GLOBAL(u32, 0x00ae23bc);
        if (cm_fn_0053e990(g, fc, f8, *(u32 *)(frame + 0x00), 0) != 0) {
            cm_fn_004ae8a0((void *)arg3, frame + 0x1c, 0x7da,
                           110u * v0 + CM_GLOBAL(u32, 0x00ae23bc));
        }
    }
    else {
        u32 f0;
        u32 f8;
        u32 fc;
        u32 esc;

        /* 0x0041b1a3..0x0041b1b7 */
        f0 = *(u32 *)(frame + 0x00);
        f8 = *(u32 *)(frame + 0x08);
        fc = *(u32 *)(frame + 0x0c);
        cm_fn_0053e140(fc, f8, f0, (u32)(frame + 0x1c), 0x190, 1, v0);

        /* 0x0041b1c6: cmp ebp,-1 / jle */
        if ((s32)v6 > -1) {
            u32 f10;
            u32 f14;

            /* 0x0041b1cf..0x0041b1ea */
            f10 = *(u32 *)(frame + 0x10);
            f14 = *(u32 *)(frame + 0x14);
            cm_fn_0053e140(f14, f10, v9, (u32)(frame + 0x404), 0x190, 1, v6);

            /* 0x0041b1ef */
            if (*(u32 *)(frame + 0x04) != 0) {
                u32 p = 581u * v1 + CM_GLOBAL(u32, 0x00ae23b4);
                s32 b = *(s8 *)(p + 0x52u);

                /* 0x0041b233: eight pushed arguments. */
                cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x00989718u,
                               (u32)b, (u32)b, 0x64u, p + 0x38u,
                               (u32)(frame + 0x404), (u32)(frame + 0x1c));
            }
            else {
                u32 p = 581u * v1 + CM_GLOBAL(u32, 0x00ae23b4);
                s32 b = *(s8 *)(p + 0x52u);

                /* 0x0041b2e2. */
                cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x0098963cu,
                               (u32)b, (u32)b, p + 0x38u,
                               (u32)(frame + 0x404), (u32)(frame + 0x1c));
            }
            esc = 581u * v1;
        }
        else {
            /* 0x0041b279 */
            if (*(u32 *)(frame + 0x04) != 0) {
                u32 p = 581u * v1 + CM_GLOBAL(u32, 0x00ae23b4);
                s32 b = *(s8 *)(p + 0x52u);

                /* 0x0041b2e2. */
                cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x0098956cu,
                               (u32)b, (u32)b, 0x64u, p + 0x38u,
                               (u32)(frame + 0x1c));
            }
            else {
                u32 p = 581u * v1 + CM_GLOBAL(u32, 0x00ae23b4);
                s32 b = *(s8 *)(p + 0x52u);

                /* 0x0041b2e2: 100 precedes the sign-extended byte here. */
                cm_fn_0066f4e0((char *)(frame + 0x7ec), (const char *)0x0098949cu,
                               0x64u, (u32)b, (u32)b,
                               (u32)(frame + 0x1c), p + 0x38u);
            }
            esc = 581u * v1;
        }

        /* 0x0041b2ea */
        cm_fn_004ae660((void *)arg3, (u32)(frame + 0x7ec));

        /* 0x0041b300..0x0041b325 */
        {
            u32 a = *(u32 *)(frame + 0x00);
            u32 sec = *(u32 *)(frame + 0x08);
            u32 fir = *(u32 *)(frame + 0x0c);

            g = 110u * v0 + CM_GLOBAL(u32, 0x00ae23bc);
            if (cm_fn_0053e990(g, fir, sec, a, 0) != 0) {
                cm_fn_004ae8a0((void *)arg3, frame + 0x1c, 0x7da,
                               110u * v0 + CM_GLOBAL(u32, 0x00ae23bc));
            }
        }

        /* 0x0041b34b */
        {
            u32 e = esc + CM_GLOBAL(u32, 0x00ae23b4);

            cm_fn_004ae8a0((void *)arg3, (u8 *)(e + 0x38u), 0x7d5, e);
        }

        /* 0x0041b364: cmp ebp,-1 / jle */
        if ((s32)v6 > -1) {
            u32 t18;
            u32 t10;
            u32 t14;

            /* 0x0041b369..0x0041b37b */
            t18 = *(u32 *)(frame + 0x18);
            t10 = *(u32 *)(frame + 0x10);
            t14 = *(u32 *)(frame + 0x14);
            g = 110u * v6 + CM_GLOBAL(u32, 0x00ae23bc);
            if (cm_fn_0053e990(g, t14, t10, t18, 0) != 0) {
                cm_fn_004ae8a0((void *)arg3, frame + 0x404, 0x7da,
                               110u * v6 + CM_GLOBAL(u32, 0x00ae23bc));
            }
        }
    }
}
