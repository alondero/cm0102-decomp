#include "cmre.h"

/* Argument object `o` (first stack argument) by byte offset:
   0x0c pointer, 0x10 pointer, 0x18 pointer, 0x1c pointer, 0x20 pointer,
   0x28 word, 0x2a word. Chain from the 0x18 pointer: +0x38 pointer, then +0x1f dword.
   Other pointers: +0x7e byte (must equal 3), +0x53 dword. */
#define OBJ_U16(off) (*(volatile u16 *)(o + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(o + (off)))
#define PTR_U8(p, off) (*(volatile u8 *)((p) + (off)))
#define PTR_U32(p, off) (*(volatile u32 *)((p) + (off)))

extern unsigned long long CM_THISCALL cm_fn_00549ef0(void *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_0054c8e0(short *param_1, u32 *param_2, int param_3);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern void CM_THISCALL cm_fn_0062e820(int param_1, u32 param_2, char param_3, int param_4, u32 *param_5);

void CM_STDCALL cm_fn_005ab3c0(u32 arg1)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    /* Scratch area of the original's `sub esp, 0x20` locals. Callee buffers
       are at offsets 0x00, 0x08, 0x10 and 0x18. */
    u8 scratch[0x20];
    u32 o;
    u32 p1;
    u32 p2;
    u32 p3;
    u32 p4;
    u32 p5;
    u32 p6;
    u32 p7;
    u32 g;

    seh.state = -1;
    seh.handler = 0x009596c0u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    o = arg1;
    p1 = OBJ_U32(0x18);
    if (p1 == 0) {
        goto L_end;
    }
    p2 = PTR_U32(p1, 0x38);
    if (p2 == 0) {
        goto L_end;
    }
    p3 = PTR_U32(p2, 0x1f);
    if (p3 == 0) {
        goto L_end;
    }
    if (PTR_U8(p3, 0x7e) != 3) {
        goto L_end;
    }
    p4 = OBJ_U32(0x1c);
    if (p4 == 0) {
        goto L_480;
    }
    if (PTR_U32(p4, 0x53) == p3) {
        goto L_480;
    }

    {
        u32 pushA;
        u32 pushB;
        u32 r1;

        pushA = (p4 & 0xffff0000u) | OBJ_U16(0x28);
        pushB = (p3 & 0xffff0000u) | OBJ_U16(0x2a);
        (void)cm_fn_00549ef0((void *)&scratch[0x00], pushB, pushA);

        seh.state = 0;
        r1 = cm_fn_0054c8e0((short *)&scratch[0x00], (u32 *)&scratch[0x08], 1);
        *(volatile u8 *)&seh.state = 1;
        cm_fn_00417c10((u32 *)&scratch[0x00], (u32 *)&scratch[0x10], (u16 *)(u32)r1);

        p1 = OBJ_U32(0x18);
        *(volatile u8 *)&seh.state = 0;
        p2 = PTR_U32(p1, 0x38);
        p5 = OBJ_U32(0x0c);
        p3 = PTR_U32(p2, 0x1f);
        g = CM_GLOBAL(volatile u32, 0x00b63d28);
        cm_fn_0062e820((int)g, (u32)&scratch[0x00], (char)3, (int)p5, (u32 *)p3);
        seh.state = -1;
    }

L_480:
    p6 = OBJ_U32(0x20);
    if (p6 == 0) {
        goto L_end;
    }
    p1 = OBJ_U32(0x18);
    {
        u32 cmpv;
        u32 pushC;
        u32 pushD;
        u32 r2;

        cmpv = PTR_U32(p6, 0x53);
        p2 = PTR_U32(p1, 0x38);
        if (cmpv == PTR_U32(p2, 0x1f)) {
            goto L_end;
        }
        pushC = (p1 & 0xffff0000u) | OBJ_U16(0x28);
        pushD = (p2 & 0xffff0000u) | OBJ_U16(0x2a);
        (void)cm_fn_00549ef0((void *)&scratch[0x08], pushD, pushC);

        seh.state = 2;
        r2 = cm_fn_0054c8e0((short *)&scratch[0x08], (u32 *)&scratch[0x10], 1);
        *(volatile u8 *)&seh.state = 3;
        cm_fn_00417c10((u32 *)&scratch[0x08], (u32 *)&scratch[0x18], (u16 *)(u32)r2);

        p1 = OBJ_U32(0x18);
        *(volatile u8 *)&seh.state = 2;
        p2 = PTR_U32(p1, 0x38);
        p7 = OBJ_U32(0x10);
        p3 = PTR_U32(p2, 0x1f);
        g = CM_GLOBAL(volatile u32, 0x00b63d28);
        cm_fn_0062e820((int)g, (u32)&scratch[0x08], (char)3, (int)p7, (u32 *)p3);
    }

L_end:
    CM_FS(u32, 0) = seh.prev;
}
