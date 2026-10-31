/* Preferred-image VA 0x0084cc80, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
typedef u32 (__attribute__((stdcall, regparm(3))) *callback_84cc80)(u32, u32, u32, u32, u32);
u32 CM_THISCALL cm_fn_0084cc80(u32 self, volatile u32 index_arg)
{
    u32 index, element, table;
    if (!*(volatile u32 *)(self + 0x28u)) return 0;
    index = (u8)index_arg;
    if ((s8)index < 0 || (s8)index >= 10) return 0;
    element = *(volatile u32 *)(self + index * 4u);
    if (!element) return 0;
    table = *(volatile u32 *)element;
    ((callback_84cc80)*(volatile u32 *)(table + 0x24u))(
        element, (u32)&index_arg, table, element, (u32)&index_arg);
    return (index_arg & 5u) ? 1u : 0u;
}
