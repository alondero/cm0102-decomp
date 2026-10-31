#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007e82e0(u32 self, u32 key, volatile u32 create);
typedef u32 (CM_THISCALL *method_fn)(u32 self, u32 found, u32 second, u32 third, u32 fourth);

u32 CM_THISCALL cm_fn_007e8d60(u32 self, u32 key, volatile u32 second,
                                volatile u32 third, volatile u32 fourth)
{
    u32 found = cm_fn_007e82e0(self, key, 0);
    u32 a, b, c, table;
    method_fn method;
    if (found == 0)
        return 0;
    c = fourth;
    table = *(volatile u32 *)self;
    b = third;
    a = second;
    method = (method_fn)*(volatile u32 *)(table + 0x14u);
    return method(self, found, a, b, c);
}
