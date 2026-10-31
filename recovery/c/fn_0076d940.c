#include "cmre.h"

/* One stack argument (ret 4): a pointer to a u32 value. A null pointer returns 0
   without a read. Otherwise the value is compared for equality against the
   globals below, in the listing's order, stopping at the first match. */
#define GLOBAL_U32(va) CM_GLOBAL(volatile u32, (va))

u32 CM_STDCALL cm_fn_0076d940(u32 arg1)
{
    u32 value;

    if (arg1 == 0) {
        return 0u;
    }

    value = *(volatile u32 *)arg1;
    if (value == GLOBAL_U32(0x009cf79c) ||
        value == GLOBAL_U32(0x009cf7a0) ||
        value == GLOBAL_U32(0x009cf7a4) ||
        value == GLOBAL_U32(0x009cf7a8) ||
        value == GLOBAL_U32(0x009cf7ac) ||
        value == GLOBAL_U32(0x009cf790) ||
        value == GLOBAL_U32(0x009cf8a0) ||
        value == GLOBAL_U32(0x009cf78c) ||
        value == GLOBAL_U32(0x009cf964)) {
        return 1u;
    }
    return 0u;
}
