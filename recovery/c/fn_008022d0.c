/* Preferred-image VA 0x008022d0, image base 0x00400000.
   Target SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, two stack arguments, ret 8. No defined EAX result on the
   arg1==-1 path. Scratch L is entry ESP-0x200; buffers L and L+0x100.
   Original callees remain explicit; 0x00564080 uses ECX and ret 0xc. */
#include "cmre.h"

extern void CM_THISCALL cm_fn_00564080(u32 self, u32 arg1, u32 arg2, u32 arg3);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 buffer, u32 count, u32 arg3);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);

#define NETWORK_ERROR(source, first, second, line) do { \
    char *text; \
    cm_fn_00944cff((source), 0, 0, (first), (second)); \
    text = cm_fn_0090d130(); \
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text, (first), (second), (line)); \
    cm_fn_005e8290((const char *)0x009870e0u, (const char *)0x00ae24d0u, 0); \
    CM_GLOBAL(volatile u32, 0x00b67a34) = 0; \
} while (0)

void CM_THISCALL cm_fn_008022d0(u32 self, u16 arg1, u32 arg2)
{
    u8 scratch[0x200];
    u32 position;
    u32 capacity;
    u32 buffer;
    u32 value;
    u32 offset;
    u32 object;
    u32 receiver;
    u16 word;
    if (arg1 == 0xffffu)
        return;

    *(volatile u32 *)(self + 0x3802u) = 0;
    position = *(volatile u32 *)(self + 0x3802u);
    capacity = *(volatile u32 *)(self + 0x3806u);
    if (position + 1u > capacity) {
        NETWORK_ERROR(0x009c3734u, scratch + 0x100, scratch, 0x85u);
    } else {
        buffer = *(volatile u32 *)(self + 0x37feu);
        *(volatile u8 *)(buffer + position) = 8;
        position = *(volatile u32 *)(self + 0x3802u);
        *(volatile u32 *)(self + 0x3802u) = position + 1u;
    }
    position = *(volatile u32 *)(self + 0x3802u);
    capacity = *(volatile u32 *)(self + 0x3806u);
    if (position + 2u > capacity) {
        NETWORK_ERROR(0x009c3734u, scratch, scratch + 0x100, 0x86u);
    } else {
        buffer = *(volatile u32 *)(self + 0x37feu);
        *(volatile u16 *)(buffer + position) = arg1;
        position = *(volatile u32 *)(self + 0x3802u);
        *(volatile u32 *)(self + 0x3802u) = position + 2u;
    }
    position = *(volatile u32 *)(self + 0x3802u);
    capacity = *(volatile u32 *)(self + 0x3806u);
    if (position + 1u > capacity) {
        NETWORK_ERROR(0x009c3734u, scratch, scratch + 0x100, 0x85u);
    } else {
        buffer = *(volatile u32 *)(self + 0x37feu);
        *(volatile u8 *)(buffer + position) = 10;
        position = *(volatile u32 *)(self + 0x3802u);
        *(volatile u32 *)(self + 0x3802u) = position + 1u;
    }
    position = *(volatile u32 *)(self + 0x3802u);
    capacity = *(volatile u32 *)(self + 0x3806u);
    if (position + 4u > capacity) {
        NETWORK_ERROR(0x009c3734u, scratch, scratch + 0x100, 0x89u);
    } else {
        buffer = *(volatile u32 *)(self + 0x37feu);
        *(volatile u32 *)(buffer + position) = arg2;
        position = *(volatile u32 *)(self + 0x3802u);
        *(volatile u32 *)(self + 0x3802u) = position + 4u;
    }

    offset = (u32)(s32)*(volatile s16 *)(self + 0x382au) * 0x300u;
    object = self + offset;
    word = *(volatile u16 *)(object + 0x7d4u);
    value = (offset & 0xffff0000u) | word;
    if (word == 0) {
        receiver = *(volatile u32 *)(self + 0x3840u);
        cm_fn_00564080(self + 0x3844u, receiver, 0, self + 0x37feu);
        return;
    }
    buffer = *(volatile u32 *)(object + 0x7fcu);
    if (*(volatile u32 *)(buffer + 0x12f53du) != 0) {
        receiver = *(volatile u32 *)(self + 0x3840u);
        cm_fn_00564080(self + 0x132ea5u, receiver, 0, self + 0x37feu);
        return;
    }
    position = *(volatile u32 *)(self + 0x3802u);
    buffer = *(volatile u32 *)(self + 0x37feu);
    receiver = *(volatile u32 *)(self + 0x3840u);
    if (cm_fn_0077ac30(receiver, buffer, position, value) == 0) {
        NETWORK_ERROR(0x00a7ddbcu, scratch, scratch + 0x100, 0xb8du);
    }
}
