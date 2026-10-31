#include "cmre.h"

extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, u32 *output, u32 days);

u32 CM_THISCALL cm_fn_00774f90(volatile u32 *self, volatile u32 *first,
                               volatile u32 *second)
{
    u32 list = *self;
    u32 node;
    u32 date[2];
    if (list) {
        *(volatile u32 *)(list + 0xa) = *(volatile u32 *)(list + 2);
        node = cm_fn_0068d1b0(*self);
        while (node) {
            if (*(volatile u32 *)(node + 4) == *second &&
                *(volatile u32 *)node == *first) {
                u32 result = cm_fn_0054c5b0(0xae2c90, date, 1);
                *(volatile u16 *)(node + 0x10) = *(volatile u16 *)result;
            }
            node = cm_fn_0068d1b0(*self);
        }
    }
    return 0;
}
