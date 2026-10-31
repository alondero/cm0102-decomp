#include "cmre.h"

extern s32 CM_CDECL cm_fn_009349a0(u32 first, u32 flag, u32 target,
    u32 zero, s32 distance);
extern void CM_CDECL cm_fn_00934960(u32 first, u32 second, s32 result,
    s32 index_a, s32 index_b, u32 selector);

typedef void (CM_CDECL *fallback_fn)(u32 first, u32 second, u32 minus_one,
    u32 one, u32 value6, u32 value7, u32 value8, u32 value9,
    s32 index_a, s32 index_b);

void CM_CDECL cm_fn_00933da0(u32 first, u32 second, u16 *values,
    u32 selector, u32 value5, u32 value6, u32 value7, u32 value8,
    u32 value9, s32 index_a, s32 index_b)
{
    u32 address_b = (u32)values + (u32)index_b * 2u;
    u32 address_a = (u32)values + (u32)index_a * 2u;
    s32 at_a = *(volatile s16 *)address_a;
    s32 at_b = *(volatile s16 *)address_b;
    s32 delta = at_b - at_a;
    if (delta < 0)
        delta = -delta;

    s32 result = cm_fn_009349a0(first, selector, value5, 0, delta);
    if ((s16)result >= 0) {
        cm_fn_00934960(first, second, result, index_a, index_b, selector);
        return;
    }

    fallback_fn target = (fallback_fn)((u16)selector != 0
        ? 0x009342c0u : 0x00934380u);
    target(first, second, 0xffffffffu, 1u,
        value6, value7, value8, value9, (u32)index_a, (u32)index_b);
}
