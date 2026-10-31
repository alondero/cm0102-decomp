#include "cmre.h"

extern u32 CM_CDECL cm_fn_005b7d20(u32 record);
extern s32 CM_STDCALL cm_fn_0084b230(u32 first, u32 second, s8 mode);

s32 CM_THISCALL cm_fn_0084b1f0(u32 self, volatile u32 first,
                               volatile u8 *record, volatile u32 mode)
{
    u32 nested, value, saved_mode, saved_first;
    (void)self;
    if (!record)
        return -1;
    nested = *(volatile u32 *)(record + 0x61);
    if (!nested)
        return -1;
    value = cm_fn_005b7d20(nested);
    saved_mode = mode;
    saved_first = first;
    return cm_fn_0084b230(saved_first, value, (s8)saved_mode);
}
