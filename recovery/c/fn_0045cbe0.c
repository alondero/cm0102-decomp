/* Preferred-image VA 0045cbe0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
extern u32 CM_THISCALL cm_fn_0062c400(u32 self, u32 player);
extern u32 CM_THISCALL cm_fn_004cf6d0(u32 self, u32 player);
extern u32 CM_THISCALL cm_fn_004cf5a0(u32 self, u32 player);
extern u32 CM_THISCALL cm_fn_008cf5e0(u32 self, u32 player, u32 object);
extern u32 CM_THISCALL cm_fn_007757d0(u32 self, u32 player, u32 fourth, u32 third, u32 zero1, u32 zero2);
extern u32 CM_THISCALL cm_fn_0055d270(u32 self, u32 player, u32 value, u32 one, u32 zero);
extern u32 CM_THISCALL cm_fn_008c7560(u32 self, u32 player, u32 third, u32 zero);

u32 CM_CDECL cm_fn_0045cbe0(u32 player, u32 object, u32 third, u32 fourth)
{
    u32 receiver, result, left, right, value;
    if (player == 0)
        return 0;
    if (object == 0)
        return 1;
    if (third == 0 || fourth == 0)
        return 0;
    receiver = D(0x00b63d28u);
    result = cm_fn_0062c400(receiver, player);
    if ((u8)result == 2)
        return 0;
    receiver = D(0x00ae1050u);
    result = cm_fn_004cf6d0(receiver, player);
    if (result != 0) {
        receiver = D(0x00ae1050u);
        result = cm_fn_004cf5a0(receiver, player);
        if (result != 0) {
            receiver = D(0x00ae1050u);
            result = cm_fn_004cf5a0(receiver, player);
            left = D(result + 4u);
            right = D(third);
            if (left == right)
                return 0;
        }
    }
    receiver = D(0x00de1f54u);
    result = cm_fn_008cf5e0(receiver, player, object);
    if ((u8)result != 0)
        return 0;
    receiver = D(0x00b74318u);
    result = cm_fn_007757d0(receiver, player, fourth, third, 0, 0);
    if (result != 0)
        return 0;
    value = D(object + 0x14u);
    receiver = D(0x00ae28f0u);
    result = cm_fn_0055d270(receiver, player, value, 1, 0);
    if (result != 0)
        return 0;
    receiver = D(0x00de1f54u);
    result = cm_fn_008c7560(receiver, player, third, 0);
    return result == 0;
}
