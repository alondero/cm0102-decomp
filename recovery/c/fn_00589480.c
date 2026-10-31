/* WIP: passes the gate but not accepted. The stack probe 0x00945610 is called
   from compiled C (regparm(1) wrapper); C-BOUNDARIES "Original stack-probe return
   slots" says that compiled binding has no reviewed contract, and at -O2 the wrapper
   pops EBX/ESI/EDI after the helper has lowered ESP by 0x16f0. Needs a boundary
   decision for _chkstk callers before acceptance; body not yet parent-reviewed. */
#include "cmre.h"

/* Frame words: the original reserves 0x16f0 bytes with the stack-probe helper
   0x00945610, which takes the byte count in EAX (declared regparm(1) so EAX
   carries it). It is called here from the entry stack pointer, as in the
   original, before the body. The body keeps its frame in one scratch array
   and FRAME_WORD(off) / FRAME_PTR(off) use byte offsets from that array; all
   offsets are multiples of four.
   Caller arguments, by offset from the entry stack pointer: +4 object (ECX for
   0x00785830), +8 flag (zero selects the second branch), +0xc object (ECX for
   0x004ae660 and 0x004ae8a0). */

#define FRAME_BYTES 0x16f0u
#define FRAME_WORD(off) (scratch[(off) / 4u])
#define FRAME_PTR(off) ((u32)scratch + (u32)(off))
#define GLOBAL_0AE23BC CM_GLOBAL(u32, 0x00ae23bcu)

extern void __attribute__((regparm(1))) cm_fn_00945610(u32 frame_bytes);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern unsigned long long CM_THISCALL cm_fn_004ae660(void *self, u32 arg1);
extern u32 CM_THISCALL cm_fn_004ae8a0(int param_1, u8 *param_2, int param_3, int param_4);
extern u32 cm_fn_0053e140(int param_1, int param_2, int param_3, int param_4, u32 param_5, u32 param_6, int param_7);
extern u32 cm_fn_0053e990(int param_1, int param_2, int param_3, int param_4, int param_5);
extern char *cm_fn_0066f4e0(u32 dst, u32 fmt, ...);

static void __attribute__((noinline)) cm_body_00589480(u32 arg_04, u32 arg_08, u32 arg_0c)
{
    u32 scratch[FRAME_BYTES / 4u];
    u32 r0;
    u32 r1;
    u32 r2;
    u32 r3;
    u32 r9;
    u32 rv;

    if (arg_08 != 0) {
        r0 = (u32)cm_fn_00785830((void *)arg_04, 0u);
        r1 = (u32)cm_fn_00785830((void *)arg_04, 1u);
        r2 = (u32)cm_fn_00785830((void *)arg_04, 2u);
        FRAME_WORD(0) = (u32)cm_fn_00785830((void *)arg_04, 3u);
        cm_fn_00785830((void *)arg_04, 4u);
        FRAME_WORD(4) = (u32)cm_fn_00785830((void *)arg_04, 5u);
        FRAME_WORD(16) = (u32)cm_fn_00785830((void *)arg_04, 6u);
        FRAME_WORD(8) = (u32)cm_fn_00785830((void *)arg_04, 7u);
        FRAME_WORD(12) = (u32)cm_fn_00785830((void *)arg_04, 8u);
        r9 = (u32)cm_fn_00785830((void *)arg_04, 9u);

        cm_fn_0053e140((int)r1, (int)r2, (int)FRAME_WORD(0), (int)FRAME_PTR(0x48), 0x190u, 1u, (int)r0);
        cm_fn_0053e140((int)FRAME_WORD(8), (int)FRAME_WORD(12), (int)r9, (int)FRAME_PTR(0x1d8), 0x190u, 1u, (int)FRAME_WORD(16));

        if (FRAME_WORD(4) == 1u) {
            cm_fn_0066f4e0(FRAME_PTR(0x14), 0x009c666cu, 0x00ad9c64u);
        } else {
            cm_fn_0066f4e0(FRAME_PTR(0x14), 0x009c6640u, 0x00ad9c64u);
        }

        cm_fn_0066f4e0(FRAME_PTR(0x368), 0x009c654cu, 0x64u, 0u, 0u, 0x64u, FRAME_PTR(0x48), FRAME_PTR(0x14), FRAME_PTR(0x1d8));
        cm_fn_004ae660((void *)arg_0c, FRAME_PTR(0x368));
        rv = cm_fn_0053e990((int)(r0 * 110u + GLOBAL_0AE23BC), (int)r1, (int)r2, (int)FRAME_WORD(0), 0);
    } else {
        r0 = (u32)cm_fn_00785830((void *)arg_04, 0u);
        r1 = (u32)cm_fn_00785830((void *)arg_04, 1u);
        r2 = (u32)cm_fn_00785830((void *)arg_04, 2u);
        r3 = (u32)cm_fn_00785830((void *)arg_04, 3u);

        cm_fn_0053e140((int)r1, (int)r2, (int)r3, (int)FRAME_PTR(0x48), 0x190u, 5u, (int)r0);
        cm_fn_0066f4e0(FRAME_PTR(0x368), 0x009c651cu, FRAME_PTR(0x48));
        cm_fn_004ae660((void *)arg_0c, FRAME_PTR(0x368));
        rv = cm_fn_0053e990((int)(r0 * 110u + GLOBAL_0AE23BC), (int)r1, (int)r2, (int)r3, 0);
    }

    if (rv != 0) {
        cm_fn_004ae8a0((int)arg_0c, (u8 *)FRAME_PTR(0x48), 0x7da, (int)(r0 * 110u + GLOBAL_0AE23BC));
    }
}

void CM_CDECL cm_fn_00589480(u32 arg_04, u32 arg_08, u32 arg_0c)
{
    cm_fn_00945610(FRAME_BYTES);
    cm_body_00589480(arg_04, arg_08, arg_0c);
}
