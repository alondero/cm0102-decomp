/* Faithful C recovery at preferred-image VA 004ce450. */
#include "cmre.h"

extern void CM_FASTCALL cm_fn_0085e410(u16 *scratch);
extern s32 CM_THISCALL cm_fn_0085fc50(u16 *scratch,
    u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6, u32 arg7);

s32 CM_STDCALL cm_fn_004ce450(u32 first, u32 second)
{
    volatile struct seh_registration {
        u32 prev;
        u32 handler;
        u32 state;
    } frame;
    u32 scratch[20];

    frame.state = 0xffffffffu;
    frame.handler = 0x00956d88u;
    frame.prev = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0085e410((u16 *)scratch);
    frame.state = 0;
    s32 first_value = cm_fn_0085fc50((u16 *)scratch,
        first, second, 0xffffffffu, 1u, 0xbu, 0u, 0u);
    s32 minimum = 0x00989680;
    if (first_value < minimum)
        minimum = first_value;

    s32 second_value = cm_fn_0085fc50((u16 *)scratch,
        first, second, 0xffffffffu, 2u, 0xbu, 0u, 0u);
    s32 result = second_value;
    if (result >= minimum)
        result = minimum;

    u32 previous = frame.prev;
    CM_FS(u32, 0) = previous;
    return result;
}
