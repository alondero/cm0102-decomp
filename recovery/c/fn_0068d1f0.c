#include "cmre.h"

extern void CM_FASTCALL cm_fn_0068d010(u32 *self);
extern unsigned long long CM_CDECL cm_fn_00944c94(void *pointer);

u32 CM_THISCALL cm_fn_0068d1f0(u8 *self, const u8 *target)
{
    u32 head = *(volatile u32 *)(self + 2u);
    u32 node = head;
    while (node != 0) {
        s32 signed_length = *(volatile s16 *)(node + 4u);
        u32 remaining = (u32)signed_length;
        const volatile u8 *left = (const volatile u8 *)(u32)*(volatile u32 *)node;
        const volatile u8 *right = target;
        u32 equal = 1;
        while (remaining != 0) {
            u8 a = *left;
            u8 b = *right;
            if (a != b) {
                equal = 0;
                break;
            }
            left++;
            right++;
            remaining--;
        }
        if (equal != 0) {
            if (node == head) {
                u32 next = *(volatile u32 *)(node + 6u);
                *(volatile u32 *)(self + 2u) = next;
            } else {
                u32 node_0a = *(volatile u32 *)(node + 0x0au);
                u32 next = *(volatile u32 *)(node + 6u);
                *(volatile u32 *)(node_0a + 6u) = next;
            }

            u32 tail = *(volatile u32 *)(self + 6u);
            if (node == tail) {
                u32 node_0a = *(volatile u32 *)(node + 0x0au);
                *(volatile u32 *)(self + 6u) = node_0a;
            } else {
                u32 next = *(volatile u32 *)(node + 6u);
                u32 node_0a = *(volatile u32 *)(node + 0x0au);
                *(volatile u32 *)(next + 0x0au) = node_0a;
            }

            u32 self_0a = *(volatile u32 *)(self + 0x0au);
            *(volatile u16 *)self = (u16)(*(volatile u16 *)self - 1u);
            if (node == self_0a) {
                u32 link_06 = *(volatile u32 *)(self_0a + 6u);
                *(volatile u32 *)(self + 0x0au) = link_06;
            }

            cm_fn_0068d010((u32 *)node);
            (void)cm_fn_00944c94((void *)node);
            return 1;
        }
        node = *(volatile u32 *)(node + 6u);
    }
    return 0;
}
