#include "cmre.h"

/* fn_007e5b10: cdecl, two stack arguments (u32 pointer, byte flag; ret 0), no ECX read
   before write. Preferred-image VA 0x007e5b10, image base 0x00400000.
   Object fields: 0x00 first dword (passed as the third argument of the 0x007ff270 call),
   0x39 pointer whose first dword is passed as the third argument of the last call.
   The byte-map at 0x007e5bcc (49 bytes) and the DWORD target table at 0x007e5bc0 are
   read from the image as the original dispatch does. */
extern u8 CM_THISCALL cm_fn_0062ed30(int *param_1, int *param_2);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 param_2, u32 param_3, u32 param_4, u32 param_5, u32 param_6);
extern void CM_THISCALL cm_fn_007ff270(int param_1, int param_2, char *param_3, int param_4);

void cm_fn_007e5b10(u32 *param_1, u8 param_2)
{
    u8 r;
    u8 bl;
    s32 v;
    u32 t;
    u8 idx;
    u32 target;
    u32 edx;

    if (param_1 == 0) {
        return;
    }
    if (*(volatile u32 *)((u8 *)param_1 + 0x39) == 0u) {
        return;
    }
    if (cm_fn_007fe600(0x00b74450u, 0x007e5c00u, 0x007e5e30u, 1u, 0x005a8f60u, 0u) == 0u) {
        return;
    }

    if (param_2 == 0) {
        r = cm_fn_0062ed30((int *)CM_GLOBAL(u32, 0x00b63d28), (int *)param_1);
        v = (s32)(s8)r;
        t = (u32)(v - 11);
        if (t > 0x30u) {
            bl = 0x4d;
        } else {
            /* byte map at 0x007e5bcc, then the DWORD target at 0x007e5bc0 + idx*4:
               0x007e5b7c -> 'N', 0x007e5b80 -> 'O', 0x007e5b84 -> 'M'. */
            idx = CM_GLOBAL(u8, 0x007e5bccu + t);
            target = *(volatile u32 *)(0x007e5bc0u + (u32)idx * 4u);
            if (target == 0x007e5b7cu) {
                bl = 0x4e;
            } else if (target == 0x007e5b80u) {
                bl = 0x4f;
            } else {
                bl = 0x4d;
            }
        }
    } else {
        bl = 0x51;
    }

    edx = *(volatile u32 *)param_1;
    cm_fn_007ff270(0x00b74450, 0, (char *)edx, 0);
    cm_fn_007ff270(0x00b74450, 1, (char *)(s32)(s8)bl, 0);
    edx = *(volatile u32 *)(*(volatile u32 *)((u8 *)param_1 + 0x39));
    cm_fn_007ff270(0x00b74450, 2, (char *)edx, 0);
}
