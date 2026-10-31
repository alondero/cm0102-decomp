#include "cmre.h"

extern u32 CM_CDECL cm_fn_0066f4e0(u32 dst, u32 fmt, ...);
extern unsigned long long CM_THISCALL cm_fn_0043f460(u32 *self, u32 arg);
extern u32 CM_CDECL cm_fn_00441c10(char *param_1, short param_2, u32 param_3, u32 param_4, u8 param_5);

/* Thiscall with four stack arguments (ret 0x10). ECX is a pointer to the object. arg1 is a
   signed byte selecting one of four constant strings; arg2 is a destination buffer; arg4's
   low byte selects between a plain copy (1) and a formatted number (otherwise).
   The routine links an SEH frame {prev, handler, state} at fs:[0]; the state is -1 on
   entry and 0 before the formatted path calls 0x0043f460. The local area below the frame
   is one buffer with explicit offsets: the first sprintf writes at +4, the dword read by
   0x0043f460 is at +0 and the formatted number at +0xcc (200 bytes). */
void CM_THISCALL cm_fn_0085f830(u8 *param_1, u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    volatile struct {
        u32 prev;
        u32 handler;
        s32 state;
    } frame;
    u8 local[0x194];
    s32 k;
    u32 str;
    u32 n;
    u32 i;
    u32 v;
    unsigned long long r64;
    u32 r;

    (void)arg3;
    frame.state = -1;
    frame.handler = 0x00962a4bU;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    k = (s32)(s8)(u8)arg1;
    if (k == 0) {
        str = 0x00a83744U;
    } else if (k == 1) {
        str = 0x00a83750U;
    } else if (k == 2) {
        str = 0x00a83760U;
    } else {
        str = 0x00a83774U;
    }
    cm_fn_0066f4e0((u32)(local + 4), str);

    if ((u8)arg4 == 1) {
        n = 0;
        while (local[4 + n] != 0) {
            n++;
        }
        n++;
        for (i = 0; i < (n >> 2); i++) {
            *(u32 *)(arg2 + 4 * i) = *(u32 *)(local + 4 + 4 * i);
        }
        for (i = (n >> 2) * 4; i < n; i++) {
            *(u8 *)(arg2 + i) = local[4 + i];
        }
    } else {
        v = *(u32 *)(param_1 + (u32)k * 4 + 0x10);
        *(u32 *)(local + 0) = v;
        frame.state = 0;
        r64 = cm_fn_0043f460((u32 *)local, 10U);
        r = cm_fn_00441c10((char *)(local + 0xcc), 0xc8, (u32)r64, (u32)(r64 >> 32), 10);
        (void)r;
        cm_fn_0066f4e0(arg2, 0x00a83720U, (u32)(local + 0xcc), (u32)(local + 4));
    }
    CM_FS(u32, 0) = frame.prev;
}
