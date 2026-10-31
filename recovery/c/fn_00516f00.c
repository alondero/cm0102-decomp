#include "cmre.h"
typedef u32 (CM_THISCALL *slot_t)(void *, s32, void *, void *, u32);
u32 CM_THISCALL cm_fn_00516f00(void *object)
{
    volatile u8 *s = (volatile u8 *)object;
    u32 vtable, global_value, result;
    slot_t slot;
    *(volatile u16 *)(s + 0x3c) = 2;
    s[0xc2] = 3; s[0xc3] = 1; s[0xc4] = 2;
    *(volatile u16 *)(s + 0xab) = 0; *(volatile u16 *)(s + 0xdb) = 0;
    s[0x42] = 1; s[0xc5] = 4; s[0xc6] = 1; s[0xc7] = 2; s[0xc8] = 0;
    s[0xbe] = 0; s[0xbf] = 0; s[0xc0] = 0; s[0xc1] = 2;
    global_value = CM_GLOBAL(volatile u32, 0x009cf8a8);
    *(volatile u8 *)(s + 0xd9) |= 0x40;
    s[0x52] = 2;
    s[0x4a] = 3;
    vtable = *(volatile u32 *)s;
    *(volatile u32 *)(s + 0x1c) = global_value;
    *(volatile u32 *)(s + 0x20) = 0xffffffffu;
    s[0x49] = 7;
    *(volatile u32 *)(s + 0xe6) = 1;
    slot = *(slot_t volatile *)(vtable + 0x3c);
    result = slot(object, -1, (void *)(s + 0xa9), (void *)(s + 0x3a), 0);
    *(volatile u32 *)(s + 0xba) = result;
    return 1;
}
