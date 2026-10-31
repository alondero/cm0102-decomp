#include "cmre.h"

/* SEH registration record linked at fs:[0]. Offsets follow the listing: the
   state word sits at the top of the frame, then the handler, then prev. */
struct cm_seh_frame {
    u32 prev;
    u32 handler;
    u32 state;
};

extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 arg1, u32 arg2);
extern unsigned long long CM_THISCALL cm_fn_00785830(void *self, u32 index);
extern u32 CM_THISCALL cm_fn_00786b70(u32 self, u32 param_1, u32 param_2, u32 param_3);
extern u32 CM_THISCALL cm_fn_008c61e0(u32 self, u32 param_2);
extern u32 CM_CDECL cm_fn_008e9350(u32 param_1, u32 param_2);

u32 CM_CDECL cm_fn_008f1b40(u32 param_1, u32 param_2)
{
    volatile struct cm_seh_frame frame;
    u8 local_ec[0xe0];
    u32 result_index_b;
    u32 result_index_c;
    u32 object_slot;
    u32 object;
    u32 field24;
    u8 flag;
    u32 second_arg;
    u32 field0c;
    u32 table_base;
    u32 result;

    frame.state = 0xffffffffu;
    frame.handler = 0x00964d1bu;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)(unsigned long)&frame;

    cm_fn_0077bbe0((void *)local_ec, 0u, 0u);
    frame.state = 0u;

    cm_fn_00786b70(CM_GLOBAL(volatile u32, 0x00b7431c), param_1, param_2,
                   (u32)(unsigned long)local_ec);
    result_index_b = (u32)cm_fn_00785830((void *)local_ec, 0xbu);
    result_index_c = (u32)cm_fn_00785830((void *)local_ec, 0xcu);

    object_slot = cm_fn_008c61e0(CM_GLOBAL(volatile u32, 0x00de1f54), result_index_b);

    object = *(volatile u32 *)(unsigned long)object_slot;
    field24 = *(volatile u32 *)(unsigned long)(object + 0x24u);
    second_arg = 0u;
    if (field24 == result_index_c) {
        flag = *(volatile u8 *)(unsigned long)(object + 0x2cu);
        if (flag != 3u && flag != 2u) {
            second_arg = object_slot;
        }
    }

    field0c = *(volatile u32 *)(unsigned long)(object + 0x0cu);
    table_base = CM_GLOBAL(volatile u32, 0x00ae23bc);
    result = cm_fn_008e9350(table_base + field0c * 0x6eu, second_arg);

    CM_FS(u32, 0) = frame.prev;
    return result;
}
