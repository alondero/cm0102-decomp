/* Preferred-image VA 0x00800d30, image base 0x00400000.
   Target SHA-256: 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   ECX receiver, two stack arguments, ret 8. Scratch L = entry ESP-0x200.
   Error buffers occupy L and L+0x100. The second argument is serialized as a DWORD. */
#include "cmre.h"

extern s16 CM_THISCALL cm_fn_00564080(u32 self, u32 object, u32 flag, u32 buffer);
extern u32 CM_THISCALL cm_fn_0077ac30(u32 self, u32 buffer, u32 size, u32 route);
extern void cm_fn_005e8290(const char *title, const char *text, int flags);
extern char *cm_fn_0090d130(void);
extern u32 cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 cm_fn_00944cff(u32 source, u32 a, u32 b, void *first, void *second);

void CM_THISCALL cm_fn_00800d30(u32 self, u16 field, u32 value)
{
    u8 scratch[0x200];
    u32 buffer = self + 0x37feu;
    u32 offset;
    u32 capacity;
    u32 base;
    u32 index;
    u32 row;
    u32 route;
    u32 pointer;
    u32 object;
    u32 size;
    u16 tag;
    char *text;

    if (field == 0xffffu)
        return;
    *(volatile u32 *)(self + 0x3802u) = 0;
    offset = *(volatile u32 *)(buffer + 4u);
    capacity = *(volatile u32 *)(buffer + 8u);
    if (offset + 1u > capacity) {
        cm_fn_00944cff(0x009c3734u, 0, 0, scratch + 0x100, scratch);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch + 0x100, scratch, 0x85u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        base = *(volatile u32 *)buffer;
        *(volatile u8 *)(base + offset) = 8;
        offset = *(volatile u32 *)(buffer + 4u);
        *(volatile u32 *)(buffer + 4u) = offset + 1u;
    }
    offset = *(volatile u32 *)(buffer + 4u);
    capacity = *(volatile u32 *)(buffer + 8u);
    if (offset + 2u > capacity) {
        cm_fn_00944cff(0x009c3734u, 0, 0, scratch, scratch + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch, scratch + 0x100, 0x86u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        base = *(volatile u32 *)buffer;
        *(volatile u16 *)(base + offset) = field;
        offset = *(volatile u32 *)(buffer + 4u);
        *(volatile u32 *)(buffer + 4u) = offset + 2u;
    }
    offset = *(volatile u32 *)(buffer + 4u);
    capacity = *(volatile u32 *)(buffer + 8u);
    if (offset + 1u > capacity) {
        cm_fn_00944cff(0x009c3734u, 0, 0, scratch, scratch + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch, scratch + 0x100, 0x85u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        base = *(volatile u32 *)buffer;
        *(volatile u8 *)(base + offset) = 2;
        offset = *(volatile u32 *)(buffer + 4u);
        *(volatile u32 *)(buffer + 4u) = offset + 1u;
    }
    offset = *(volatile u32 *)(buffer + 4u);
    capacity = *(volatile u32 *)(buffer + 8u);
    if (offset + 4u > capacity) {
        cm_fn_00944cff(0x009c3734u, 0, 0, scratch, scratch + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch, scratch + 0x100, 0x8du);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    } else {
        base = *(volatile u32 *)buffer;
        *(volatile u32 *)(base + offset) = value;
        offset = *(volatile u32 *)(buffer + 4u);
        *(volatile u32 *)(buffer + 4u) = offset + 4u;
    }
    index = (u32)(s32)*(volatile s16 *)(self + 0x382au);
    offset = index * 0x300u;
    row = self + offset;
    tag = *(volatile u16 *)(row + 0x7d4u);
    route = (offset & 0xffff0000u) | tag;
    if (tag == 0) {
        object = *(volatile u32 *)(self + 0x3840u);
        cm_fn_00564080(self + 0x3844u, object, 0, buffer);
        return;
    }
    pointer = *(volatile u32 *)(row + 0x7fcu);
    if (*(volatile u32 *)(pointer + 0x12f53du) != 0) {
        object = *(volatile u32 *)(self + 0x3840u);
        cm_fn_00564080(self + 0x132ea5u, object, 0, buffer);
        return;
    }
    size = *(volatile u32 *)(self + 0x3802u);
    base = *(volatile u32 *)buffer;
    object = *(volatile u32 *)(self + 0x3840u);
    if (cm_fn_0077ac30(object, base, size, route) == 0) {
        cm_fn_00944cff(0x00a7ddbcu, 0, 0, scratch, scratch + 0x100);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, (u32)text,
                       scratch, scratch + 0x100, 0xa77u);
        cm_fn_005e8290((const char *)0x009870e0u,
                       (const char *)0x00ae24d0u, 0);
        CM_GLOBAL(volatile u32, 0x00b67a34) = 0;
    }
}
