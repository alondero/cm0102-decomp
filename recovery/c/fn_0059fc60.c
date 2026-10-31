#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(u8 *self, u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_00785790(u32 *self, u32 index, u32 value);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 divisor);
extern unsigned long long CM_THISCALL cm_fn_007861e0(u32 *self, u32 first, u32 second);

u32 CM_STDCALL cm_fn_0059fc60(u32 input)
{
    struct {
        u32 previous;
        u32 handler;
        u32 scope;
    } volatile frame;
    u8 local_area[0xe0];
    u32 *buffer = (u32 *)local_area;
    s32 converted;
    u32 saved_argument;
    u32 result;

    frame.scope = 0xffffffffu;
    frame.handler = 0x0095910bu;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0077bbe0((u8 *)buffer, 0x177bu, 0);
    saved_argument = input;
    frame.scope = 0;
    (void)cm_fn_00785790(buffer, 0, *(volatile u32 *)saved_argument);

    converted = cm_fn_0090cfc0(2);
    (void)cm_fn_00785790(buffer, 1, (u32)(s32)(s16)converted);

    {
        u32 context = CM_GLOBAL(volatile u32, 0x00b7431c);
        result = (u32)cm_fn_007861e0((u32 *)context, (u32)buffer, saved_argument);
    }

    CM_FS(u32, 0) = frame.previous;
    return result;
}
