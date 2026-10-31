#include "cmre.h"

/* Object fields by byte offset from the object pointer (first stack argument):
   0x00 dword, 0x2c byte flag, 0x2d byte flag, 0x48 byte flag.
   The entry ECX is the receiver forwarded to 0x007e82e0 (thiscall). */
#define OBJ_U8(off) (*(volatile u8 *)(obj + (off)))
#define OBJ_U32(off) (*(volatile u32 *)(obj + (off)))

extern int CM_THISCALL cm_fn_007e82e0(int param_1, u32 param_2, int param_3);
extern void CM_FASTCALL cm_fn_0054a110(u16 *param_1);
extern void CM_THISCALL cm_fn_00417c10(u32 *param_1, u32 *param_2, u16 *param_3);
extern void CM_THISCALL cm_fn_00556940(int param_1, int param_2, char *param_3);
extern void CM_STDCALL cm_fn_00557280(int param_1, char *param_2, int param_3);

void CM_THISCALL cm_fn_007e1420(u32 self, u32 arg1, u32 arg2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    /* Scratch area of the original's `sub esp, 0x20` locals. Offsets are
       from the post-`sub` base; the callee buffers start at 0x04, 0x0c and 0x10. */
    u8 frame[0x20];
    u32 obj;
    u8 bl;

    seh.state = -1;
    seh.handler = 0x00960398u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    obj = arg1;
    bl = 0;
    frame[0x03] = 0xff;
    if (OBJ_U8(0x2c) != 0 || OBJ_U8(0x2d) != 0) {
        u32 first;
        int ret;
        u32 ptr;
        u8 count;

        first = OBJ_U32(0x00);
        ret = cm_fn_007e82e0((int)self, first, 1);
        if (OBJ_U8(0x2d) == 1) {
            bl = 1;
            frame[0x03] = 0x02;
            arg1 = (arg1 & 0xffffff00u) | bl;
        }
        if (OBJ_U8(0x2c) != 0) {
            ptr = (u32)ret;
            count = (u8)(*(volatile u8 *)(ptr + 4) + 1);
            *(volatile u8 *)(ptr + 4) = count;
            if (count % 3 == 0) {
                bl = (u8)(bl + 1);
                frame[0x03] = 0x01;
                arg1 = (arg1 & 0xffffff00u) | 3u;
            }
        }

        if (bl != 0) {
            u8 al;
            u8 al2;
            u8 dl;
            u16 cx;
            u32 x;
            s32 g;
            u32 gthis;

            cm_fn_0054a110((u16 *)&frame[0x10]);
            seh.state = 0;
            frame[0x0c] = bl;
            frame[0x0d] = 0;
            frame[0x0e] = 0xff;
            frame[0x0f] = 0xff;
            cm_fn_00417c10((u32 *)&frame[0x10], (u32 *)&frame[0x04], (u16 *)0x00ae2c90u);

            al = frame[0x03];
            dl = OBJ_U8(0x48);
            frame[0x1c] = al;
            al2 = frame[0x1f];
            al2 = (u8)(al2 & 0xfd);
            cx = (u16)(s16)(s8)(u8)arg1;
            frame[0x1d] = (u8)cx;
            frame[0x1e] = (u8)(cx >> 8);
            dl = (dl == 0) ? 4 : 0;
            x = OBJ_U32(0x00);
            al2 = (u8)(al2 & 0xfb);
            frame[0x18] = 0xff;
            dl = (u8)(dl | al2);
            gthis = CM_GLOBAL(volatile u32, 0x00ae28f0);
            frame[0x19] = 0xff;
            frame[0x1a] = 0x23;
            frame[0x1b] = 0xff;
            frame[0x1f] = dl;
            cm_fn_00556940((int)gthis, (int)x, (char *)&frame[0x0c]);

            x = OBJ_U32(0x00);
            g = CM_GLOBAL(volatile s32, 0x00ae23bc);
            (void)CM_GLOBAL(volatile u32, 0x00ae28f0);
            cm_fn_00557280((int)((u32)g + x * 110u), (char *)&frame[0x0c], (int)arg2);
        }
    }
    CM_FS(u32, 0) = seh.prev;
}
