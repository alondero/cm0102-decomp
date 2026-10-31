#include "cmre.h"

/* Entry: THISCALL (ECX saved at frame offset +8 and reused as the receiver of
   0x007f6cd0, 0x007ae9c0 and 0x007f9890), two stack arguments (ret 8).
   The stack slot of the first argument holds the object pointer (ebp) until
   the 0x00945501 allocation overwrites it before the inner loop. */

extern void CM_FASTCALL cm_fn_0068d020(u16 *param_1);
extern void CM_FASTCALL cm_fn_0068d040(int param_1);
extern u32 CM_THISCALL cm_fn_007ae9c0(int self, int param_2, u32 param_3, int param_4, int param_5, int param_6);
extern u32 CM_THISCALL cm_fn_007b5be0(int self, int *param_1, int *param_2, int param_3);
extern int CM_THISCALL cm_fn_007f6cd0(int self, char param_2, int *param_3);
extern void CM_THISCALL cm_fn_007f9890(int self, short *param_1);
extern int CM_CDECL cm_fn_0090cfc0(int param_1);
extern unsigned long long CM_CDECL cm_fn_009452ca(u32 arg4);
extern void *CM_CDECL cm_fn_00945501(u32 arg1, u32 arg2);

u32 CM_THISCALL cm_fn_007f8c00(u32 self, u32 param_1, u32 param_2)
{
    volatile struct { u32 prev; u32 handler; s32 state; } seh;
    u32 buf[4];      /* 16 bytes at frame offset +0x10 */
    s32 loop_i;      /* frame offset +0 */
    s32 loop_left;   /* frame offset +4 */
    s8 slot;         /* frame offset +0x0c, byte store only */
    u32 array;       /* stack slot of the first argument after 0x007f8c74 */
    u32 obj;         /* ebp: first argument as read at entry */
    u32 iv1;
    s32 iv3;
    s8 cvar5;
    u32 iv4;
    u32 target;

    seh.state = -1;
    seh.handler = 0x00960fd8u;
    seh.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&seh;

    cm_fn_0068d020((u16 *)buf);

    obj = param_1;
    seh.state = 0;
    loop_i = 0;
    loop_left = 0x10;
    do {
        iv3 = (s32)((u32)CM_GLOBAL(s32, 0x00ae2364) - (u32)loop_i - 1u);
        iv1 = (u32)CM_GLOBAL(s32, 0x00ae23bc) + (u32)iv3 * 0x6eu;
        if (*(s8 *)(iv1 + 0x5fu) == 1) {
            array = (u32)cm_fn_00945501(10u, 4u);
            cvar5 = 0;
            do {
                do {
                    slot = (s8)cm_fn_0090cfc0(10);
                } while (*(s32 *)(array + (u32)(s32)slot * 4u) != 0);
                *(u32 *)(array + (u32)(s32)slot * 4u) = 1;
                iv4 = (u32)cm_fn_007f6cd0((int)self, slot, (int *)iv1);
                if (*(s32 *)(iv4 + 0x19d) != 0 &&
                    *(u8 *)(obj + 0x40) != 0 &&
                    *(u8 *)(obj + 0x41) != 0) {
                    target = *(u32 *)(iv1 + 0x39);
                    if (*(u32 *)(obj + 0x1c) == target ||
                        *(u32 *)(obj + 0x20) == target) {
                        cm_fn_007ae9c0((int)iv4, -1, 0xffffffffu, -1, 0, 1);
                    }
                }
                cm_fn_007b5be0((int)iv4, (int *)obj, (int *)param_2, (int)buf);
                cvar5 = (s8)(cvar5 + 1);
            } while (cvar5 < 10);
            cm_fn_009452ca(array);
        }
        loop_i = loop_i + 1;
        loop_left = loop_left - 1;
    } while (loop_left != 0);

    cm_fn_007f9890((int)self, (short *)buf);
    seh.state = -1;
    cm_fn_0068d040((int)buf);
    CM_FS(u32, 0) = seh.prev;
    return 1u;
}
