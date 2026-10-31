#include "cmre.h"

/* Stack arguments: a1 (first, ESI) and a2 (second, EDI) are object pointers.
   Fields read through a1: 0x1a and 0x61 (non-null checks; 0x61 holds a pointer
   whose word at +0x0b is compared), 0x00, 0x04, 0x08, 0x0c, 0x61 (pointers whose
   dword at +0x33 is passed on). Fields read through a2: 0x53, 0x69 (pointer, its
   dword at +0x38 checked), 0x80 word, 0xcf (pointer to a second object whose
   fields 0x00, 0x04, 0x08, 0x0c are passed on), and a2 +0x00 dword.
   Scratch 0xe0 bytes below the frame serve as the `this` object for the
   0x00785790 and 0x0077bbe0 calls. */
#define ARG1_U32(off) (*(volatile u32 *)(a1 + (off)))
#define ARG2_U32(off) (*(volatile u32 *)(a2 + (off)))

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self_arg, u32 index_arg, u32 value_arg);
extern unsigned long long CM_THISCALL cm_fn_007861e0(void *self, u32 arg1, u32 arg2);
extern int cm_fn_0090cfc0(int param_1);

void CM_CDECL cm_fn_00589bc0(u32 a1, u32 a2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u8 scr[0xe0];
    u32 v;
    u32 t;
    u32 c;

    seh.state = -1;
    seh.handler = 0x00958c5bu;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    if (a1 == 0) {
        goto exit;
    }
    if (ARG1_U32(0x61) == 0) {
        goto exit;
    }
    if (ARG1_U32(0x1a) == 0) {
        goto exit;
    }
    if (a2 == 0) {
        goto exit;
    }
    if (ARG2_U32(0x53) == 0) {
        goto exit;
    }
    t = ARG2_U32(0x69);
    if (t == 0) {
        goto exit;
    }
    if (*(volatile u32 *)(t + 0x38u) == 0) {
        goto exit;
    }
    if (ARG2_U32(0xcf) == 0) {
        goto exit;
    }

    cm_fn_0077bbe0((void *)scr, 0xbe2u, 0u);

    v = *(volatile u32 *)(a1 + 0x00u);
    seh.state = 0;
    cm_fn_00785790((void *)scr, 0u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(a1 + 0x04u) + 0x33u);
    cm_fn_00785790((void *)scr, 1u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(a1 + 0x08u) + 0x33u);
    cm_fn_00785790((void *)scr, 2u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(a1 + 0x0cu) + 0x33u);
    cm_fn_00785790((void *)scr, 3u, v);

    v = *(volatile u32 *)(a2 + 0x00u);
    cm_fn_00785790((void *)scr, 4u, v);

    v = 1u + (((s16)*(volatile u16 *)(ARG1_U32(0x61) + 0x0bu) > (s16)0x1c52) ? 1u : 0u);
    cm_fn_00785790((void *)scr, 5u, v);

    c = ARG2_U32(0xcf);

    v = *(volatile u32 *)(c + 0x00u);
    cm_fn_00785790((void *)scr, 6u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(c + 0x04u) + 0x33u);
    cm_fn_00785790((void *)scr, 7u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(c + 0x08u) + 0x33u);
    cm_fn_00785790((void *)scr, 8u, v);

    v = *(volatile u32 *)(*(volatile u32 *)(c + 0x0cu) + 0x33u);
    cm_fn_00785790((void *)scr, 9u, v);

    if ((s16)*(volatile u16 *)(a2 + 0x80u) > (s16)0x186a) {
        v = (u32)cm_fn_0090cfc0(2);
    } else {
        v = 0u;
    }
    cm_fn_00785790((void *)scr, 0x0au, v);

    cm_fn_007861e0((void *)CM_GLOBAL(volatile u32, 0x00b7431c), (u32)scr, a2);

exit:
    CM_FS(u32, 0) = seh.prev;
}
