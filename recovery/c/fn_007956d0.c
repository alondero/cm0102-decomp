#include "cmre.h"

extern u32 CM_THISCALL cm_fn_00795660(u32 self, u32 first, u32 second);
extern void CM_THISCALL cm_fn_007953d0(u32 self, u32 first, u32 second,
                                       u32 third, u32 fourth, u32 fifth);

void CM_THISCALL cm_fn_007956d0(u32 self, u32 first, u32 second,
                               volatile u32 third, volatile u32 fourth,
                               volatile u32 fifth)
{
    u32 a, b, c;
    cm_fn_00795660(self, first, second);
    c = fifth;
    b = fourth;
    a = third;
    cm_fn_007953d0(self, first, second, a, b, c);
}
