#include "cmre.h"

extern void CM_CDECL cm_fn_0053dba0(int param_1, char *buffer, short length, u8 mode);

u32 CM_CDECL
cm_fn_0084b8b0(u32 param_1, const u8 *param_2)
{
    u8 scratch[0x1f4];
    volatile u8 *local;
    u8 left;
    u8 right;
    u8 left_next;
    u8 right_next;


    if (param_1 == 0)
        return 0;
    cm_fn_0053dba0((int)param_1, (char *)scratch, 0x1f4, 1);
    local = scratch;
    for (;;) {
        left = *local;
        right = *(volatile const u8 *)param_2;
        if (left != right)
            return 0;
        if (left == 0)
            return 1;
        left_next = local[1];
        right_next = *(volatile const u8 *)(param_2 + 1);
        if (left_next != right_next)
            return 0;
        local += 2;
        param_2 += 2;
        if (left_next == 0)
            return 1;
    }
}

