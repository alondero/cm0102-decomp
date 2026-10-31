/* Preferred-image VA 0x00804e10, base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   thiscall, ret 8, explicit EAX result 0 or 1; no direct callers.
   Each receiver calculation reloads the signed byte at self. */
#include "cmre.h"

extern u32 CM_THISCALL cm_fn_007ae780(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_007ae7d0(u32 self, u32 value);
extern u32 CM_THISCALL cm_fn_007b3660(u32 self, u32 item, u32 flag);
extern u32 CM_THISCALL cm_fn_005fcb60(u32 self, u32 item, u32 zero, u32 mode);

u32 CM_THISCALL cm_fn_00804e10(u32 self, u32 item, u32 value)
{
    u32 receiver;
    u32 result;
    receiver = self + 1u + (u32)(s32)*(volatile s8 *)self * 0x2b0u;
    cm_fn_007ae780(receiver, 0);
    if (value != 0) {
        receiver = self + 1u + (u32)(s32)*(volatile s8 *)self * 0x2b0u;
        cm_fn_007ae7d0(receiver, value);
        receiver = self + 1u + (u32)(s32)*(volatile s8 *)self * 0x2b0u;
        result = cm_fn_007b3660(receiver, item, 1);
        receiver = self + 1u + (u32)(s32)*(volatile s8 *)self * 0x2b0u;
        cm_fn_007ae7d0(receiver, 0);
    } else {
        receiver = self + 1u + (u32)(s32)*(volatile s8 *)self * 0x2b0u;
        result = cm_fn_007b3660(receiver, item, 1);
    }
    if (result == 0)
        return 0;
    *(volatile u32 *)(self + 0x361au) = 0;
    receiver = CM_GLOBAL(volatile u32, 0x00b63c98);
    cm_fn_005fcb60(receiver, item, 0, 2);
    return 1;
}
