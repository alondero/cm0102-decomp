#include "cmre.h"
extern u32 CM_THISCALL cm_fn_00767fa0(u32 self, u32 person);

u32 CM_THISCALL cm_fn_00767ee0(u32 self, u32 unused, u32 input)
{
    u32 pass, base = input + 0x190;
    (void)unused;
    for (pass = 0; pass < 2; ++pass) {
        u32 i, record = base;
        for (i = 0; i < 20; ++i) {
            if (*(volatile u8 *)(record + 0x1d) != 0xfb &&
                (i < 11 || *(volatile u8 *)(record + 0x32) != 0xff)) {
                u32 index = *(volatile u32 *)record;
                u32 person = CM_GLOBAL(volatile u32, 0x00ae23bc) + index * 110;
                if (!person || *(volatile s32 *)person < CM_GLOBAL(volatile s32, 0x00ae2364))
                    cm_fn_00767fa0(self, record);
            }
            record += 0x49;
        }
        base += 0x625;
    }
    return 0;
}
