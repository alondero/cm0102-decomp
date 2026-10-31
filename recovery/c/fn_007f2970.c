#include "cmre.h"

u32 CM_THISCALL cm_fn_007f2970(volatile u8 *self)
{
    volatile u32 *vtable;
    u32 global_value;
    u32 result;
    u16 year;

    year = *(volatile u16 *)(self + 0x40);
    if (year == 0x07cfu) {
        *(volatile u16 *)(self + 0x3c) = 4;
        self[0xbe] = 0;
        self[0xbf] = 0;
        self[0xc0] = 1;
        self[0xc1] = 0;
        *(volatile u16 *)(self + 0xa7) = 0xffff;
    } else {
        *(volatile u16 *)(self + 0x3c) = 3;
        self[0xbe] = 0;
        self[0xbf] = 0;
        self[0xc0] = 0;
        self[0xc1] = 1;
        *(volatile u16 *)(self + 0xa7) = 0x26;
    }
    *(volatile u32 *)(self + 0xa3) = 0;
    self[0xc2] = 3;
    self[0xc3] = 1;
    self[0xc4] = 2;
    *(volatile u16 *)(self + 0xab) = 0;
    *(volatile u16 *)(self + 0xdb) = 0;
    self[0x42] = 1;
    self[0xc5] = 1;
    self[0xc6] = 2;
    self[0xc7] = 3;
    *(volatile u32 *)(self + 0x1c) = 0xffffffffu;
    global_value = CM_GLOBAL(volatile u32, 0x009cf5e0);
    *(volatile u32 *)(self + 0x20) = global_value;
    self[0x4a] = 3;
    vtable = *(volatile u32 * volatile *)self;
    self[0x52] = 2;
    self[0x49] = 5;
    result = ((u32 (CM_THISCALL *)(volatile u8 *, u32, volatile u8 *,
                                  volatile u8 *, u32))vtable[0x3c / 4])(
        self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
    year = *(volatile u16 *)(self + 0x40);
    *(volatile u32 *)(self + 0xba) = result;
    if (year != 0x07cfu)
        *(volatile u16 *)(self + 0xa9) = 0x21;
    return 1;
}
