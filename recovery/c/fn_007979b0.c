#include "cmre.h"
extern short *CM_THISCALL cm_fn_00549f70(short *self, short arg1, char arg2, u32 arg3, u32 arg4);
typedef u32 (CM_THISCALL *slot_t)(void *, s32, void *, void *, u32);
u32 CM_THISCALL cm_fn_007979b0(void *object)
{
    volatile u8 *s = (volatile u8 *)object;
    volatile struct { void *prev; u32 handler; u32 state; } frame;
    volatile u16 temp[4];
    u32 old_fs, field40, mixed, vtable, result;
    void *restore_fs;
    u16 result_word;
    slot_t slot;
    frame.state = 0xffffffffu;
    frame.handler = 0x0095eae8u;
    old_fs = (u32)CM_FS(void *, 0);
    frame.prev = (void *)old_fs;
    CM_FS(void *, 0) = (void *)&frame;
    s[0x43] = 2; s[0xc6] = 2; s[0xbe] = 2;
    field40 = *(volatile u16 *)(s + 0x40);
    mixed = (old_fs & 0xffff0000u) | field40;
    s[0xc2] = 3; s[0x49] = 3; s[0x4a] = 3;
    *(volatile u16 *)(s + 0x3c) = 1;
    s[0xc3] = 1; s[0xc4] = 8; s[0xc5] = 1; s[0xc7] = 4;
    s[0xbf] = 0; s[0xc0] = 0; s[0xc1] = 0;
    *(volatile u16 *)(s + 0xd9) = 0x28;
    (void)cm_fn_00549f70((short *)temp, 0x14, 7, mixed, 5);
    result_word = temp[0];
    vtable = *(volatile u32 *)s;
    *(volatile u16 *)(s + 0xb6) = result_word;
    frame.state = 0;
    *(volatile u16 *)(s + 0xb8) = 0;
    slot = *(slot_t volatile *)(vtable + 0x3c);
    result = slot(object, -1, (void *)(s + 0xa9), (void *)(s + 0x3a), 0);
    restore_fs = frame.prev;
    *(volatile u32 *)(s + 0xba) = result;
    CM_FS(void *, 0) = restore_fs;
    return 1;
}
