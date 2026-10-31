/* Preferred-image VA0068d0d0, base00400000, CM0102 3.9.68.
   SHA256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Entry EDX is forwarded through allocation and can survive a null result.
   Explicit ECX/EDX inputs retain the original two stack arguments/RET8.
   Three earlier failed invocations preceded this boundary correction. */
#include "cmre.h"

typedef struct { void *prev; void *handler; s32 state; } cm_68d0_seh;

/* Allocation is caller-cleaned. regparm(3) carries EAX/EDX/ECX while the
   original size remains the sole stack argument; fastcall would pop it. */
extern unsigned long long CM_CDECL __attribute__((regparm(3)))
cm_fn_00944cf1(u32 eax_in, u32 edx_in, u32 ecx_in, u32 size);
extern unsigned long long CM_FASTCALL cm_fn_0068cfc0(void *self, const u8 *forwarded_src,
                                                      const u8 *src, s16 count);

unsigned long long CM_FASTCALL cm_fn_0068d0d0(volatile u8 *self, u32 edx_in, const u8 *src, s16 count)
{
    volatile cm_68d0_seh frame;
    unsigned long long alloc_result;
    unsigned long long ctor_result;
    u32 allocation;
    volatile u32 allocation_slot __attribute__((unused));
    u32 tail;
    u32 next;
    u32 result;
    u32 high;

    frame.state = -1;
    frame.handler = (void *)0x0095cb56u;
    frame.prev = CM_FS(void *, 0);
    CM_FS(void *, 0) = (void *)&frame;

    if (*(volatile s16 *)self == 0) {
        alloc_result = cm_fn_00944cf1((u32)frame.prev, edx_in, (u32)self, 0x0eu);
        allocation = (u32)alloc_result;
        allocation_slot = allocation;
        high = (u32)(alloc_result >> 32);
        frame.state = 0;
        if (allocation != 0u) {
            ctor_result = cm_fn_0068cfc0((void *)allocation, src, src, count);
            result = (u32)ctor_result;
            high = (u32)(ctor_result >> 32);
        } else {
            result = 0u;
        }
        *(volatile u32 *)(self + 2u) = result;
        if (result == 0u) {
            CM_FS(void *, 0) = frame.prev;
            return ((unsigned long long)high << 32);
        }
        tail = result;
        next = result;
    } else {
        alloc_result = cm_fn_00944cf1((u32)frame.prev, edx_in, (u32)self, 0x0eu);
        allocation = (u32)alloc_result;
        allocation_slot = allocation;
        high = (u32)(alloc_result >> 32);
        frame.state = 1;
        if (allocation != 0u) {
            ctor_result = cm_fn_0068cfc0((void *)allocation, src, src, count);
            result = (u32)ctor_result;
            high = (u32)(ctor_result >> 32);
        } else {
            result = 0u;
        }
        tail = *(volatile u32 *)(self + 6u);
        *(volatile u32 *)(tail + 6u) = result;
        tail = *(volatile u32 *)(self + 6u);
        next = *(volatile u32 *)(tail + 6u);
        if (next == 0u) {
            CM_FS(void *, 0) = frame.prev;
            return ((unsigned long long)high << 32);
        }
        *(volatile u32 *)(next + 0x0au) = tail;
        tail = *(volatile u32 *)(self + 6u);
        next = *(volatile u32 *)(tail + 6u);
        high = tail;
    }

    *(volatile u16 *)self = (u16)(*(volatile u16 *)self + 1u);
    *(volatile u32 *)(self + 6u) = next;
    result = *(volatile u32 *)next;
    CM_FS(void *, 0) = frame.prev;
    return ((unsigned long long)high << 32) | result;
}
