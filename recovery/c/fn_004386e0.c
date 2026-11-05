/* CM0102 3.9.68, preferred-image VA 004386e0, base 00400000.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original callees remain explicit boundaries. 00438760 consumes the option
   as BL and forwards it to 0068b100's byte option (0068b1d6). */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u32 CM_THISCALL cm_fn_00685d30(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_00438760(u32, u32);

u32 CM_THISCALL cm_fn_004386e0(u32 self, u32 record, u32 option)
{
    cm_fn_00685d30(self, record, option);
    if (B(record + 0x42) == 0xff) {
        u8 category = (s16)W(record + 0x34) >= 11;
        B(record + 0x42) = category;
        if ((s32)D(self + 0x30) < (s32)category) {
            s32 required;
            do {
                u8 next = (u8)(B(self + 0x30) + 1u);
                cm_fn_00438760(self, next);
                required = (s32)(s8)B(record + 0x42);
            } while ((s32)D(self + 0x30) < required);
        }
        u32 table = D(self + 0xc);
        u32 saved = D(self + 8);
        D(self + 8) = 0;
        u32 index = (u32)(s32)(s8)B(record + 0x42);
        u32 child = D(table + index * 4u);
        u32 vtable = D(child);
        u32 target = D(vtable + 4);
        ((u32 (CM_THISCALL *)(u32, u32, u32))target)(child, record, option);
        D(self + 8) = saved;
        B(record + 0x42) = 0xff;
    }
    return 1;
}
