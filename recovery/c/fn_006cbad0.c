#include "cmre.h"
extern u8 CM_FASTCALL cm_fn_00714d30(u32 item);
u8 CM_THISCALL cm_fn_006cbad0(u32 self, u32 item)
{
    u32 itemptr = item;
    s8 b = *(volatile s8 *)(itemptr + 0x102u);
    u8 *arg_slot = (u8 *)&item;
    u8 first = cm_fn_00714d30(itemptr);
    *arg_slot = (u8)(first == 0u);
    u8 second = cm_fn_00714d30(itemptr);
    s8 a = *(volatile s8 *)(itemptr + 0x103u);
    if (second == 1u) { a = (s8)(u8)((u8)a - 1u); if (a < 0) return 0u; }
    else { a = (s8)(u8)((u8)a + 1u); if (a > 11) return 0u; }
    s32 ix = (s32)a + (s32)b * 12;
    u32 off = (u32)((s32)ix * 90);
    u32 flag = *arg_slot;
    if (b < 8 && *(volatile u8 *)(self + off + flag + 0x25eeu) == 0u) return 1u;
    if (b > 0 && *(volatile u8 *)(self + off + flag + 0x1d7eu) == 0u) return 1u;
    return (u8)(*(volatile u8 *)(self + off + flag + 0x21b6u) == 0u);
}
