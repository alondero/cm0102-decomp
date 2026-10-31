#include "cmre.h"

typedef u32 (CM_THISCALL *method_fn)(u32 self, u32 first, u32 second, u32 third, u32 fourth);

u32 CM_THISCALL cm_fn_00858e70(u32 self)
{
    u32 table = *(volatile u32 *)self;
    method_fn method = (method_fn)*(volatile u32 *)(table + 0x3cu);
    u32 result = method(self, 0xffffffffu, self + 0x3cu, self + 0x3au, 0);
    *(volatile u32 *)(self + 0xa3u) = result;
    return 1;
}
