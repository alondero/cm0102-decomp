#include "cmre.h"

extern void CM_THISCALL cm_fn_0077bbe0(u8 *self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_00786b70(u32 *self, u32 first, u32 second, u32 third);
extern u32 CM_THISCALL cm_fn_00785830(u8 *self, u32 index);
extern void CM_CDECL cm_fn_004dd0f0(u32 first, u32 second, u32 third, u32 fourth);

void CM_CDECL cm_fn_004dc9b0(u32 first_arg, u32 second_arg)
{
    struct {
        u32 previous;
        u32 handler;
        u32 scope;
    } volatile frame;
    u8 local_area[0xe0];
    u32 index;
    u32 index5;
    u32 index11;
    u32 index55;
    u32 index110;
    u32 base;

    frame.scope = 0xffffffffu;
    frame.handler = 0x0095709bu;
    frame.previous = CM_FS(u32, 0);
    CM_FS(u32, 0) = (u32)&frame;

    cm_fn_0077bbe0(local_area, 0, 0);
    frame.scope = 0;

    {
        u32 context = CM_GLOBAL(volatile u32, 0x00b7431c);
        (void)cm_fn_00786b70((u32 *)context, first_arg, second_arg, (u32)local_area);
    }
    index = cm_fn_00785830(local_area, 2);
    index5 = index * 5u;
    index11 = index + index5 * 2u;
    index55 = index11 * 5u;
    index110 = index55 * 2u;
    base = CM_GLOBAL(volatile u32, 0x00ae23bc);
    cm_fn_004dd0f0(base + index110, 0, 0, 1);

    CM_FS(u32, 0) = frame.previous;
}
