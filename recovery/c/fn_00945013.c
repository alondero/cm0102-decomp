#include "cmre.h"

extern u32 CM_CDECL cm_fn_00949d5c(u32, u32, u32, u32, u32, u32, u32, u32);

u32 CM_CDECL cm_fn_00945013(u32 first, u32 record, u32 third)
{
    u32 field_10 = *(volatile u32 *)(record + 0x10u);
    u32 field_08 = *(volatile u32 *)(record + 0x08u);
    u32 field_0c = *(volatile u32 *)(record + 0x0cu);

    return cm_fn_00949d5c(first, field_0c, third, 0, field_08,
                          field_10, record, 0);
}
