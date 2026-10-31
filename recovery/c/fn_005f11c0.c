#include "cmre.h"

/* The object at 0x00b74450 is passed as ECX to every call. The first call's
   arguments are code-section addresses (0x005f1360, 0x005f1ba0, 0x005f1d60).
   Each 0x007ff270 call pushes (value, extra, index) so the callee sees
   (self, index, value, extra) in that order. */

#define CM_OBJECT_B74450 0x00b74450u

extern u32 CM_THISCALL cm_fn_007fe600(u32 self, u32 arg2, u32 arg3, u32 arg4, u32 arg5, u32 arg6);
extern void CM_THISCALL cm_fn_007ff270(u32 self, u32 index, u32 value, u32 extra);

void CM_CDECL cm_fn_005f11c0(u32 arg_1)
{
    u32 result;

    result = cm_fn_007fe600(CM_OBJECT_B74450, 0x005f1360u, 0x005f1ba0u, 1u, 0x005f1d60u, 0u);
    if (result == 0) {
        return;
    }
    cm_fn_007ff270(CM_OBJECT_B74450, 0u, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 1u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 2u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 3u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 4u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 5u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 6u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 7u, 0u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 8u, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 9u, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xau, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xbu, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xcu, arg_1, 1u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xdu, 0xffffffffu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xeu, 0x91u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0xfu, 0x4bu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x10u, 0x2e9u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x11u, 0x20fu, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x12u, 0x115u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x13u, 0x009cec18u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x14u, 0x101u, 0u);
    cm_fn_007ff270(CM_OBJECT_B74450, 0x15u, 0x14fu, 0u);
}
