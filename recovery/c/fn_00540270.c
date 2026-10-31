/* Preferred-image VA 0x00540270, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_0053dba0(u32 person, u32 output, u32 capacity, u32 selector);
extern s32 CM_CDECL cm_fn_0090c1d0(volatile u8 *left, volatile u8 *right, s32 mode, u32 count);

s32 CM_CDECL cm_fn_00540270(volatile u32 *left, volatile u32 *right)
{
    u8 buffers[4000];
    u32 first, second;
    buffers[2000] = 0;
    first = *left;
    second = *right;
    if (first)
        cm_fn_0053dba0(first, (u32)(buffers + 2000), 2000, 1);
    buffers[0] = 0;
    if (second)
        cm_fn_0053dba0(second, (u32)buffers, 2000, 1);
    return cm_fn_0090c1d0(buffers + 2000, buffers, 1, 0);
}
