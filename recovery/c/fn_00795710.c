/* Preferred-image VA 0x00795710, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00795660(u32 self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_00795550(u8 *self, int *first, u32 *second, char *third, u32 fourth, u32 fifth, u8 sixth);
void CM_THISCALL cm_fn_00795710(u32 self, u32 first, u32 second, u32 third, u32 fourth, u32 fifth, u32 sixth)
{
    cm_fn_00795660(self, first, second);
    cm_fn_00795550((u8 *)self, (int *)first, (u32 *)second, (char *)third, fourth, fifth, (u8)sixth);
}
