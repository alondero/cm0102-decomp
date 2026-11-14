/* fn_0090a900 - preferred-image VA 0x0090a900 (image base 0x00400000).
   Recovered from the pinned original listing only; callees stay explicit
   boundaries (bound to the original code).

   Original frame (entry esp = E):
     E-20..E-13  8 bytes reserved by `sub esp, 8`  -> L+0 buffer
     E-12..E-9   SEH prev   (fs:[0] on entry)
     E-8..E-5    SEH handler (immediate 0x009654a8)
     E-4..E-1    SEH state  (init -1, later set to 0)
   After `call 0x549f70` (thiscall, four stack args -> 16 bytes popped by the
   callee) esp is E-32, hence:
     [esp+0x28] = [E+8]  = second stack argument of this function
     [esp+0x1c] = [E-4]  = SEH state
     [esp+0x0c] = [E-20] = buffer + 0
     [esp+0x0e] = [E-18] = buffer + 2  (vbuf[1], tested at 0090a945/0090a94a)
*/

#include "cmre.h"

/* The original pushes { prev, handler, state } and links it with
   `mov dword ptr fs:[0], esp`; it is unlinked with the saved prev. */
typedef struct cm_seh_frame_s {
    void *prev;
    void *handler;
    s32 state;
} cm_seh_frame;

extern short *CM_THISCALL cm_fn_00549f70(short *param_1, short param_2, char param_3, u16 param_4, u32 param_5);
extern u32 CM_THISCALL cm_fn_0051f890(int *param_1, int *param_2, int param_3);
extern void CM_FASTCALL cm_fn_0090a980(int param_1);

u32 CM_THISCALL cm_fn_0090a900(void *param_this, short *param_1, int param_2)
{
    volatile cm_seh_frame frame;
    u32 buf[2];                          /* L+0 .. L+7, left uninitialized */
    volatile u16 *vbuf = (volatile u16 *)(void *)buf;
    u32 result;

    frame.state = -1;                    /* push -1                */
    frame.handler = (void *)0x009654a8;  /* push 0x9654a8          */
    frame.prev = CM_FS(void *, 0);       /* push dword ptr fs:[0]  */
    CM_FS(void *, 0) = (void *)&frame;   /* mov dword ptr fs:[0], esp */

    /* ecx = &buf (lea ecx, [esp+0x10]); ax = word ptr [esi+2] */
    cm_fn_00549f70((short *)(void *)buf, (short)0x1a, (char)6,
                   (u16)param_1[1], (u32)6);

    frame.state = 0;                     /* mov dword ptr [esp+0x1c], 0 */

    /* 0090a945: ax = word [esp+0xe] = buffer+2 (vbuf[1]);
       0090a950: cx = word [esp+0xc] = buffer+0 (vbuf[0]) */
    if ((param_2 != 0) && (vbuf[1] == (u16)param_1[1]) &&
        (vbuf[0] == (u16)param_1[0])) {
        cm_fn_0090a980((int)(unsigned long)param_this);   /* ecx = edi */
    }

    result = cm_fn_0051f890((int *)param_this, (int *)param_1, param_2);

    CM_FS(void *, 0) = frame.prev;       /* mov dword ptr fs:[0], ecx */
    return result;
}
