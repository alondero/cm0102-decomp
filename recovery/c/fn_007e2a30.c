#include "cmre.h"

/* Object fields by byte offset from the stack argument (the object pointer):
   0x00 dword read through the element table, 0x2c and 0x2d flag bytes,
   0x48 flag byte. Access order follows the listing. */
#define OBJ_U8(off) (*(volatile u8 *)(obj + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(obj + (off)))

/* Entry frame (L = esp after `sub esp, 0x20`): frame[k] is at L+k.
   frame[0x03] flag byte, frame[0x04..0x0b] buffer passed as the first
   argument of 0x00417c10, frame[0x0c..0x1f] buffer passed to 0x00556940 and
   0x00557280 (char *), frame[0x10..] object passed as ECX to 0x0054a110 and
   0x00417c10. The SEH record sits at L+0x20 (prev, handler, state). */

extern void CM_FASTCALL cm_fn_0054a110(u32 p);
extern void CM_THISCALL cm_fn_00417c10(u32 self, u32 p1, u32 p2);
extern int CM_THISCALL cm_fn_007e82e0(u32 self, u32 p1, u32 p2);
extern void CM_THISCALL cm_fn_00556940(u32 self, u32 p1, u32 p2);
extern void CM_STDCALL cm_fn_00557280(u32 p1, u32 p2, u32 p3);

/* Receiver: ECX is forwarded unchanged to 0x007e82e0 (the first call reads
   it as `this` without this routine writing it), so it is an explicit
   parameter here. */
void CM_THISCALL cm_fn_007e2a30(u32 self, u32 obj, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    volatile u8 frame[0x20];
    u8 arg1_lo;
    u8 bl;
    u8 cl;
    u8 dl;
    u8 al;
    u32 p;
    u32 n;
    u32 g;

    seh.state = -1;
    seh.handler = 0x009604e8u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    frame[0x03] = 0xffu;
    bl = 0u;
    if (OBJ_U8(0x2c) != 0u || OBJ_U8(0x2d) != 0u) {
        n = OBJ_U32(0x00);
        p = (u32)cm_fn_007e82e0(self, n, 1u);

        if (OBJ_U8(0x2d) == 1u) {
            bl = 1u;
            frame[0x03] = 2u;
            arg1_lo = bl;
        }

        cl = OBJ_U8(0x2c);
        if (cl != 0u) {
            dl = (u8)(*(volatile u8 *)(p + 4u) + 1u);
            *(volatile u8 *)(p + 4u) = dl;
            if ((dl & 1u) == 0u) {
                bl = (u8)(bl + 1u);
                frame[0x03] = 1u;
                arg1_lo = 2u;
            }
        }

        if (bl != 0u) {
            cm_fn_0054a110((u32)&frame[0x10]);

            seh.state = 0;
            frame[0x0c] = bl;
            frame[0x0e] = 0xffu;
            frame[0x0f] = 0xffu;
            frame[0x0d] = 0u;
            cm_fn_00417c10((u32)&frame[0x10], (u32)&frame[0x04],
                           (u32)&CM_GLOBAL(u16, 0x00ae2c90));

            /* Stack argument slot low byte (arg1_lo) sign-extended to a word. */
            cl = OBJ_U8(0x48);
            frame[0x1d] = (u8)(s8)arg1_lo;
            frame[0x1e] = (u8)(((s16)(s8)arg1_lo) >> 8);
            al = frame[0x1f];
            dl = frame[0x03];
            al = (u8)(al & 0xfdu);
            frame[0x1c] = dl;
            cl = (cl == 0u) ? 1u : 0u;
            al = (u8)(al & 0xfbu);
            cl = (u8)((cl << 2) | al);
            frame[0x1f] = cl;

            n = OBJ_U32(0x00);
            frame[0x18] = 0xffu;
            frame[0x19] = 0xffu;
            frame[0x1a] = 0x02u;
            frame[0x1b] = 0xffu;
            cm_fn_00556940(CM_GLOBAL(volatile u32, 0x00ae28f0), n, (u32)&frame[0x0c]);

            n = OBJ_U32(0x00);
            g = CM_GLOBAL(volatile u32, 0x00ae23bc);
            (void)CM_GLOBAL(volatile u32, 0x00ae28f0);
            cm_fn_00557280(g + n * 110u, (u32)&frame[0x0c], arg2);
        }
    }

    CM_FS(u32, 0) = seh.prev;
}
