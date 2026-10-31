#include "cmre.h"
/* WIP: calls through a stack-local ECX and SEH frame don't match the original
   instruction-derived stack memory after the first object store. */

extern short *CM_THISCALL cm_fn_00549f70(short *arg1, short arg2, char arg3,
                                        u32 arg4, u32 arg5);

u32 CM_THISCALL cm_fn_004c4f50(volatile u8 *self)
{
    struct {
        void *prev;
        void *handler;
        u32 state;
    } volatile frame;
    volatile u16 local_word;
    volatile u8 *call_self;
    volatile u32 *vtable;
    u32 call_result;
    void *restore_prev;
    u32 entry_prev;

    frame.state = 0xffffffffu;
    frame.handler = (void *)0x00956b28;
    entry_prev = (u32)CM_FS(void *, 0);
    frame.prev = (void *)entry_prev;
    CM_FS(void *, 0) = (void *)&frame;

    self[0xc2] = 3;
    self[0xc7] = 3;
    self[0x4a] = 3;
    local_word = *(volatile u16 *)(self + 0x40);
    self[0x3c] = 2;
    self[0xc6] = 2;
    self[0x42] = 2;
    self[0xc3] = 1;
    self[0xc4] = 7;
    self[0xc5] = 1;
    self[0xbe] = 1;
    self[0xbf] = 0;
    self[0xc0] = 0;
    self[0xc1] = 0;
    *(volatile u32 *)(self + 0x1c) = 0xffffffffu;
    *(volatile u32 *)(self + 0x20) = 0xffffffffu;
    self[0x49] = 7;

    (void)cm_fn_00549f70((short *)&local_word, 0x000f, 5,
                         (entry_prev & 0xffff0000u) | local_word,
                         0xffffffffu);

    vtable = *(volatile u32 **)self;
    local_word = *(volatile u16 *)(self + 0x40);
    *(volatile u16 *)(self + 0xb6) = local_word;
    call_self = self + 0x3a;
    (void)call_self;
    call_result = 0;
    frame.state = 0;
    *(volatile u32 *)(self + 0xb8) = call_result;
    *(volatile u32 *)(self + 0x28) = 0x001e8480u;
    call_result = ((u32 (CM_THISCALL *)(volatile u8 *, u32, volatile u8 *,
                                       volatile u8 *, u32))vtable[0x3c / 4])(
        self, 0xffffffffu, self + 0xa9, self + 0x3a, 0);
    restore_prev = frame.prev;
    *(volatile u32 *)(self + 0xba) = call_result;
    CM_FS(void *, 0) = restore_prev;
    return 1;
}
