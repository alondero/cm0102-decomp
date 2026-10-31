#include "cmre.h"

extern u32 CM_THISCALL cm_fn_006847c0(int *self, short *date, int flag);
extern void CM_FASTCALL cm_fn_0092e7f0(int *self);
extern void CM_FASTCALL cm_fn_0092f230(int self);

u32 CM_THISCALL cm_fn_0092f1a0(const volatile u8 *self,
                               const volatile s16 *date, s32 flag)
{
    u16 first_object_date = *(const volatile u16 *)(self + 0x18e);
    u16 first_input_date = *(const volatile u16 *)date;
    if (first_object_date == first_input_date) {
        s32 start = *(const volatile s16 *)(self + 0x190);
        s32 offset = *(const volatile s16 *)(self + 0x40);
        u16 other = *(const volatile u16 *)((const volatile u8 *)date + 2);
        if ((u32)(start + offset) == (u32)other && flag != 0) {
            cm_fn_0092f230((int)self);
        }
    }

    u16 second_object_date = *(const volatile u16 *)(self + 0xb6);
    u16 second_input_date = *(const volatile u16 *)date;
    if (second_object_date == second_input_date) {
        s32 start = *(const volatile s16 *)(self + 0xb8);
        s32 offset = *(const volatile s16 *)(self + 0x40);
        u16 other = *(const volatile u16 *)((const volatile u8 *)date + 2);
        if ((u32)(start + offset) == (u32)other && flag != 0) {
            if (*(const volatile u32 *)(self + 0xb1) == 0) {
                cm_fn_0092e7f0((int *)(u32)self);
            }
        }
    }

    return cm_fn_006847c0((int *)self, (short *)date, flag);
}
