#include "cmre.h"

/* Preferred-image VA 0x006e9540, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Ret 0x10; three direct callers observe AL. The fourth forwards EAX through
   0x006cfca0, whose three direct callers also observe only AL. */
extern u8 CM_THISCALL cm_fn_006bab90(u8 *object);
extern u32 CM_THISCALL cm_fn_006c38c0(u32 object, u32 player);
extern u32 CM_THISCALL cm_fn_006f5b20(u32 object, u8 kind);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 limit);

#define D(a) (*(volatile u32 *)(u32)(a))
#define W(a) (*(volatile u16 *)(u32)(a))
#define B(a) (*(volatile u8 *)(u32)(a))
#define S(a) (*(volatile s8 *)(u32)(a))

u8 CM_THISCALL cm_fn_006e9540(u32 self, u32 player, u16 kind,
                             u32 output_kind, u32 output_value)
{
    u32 object, result;
    s32 limit;
    u8 value;

    object = D(self);
    if (cm_fn_006bab90((u8 *)object) != 0)
        return 0;
    object = D(self);
    result = cm_fn_006c38c0(object, player);
    if ((result & 1u) != 0) {
        value = B(player + 0x27u);
        if ((u8)cm_fn_006f5b20(player, value) != 0)
            return 0;
    }
    object = D(self + 4u);
    if (object != 0) {
        limit = (s32)S(object + 0x28u) + 15;
        if (cm_fn_0090cfc0(limit) == 0) {
            W(self + 8u) = (u16)(W(self + 8u) + 0xff06u);
            return 0;
        }
    }
    value = B(player + 0x27u);
    if ((u8)cm_fn_006f5b20(player, value) != 0) {
        if (cm_fn_0090cfc0(4) != 0)
            return 0;
    }
    W(self + 8u) = (u16)(W(self + 8u) + 0xafu);
    if (kind == 0x25u) {
        if (cm_fn_0090cfc0(20) == 0)
            return 0;
    } else {
        if (cm_fn_0090cfc0(155) != 0)
            return 0;
        object = D(self);
        if (S(object + 0x8ea9u) <= 1)
            return 0;
    }
    value = B(player + 0x14u);
    result = (u32)cm_fn_0090cfc0(value);
    B(output_value) = (u8)(result + 1u);
    B(output_kind) = 4;
    return 1;
}
