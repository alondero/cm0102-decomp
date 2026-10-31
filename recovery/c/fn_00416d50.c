#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00418140(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg1, u32 arg2,
                                      u32 arg3, u32 arg4, u32 arg5);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 arg1, u32 arg2,
                                       u32 arg3);
extern void CM_CDECL cm_fn_005e8290(const char *message, const char *buffer,
                                    u32 flags);
extern char *CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 message, u32 arg2, u32 arg3,
                                   void *buffer1, void *buffer2);

void CM_CDECL cm_fn_00416d50(void *input)
{
    volatile u8 scratch[0x200];
    u32 input_value = (u32)input;

    if (input_value == 0u) {
        void *buffer0 = (void *)&scratch[0];
        void *buffer100 = (void *)&scratch[0x100];
        u32 formatted;

        (void)cm_fn_00944cff(0x00987fa4u, 0u, 0u, buffer100, buffer0);
        formatted = (u32)cm_fn_0090d130();
        (void)cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, formatted,
                             buffer100, buffer0, 0x3c4u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0u);
        CM_GLOBAL(volatile u32, 0x00b67a34u) = 0u;
        return;
    }

    {
        s32 index = (s32)(s16)CM_GLOBAL(volatile s16, 0x00b77c7au);
        u32 table_value;
        u32 object_value;
        u32 slot_result;
        u32 matched;

        table_value = CM_GLOBAL(volatile u32, 0x00b74c26u +
                                (u32)index * 0x300u);
        object_value = *(volatile u32 *)(input_value + 0x1fu);
        slot_result = cm_fn_00418140(object_value, table_value);
        matched = ((u16)slot_result == 0xffffu) ? 1u : 0u;

        slot_result = cm_fn_007fe600(0x00b74450u, 0x00416e40u,
                                     0x004172f0u, matched, 0u, 0u);
        if (slot_result != 0u) {
            cm_fn_007ff270(0x00b74450u, 0u, input_value, 0u);
            cm_fn_007ff270(0x00b74450u, 1u, matched, 0u);
        }
    }
}
