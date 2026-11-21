/* fn_008e3b80 — plain C translation of the CM0102 3.9.68 original.
 *
 * Original: preferred-image VA 0x008e3b80 (body 549 B, module unknown).
 * Pinned binary: CM0102/Game/cm0102.exe 3.9.68, SHA-256
 *   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86
 * All VAs below are preferred-image VAs of that binary.
 *
 * ABI findings (cited from the original instructions):
 *  - 0x008e3cf5 and 0x008e3da2 `ret 0xc` pop 12 bytes of arguments, so the
 *    entry is stdcall with three stack arguments.  The arguments are read at
 *    0x008e3b8a ([esp+0x148] -> E+4), 0x008e3bd7 ([esp+0x14c] -> E+8) and
 *    0x008e3c59 / 0x008e3ca0 / 0x008e3cd1 / 0x008e3d3f ([esp+0x150],
 *    [esp+0x154], [esp+0x160] -> E+0xc).
 *  - 0x008e3b95, 0x008e3ba0, 0x008e3bab, 0x008e3bb6, 0x008e3bc3, 0x008e3bd0:
 *    six thiscall calls to 0x00785830 with ECX = argument 1 and one pushed
 *    index (0..5) each; the callee removes the pushed index.
 *  - 0x008e3bbf stores the index-3 result at frame+0 and 0x008e3bcc stores
 *    the index-4 result at frame+4; 0x008e3bed / 0x008e3cfe put the 300-byte
 *    output buffer at frame+8 (frame size 0x134).
 *  - 0x008e3be0 pushes argument-1's index-0 result (esi) as the seventh
 *    argument of the 0x0053e140 call in both branches (the `je` at
 *    0x008e3be1 is taken after that push); `add esp,0x1c` at 0x008e3c03 and
 *    0x008e3d14 confirms seven arguments.
 *  - 0x008e3c15 / 0x008e3c1b / 0x008e3d35 / 0x008e3d3a: the 0x0066f4e0
 *    call is cdecl and takes four arguments there (`add esp,0x10` at
 *    0x008e3c20 / 0x008e3d46) and seven arguments at 0x008e3c51
 *    (`add esp,0x1c` at 0x008e3c56).  The recovered two-parameter map of
 *    0x0066f4e0 (legacy record) cannot express either call, so it is
 *    declared variadic here; the fixed parameters keep the recovered types
 *    and the pushed dwords are bit-identical to the original.
 *
 * The entry returns the original's EAX: 0x008e3ce6 leaves the result of
 * 0x004ae8a0 in EAX in the argument-2 != 0 branch, and the argument-2 == 0
 * branch keeps either the 0x0053e990 result (0) or the 0x004ae8a0 result
 * (0x008e3d73 / 0x008e3d93).
 */

#include "cmre.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/* Original globals (preferred-image VAs). */
#define G_a9d7bc (&CM_GLOBAL(char, 0x00a9d7bc))
#define G_a9d7f4 (&CM_GLOBAL(char, 0x00a9d7f4))
#define G_a9d82c (&CM_GLOBAL(char, 0x00a9d82c))
#define G_a9d8d0 (&CM_GLOBAL(char, 0x00a9d8d0))
#define G_ae23a8 CM_GLOBAL(int, 0x00ae23a8)
#define G_ae23bc CM_GLOBAL(volatile u32, 0x00ae23bc)
#define G_de1f64 CM_GLOBAL(u32, 0x00de1f64)

/* Callees stay explicit; each binds to the original code. */
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(u32 self, const char *name, u32 key_a, u32 key_b);
extern u32 CM_CDECL cm_fn_0053e140(u32 first, u32 second, u32 third, u32 output, u32 size, u32 kind, u32 person);
extern u32 CM_CDECL cm_fn_0053e990(u32 person, u32 first, u32 second, u32 third, u32 date);
extern u32 CM_CDECL cm_fn_0053b3d0(u32 nation);
extern char * CM_CDECL cm_fn_0066f4e0(int param_1, char *param_2, ...);

u32 CM_STDCALL cm_fn_008e3b80(u32 param_1, int param_2, u32 param_3)
{
    u8 buf[300];            /* frame+8, 0x12c bytes, uninitialised in the original */
    u32 r0, r1, r2, r3, r4, r5;
    u32 rec;                /* DAT_00ae23a8 + r5 * 0x122 */
    u32 person;
    u32 res;

    /* 0x008e3b95 .. 0x008e3bd0: six thiscall reads of argument 1. */
    r0 = (u32)cm_fn_00785830((void *)param_1, 0);
    r1 = (u32)cm_fn_00785830((void *)param_1, 1);
    r2 = (u32)cm_fn_00785830((void *)param_1, 2);
    r3 = (u32)cm_fn_00785830((void *)param_1, 3);
    r4 = (u32)cm_fn_00785830((void *)param_1, 4);
    r5 = (u32)cm_fn_00785830((void *)param_1, 5);

    if (param_2 != 0) {
        /* 0x008e3be0 pushes r0 (person), 0x008e3beb..0x008e3bf9 push the
         * remaining six arguments of 0x0053e140 (kind 1). */
        (void)cm_fn_0053e140(r1, r2, r3, (u32)buf, 0x12c, 1, r0);
        if (r4 != 0) {
            /* 0x008e3c0a..0x008e3c1b: four arguments. */
            (void)cm_fn_0066f4e0((int)(u32)&G_de1f64, G_a9d8d0, 0x64, (u32)buf);
        } else {
            /* 0x008e3c25..0x008e3c51: seven arguments. */
            rec = (u32)G_ae23a8 + r5 * 0x122u;
            {
                int c = (int)*(volatile const s8 *)(rec + 0x37u); /* 0x008e3c3a */
                (void)cm_fn_0066f4e0((int)(u32)&G_de1f64, G_a9d82c, 0x64,
                                     (u32)c, (u32)c, (u32)buf, rec + 4u);
            }
        }
        /* 0x008e3c59..0x008e3c65 */
        (void)cm_fn_004ae660((void *)param_3, 0x00de1f64);
        /* 0x008e3c6a..0x008e3c88 */
        person = r0 * 110u + (u32)G_ae23bc;
        res = cm_fn_0053e990(person, r1, r2, r3, 0);
        if (res != 0) {
            /* 0x008e3c94..0x008e3cae */
            (void)cm_fn_004ae8a0(param_3, (const char *)buf, 0x7d7, r0 * 110u + G_ae23bc);
        }
        /* 0x008e3cb3..0x008e3cc6 */
        {
            u32 nation = cm_fn_0053b3d0((u32)G_ae23a8 + r5 * 0x122u);
            /* 0x008e3cdb..0x008e3ce6 */
            return cm_fn_004ae8a0(param_3, (const char *)((u32)G_ae23a8 + r5 * 0x122u + 4u),
                                   0x7d5, nation);
        }
    } else {
        /* 0x008e3cfc..0x008e3d0b: same seven arguments, kind 5. */
        (void)cm_fn_0053e140(r1, r2, r3, (u32)buf, 0x12c, 5, r0);
        if (r4 != 0) {
            /* 0x008e3d1b..0x008e3d22 */
            (void)cm_fn_0066f4e0((int)(u32)&G_de1f64, G_a9d7f4, 0x64, (u32)buf);
        } else {
            /* 0x008e3d29..0x008e3d30 */
            (void)cm_fn_0066f4e0((int)(u32)&G_de1f64, G_a9d7bc, 0x64, (u32)buf);
        }
        /* 0x008e3d3f..0x008e3d50 */
        (void)cm_fn_004ae660((void *)param_3, 0x00de1f64);
        /* 0x008e3d55..0x008e3d73 */
        person = r0 * 110u + (u32)G_ae23bc;
        res = cm_fn_0053e990(person, r1, r2, r3, 0);
        if (res != 0) {
            /* 0x008e3d7f..0x008e3d93 */
            return cm_fn_004ae8a0(param_3, (const char *)buf, 0x7d7, r0 * 110u + G_ae23bc);
        }
        /* 0x008e3d98: EAX still holds the zero result of 0x0053e990. */
        return res;
    }
}
