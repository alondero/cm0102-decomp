#include "cmre.h"

extern void CM_CDECL cm_fn_00945333(u32 *arg1, u32 arg2, u32 arg3, void *arg4);
extern void CM_FASTCALL cm_fn_0068d080(u16 *self);
extern u32 CM_THISCALL cm_fn_0062c400(s32 *self, s32 *arg);
extern u32 CM_THISCALL cm_fn_0068d0d0(s16 *self, u32 arg1, u32 arg2);

u8 CM_THISCALL cm_fn_008b5000(u8 *self, u32 arg1, u32 arg2, u32 arg3)
{
    volatile u8 count = 0;
    u32 *rows = (u32 *)arg2;
    u32 *second = (u32 *)arg1;

    if (arg3 != 0)
        cm_fn_00945333(rows, 0x32u, 5u, (void *)arg3);

    if (arg1 != 0)
        cm_fn_0068d080((u16 *)arg1);

    s32 remaining = 0x32;
    do {
        u32 first = *(volatile u32 *)rows;
        if (first != 0) {
            s32 row_value = *(volatile s8 *)((u8 *)rows + 4);
            s32 self_value = *(volatile s32 *)self;
            if (row_value == self_value) {
                if (arg1 != 0)
                    (void)cm_fn_0068d0d0((s16 *)arg1, (u32)rows, 5u);
                count++;
            } else {
                u8 flag = *(volatile u8 *)(self + 0x15);
                if (flag == 0xffu) {
                    if (arg1 != 0)
                        (void)cm_fn_0068d0d0((s16 *)arg1, (u32)rows, 5u);
                    count++;
                } else if (flag == 0) {
                    u32 selector = CM_GLOBAL(volatile u32, 0x00b63d28u);
                    if ((u8)cm_fn_0062c400((s32 *)selector, (s32 *)first) == 2u) {
                        if (arg1 != 0)
                            (void)cm_fn_0068d0d0((s16 *)arg1, (u32)rows, 5u);
                        count++;
                    }
                }
            }
        }
        rows = (u32 *)((u32)rows + 5u);
    } while (--remaining != 0);
    (void)second;
    return count;
}
