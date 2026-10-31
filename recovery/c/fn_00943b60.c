#include "cmre.h"

extern u32 CM_CDECL cm_fn_009443a0(u32 owner, u32 size);
extern u32 CM_CDECL cm_fn_00941580(u32 input);

u32 CM_CDECL cm_fn_00943b60(u32 owner, u32 input)
{
    u32 output = cm_fn_009443a0(owner, 0x14);
    u32 value;
    *(volatile u32 *)output = owner;
    value = cm_fn_00941580(input);
    *(volatile u32 *)(output + 4) = value;
    value = cm_fn_00941580(input);
    *(volatile u32 *)(output + 8) = value;
    value = cm_fn_00941580(input);
    *(volatile u32 *)(output + 0xc) = value;
    value = cm_fn_00941580(input);
    *(volatile u32 *)(output + 0x10) = value;
    return output;
}
