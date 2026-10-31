#include "cmre.h"

extern int CM_THISCALL cm_fn_0054cdb0(u32 *date, u32 first, u32 second);
extern void CM_THISCALL cm_fn_00417c10(u8 *self, u32 *output, u32 *source);
extern u32 CM_STDCALL cm_fn_005b06e0(u32 first, u32 second);

void CM_THISCALL cm_fn_005b0260(u8 *self)
{
    s32 initial_count = CM_GLOBAL(volatile s32, 0x00ae2364);
    u32 ebx_value = CM_GLOBAL(volatile u32, 0x00ae23bc);
    s32 ebp_value = 0;
    u32 edi_value = 0;
    volatile u32 local_output[2];

    if (initial_count > 0) {
        do {
            u32 base = *(volatile u32 *)self;
            u32 row = base + edi_value;
            u8 current = *(volatile u8 *)row;

            if (current != 0) {
                u32 arg_at_5 = *(volatile u32 *)(row + 5u);
                u32 arg_at_1 = *(volatile u32 *)(row + 1u);
                int amount = cm_fn_0054cdb0((u32 *)0x00ae2c90, arg_at_1, arg_at_5);

                if (amount > 0x21c) {
                    base = *(volatile u32 *)self;
                    u8 *helper_self = (u8 *)(base + edi_value + 1u);
                    cm_fn_00417c10(helper_self, (u32 *)&local_output[0], (u32 *)0x00ae2c90);
                    base = *(volatile u32 *)self;
                    row = base + edi_value;
                    current = *(volatile u8 *)row;
                    *(volatile u8 *)row = (u8)(current - 1u);
                }
            }

            {
                u32 first = *(volatile u32 *)(self + 4);
                u32 result = cm_fn_005b06e0(first, ebx_value);
                base = *(volatile u32 *)self;
                row = base + edi_value;
                current = *(volatile u8 *)row;
                if ((s8)current < (s8)result)
                    *(volatile u8 *)row = (u8)result;
            }

            initial_count = CM_GLOBAL(volatile s32, 0x00ae2364);
            ebp_value = (s32)((u32)ebp_value + 1u);
            edi_value += 9u;
            ebx_value += 0x6eu;
        } while (ebp_value < initial_count);
    }
}
