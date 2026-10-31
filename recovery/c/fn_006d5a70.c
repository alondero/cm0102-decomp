/* Preferred-image VA 0x006d5a70, base 0x00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_THISCALL cm_fn_00714d30(u32 self);
extern u8 CM_THISCALL cm_fn_0070df40(u32 self);
extern u8 CM_FASTCALL cm_fn_0070df20(u32 self);
extern u8 CM_THISCALL cm_fn_006bcbd0(u32 self, u32 entry, volatile u32 *argument, u32 zero);

u32 CM_THISCALL cm_fn_006d5a70(u32 self, volatile u32 argument)
{
    s32 group = (s8)cm_fn_00714d30(argument);
    u32 entry = self + (u32)(group * 8920) + 0x4796u;
    u32 index;
    for (index = 0; index < 20; ++index, entry += 0x1beu) {
        if ((s8)B(entry + 0x19u) >= 0 && cm_fn_0070df40(entry) &&
            !cm_fn_0070df20(entry) && !cm_fn_006bcbd0(self, entry, &argument, 0))
            return entry;
    }
    return 0;
}
