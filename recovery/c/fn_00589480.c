/* fn_00589480 @ preferred-image VA 0x00589480, image base 0x00400000.
   Pinned binary: CM0102/Game/cm0102.exe, SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86 (3.9.68).

   HELD concern "calls stack probe 0x945610 from compiled C" is resolved by
   omitting the probe in plain C, which C-BOUNDARIES allows as the alternative
   to a reviewed compiled-C contract. The original prologue is

       00589480  mov eax, 0x16f0
       00589485  call 0x945610            ; MSVC stack-probe/allocation helper
       0058948a  mov eax, [esp + 0x16f8]  ; arg at entry-SP + 8

   and the epilogue is

       005896ba  add esp, 0x16f0

   0x00945610 receives the frame size in EAX and lowers ESP by that size; it
   carries no engine state of this routine. Under a compiled C caller that
   binding has no reviewed contract (the helper would lower ESP with no matching
   restore inside C), so the helper call is not reproduced. Its only effect that
   matters to this body, reserving 0x16f0 bytes of frame, is performed by the C
   compiler for the `frame` scratch object below (see ADR 0006 for the same
   treatment of compiler-inserted helpers). The probe itself writes only stack
   pages below ESP and is invisible to the routine's arguments, memory and
   return value.

   Frame model. After the probe, ESP = L = entry-SP - 0x16f0. All landmarks are
   taken from the original's `[esp + N]` sites with the callee-cleaned argument
   pushes (each `call 0x785830` pops its index; the two `call 0x53e140` argument
   sets are cleared together by `add esp, 0x38` at 0x0058955b):

       0058948a [esp+0x16f8] = L+0x16f8 = arg2 (flag, selects the branch)
       0058949f [esp+0x1708] = L+0x16f4 = arg1 (thiscall object)
       005894d2 [esp+0x14]   = L+0x0    = result of get(3)
       005894e8 [esp+0x18]   = L+0x4    = result of get(5)
       005894f5 [esp+0x24]   = L+0x10   = result of get(6)
       00589502 [esp+0x1c]   = L+0x8    = result of get(7)
       0058950f [esp+0x20]   = L+0xc    = result of get(8)
       00589521 lea [esp+0x60] (esp=L-0x18)          = L+0x48  record A
       00589542 lea [esp+0x20c] (esp=L-0x34)         = L+0x1d8 record B
       00589568/00589574 lea [esp+0x28] (esp=L-0x14) = L+0x14  string A
       005895a0 lea [esp+0x394] (esp=L-0x2c)         = L+0x368 string C
       0058962d lea [esp+0x60] (esp=L-0x18)          = L+0x48  record A
       00589648 lea [esp+0x378] (esp=L-0x10)         = L+0x368 string C
       005896a6 lea [esp+0x58] (esp=L-0x10)          = L+0x48  record A
       00589697 [esp+0x170c] = L+0x16fc = arg3 (receiver of 0x004ae660/0x004ae8a0)
       005895b2 [esp+0x1730] (esp=L-0x10) = arg3, same home

   The two branches write and read the same record buffer at L+0x48, and the
   `cmp eax, 1` at 0x0058955e tests the value spilled at L+0x4 (result of get(5)).
   Plain-call order, widths and the original bug where the get(4) result is
   discarded (0x005894d6 has no store) are kept as in the listing. */

#include "cmre.h"

#define FRAME_BYTES 0x16f0u
#define FRAME_WORD(off) (frame[(off) / 4u])
#define FRAME_PTR(off) ((u32)(u32 *)frame + (u32)(off))
#define GLOBAL_0AE23BC CM_GLOBAL(u32, 0x00ae23bcu)

extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern char *CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

void CM_CDECL cm_fn_00589480(u32 arg_04, u32 arg_08, u32 arg_0c)
{
    u32 frame[FRAME_BYTES / 4u];
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r9;
    u32 rv;

    /* 0x0058948a: mov eax, [esp + 0x16f8]; test eax, eax; je 0x005895fb */
    if (arg_08 != 0) {
        /* The frame buffers are left uninitialised, as in the listing: only the
           callees write them. */
        r0 = (u32)cm_fn_00785830((void *)arg_04, 0u);
        r1 = (u32)cm_fn_00785830((void *)arg_04, 1u);
        r2 = (u32)cm_fn_00785830((void *)arg_04, 2u);
        FRAME_WORD(0x0) = (u32)cm_fn_00785830((void *)arg_04, 3u);
        cm_fn_00785830((void *)arg_04, 4u);           /* 0x005894d6: result unused */
        FRAME_WORD(0x4) = (u32)cm_fn_00785830((void *)arg_04, 5u);
        FRAME_WORD(0x10) = (u32)cm_fn_00785830((void *)arg_04, 6u);
        FRAME_WORD(0x8) = (u32)cm_fn_00785830((void *)arg_04, 7u);
        FRAME_WORD(0xc) = (u32)cm_fn_00785830((void *)arg_04, 8u);
        r9 = (u32)cm_fn_00785830((void *)arg_04, 9u);

        /* 0x0058952e: (r1, r2, get(3), L+0x48, 0x190, 1, r0) */
        cm_fn_0053e140(r1, r2, FRAME_WORD(0x0), FRAME_PTR(0x48), 0x190u, 1u, r0);
        /* 0x00589552: (get(7), get(8), r9, L+0x1d8, 0x190, 1, get(6)) */
        cm_fn_0053e140(FRAME_WORD(0x8), FRAME_WORD(0xc), r9, FRAME_PTR(0x1d8), 0x190u, 1u, FRAME_WORD(0x10));

        /* 0x0058955e: cmp [L+0x4], 1; jne 0x00589574 */
        if (FRAME_WORD(0x4) == 1u) {
            cm_fn_0066f4e0(FRAME_PTR(0x14), 0x009c666cu, 0x00ad9c64u);
        } else {
            cm_fn_0066f4e0(FRAME_PTR(0x14), 0x009c6640u, 0x00ad9c64u);
        }

        /* 0x005895ad: (L+0x368, 0x9c654c, 0x64, 0, 0, 0x64, L+0x48, L+0x14, L+0x1d8) */
        cm_fn_0066f4e0(FRAME_PTR(0x368), 0x009c654cu, 0x64u, 0u, 0u, 0x64u,
                       FRAME_PTR(0x48), FRAME_PTR(0x14), FRAME_PTR(0x1d8));

        /* 0x005895c4: ecx = arg3, arg = L+0x368; EAX/EDX discarded */
        cm_fn_004ae660((void *)arg_0c, FRAME_PTR(0x368));

        /* 0x005895e6: (110*r0 + [0xae23bc], r1, r2, get(3), 0) */
        rv = cm_fn_0053e990((110u * r0 + GLOBAL_0AE23BC), r1, r2, FRAME_WORD(0x0), 0u);
    } else {
        r0 = (u32)cm_fn_00785830((void *)arg_04, 0u);
        r1 = (u32)cm_fn_00785830((void *)arg_04, 1u);
        r2 = (u32)cm_fn_00785830((void *)arg_04, 2u);
        r9 = (u32)cm_fn_00785830((void *)arg_04, 3u);

        /* 0x0058963c: (r1, r2, get(3), L+0x48, 0x190, 5, r0) */
        cm_fn_0053e140(r1, r2, r9, FRAME_PTR(0x48), 0x190u, 5u, r0);

        /* 0x00589656: (L+0x368, 0x9c651c, L+0x48) */
        cm_fn_0066f4e0(FRAME_PTR(0x368), 0x009c651cu, FRAME_PTR(0x48));

        /* 0x0058966d: ecx = arg3, arg = L+0x368 */
        cm_fn_004ae660((void *)arg_0c, FRAME_PTR(0x368));

        /* 0x0058968b: (110*r0 + [0xae23bc], r1, r2, get(3), 0) */
        rv = cm_fn_0053e990((110u * r0 + GLOBAL_0AE23BC), r1, r2, r9, 0u);
    }

    /* 0x005895f0 / 0x00589695: je 0x005896b6; shared 0x00589697 tail. */
    if (rv != 0) {
        /* 0x005896b1: ecx = arg3 (0x0058969d), (arg3, L+0x48, 0x7da, 110*r0 + [0xae23bc]) */
        cm_fn_004ae8a0((int)arg_0c, (u8 *)FRAME_PTR(0x48), 0x7da, (int)(110u * r0 + GLOBAL_0AE23BC));
    }
}
