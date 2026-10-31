#include "cmre.h"
extern int CM_CDECL cm_fn_009447e0(u32 value, u16 amount);

void CM_CDECL cm_fn_009394a0(u32 object)
{
    u32 stackp = *(volatile u32 *)(object + 0x20u) - 4u;
    *(volatile u32 *)(object + 0x20u) = stackp;
    u32 value = *(volatile u32 *)stackp;
    u32 eax = stackp;
    u32 add_a = 0;
    u32 add_b = 0;
    u16 amount_a = *(volatile u16 *)(object + 0x18u);
    if (amount_a != 0) {
        eax = (eax & 0xffff0000u) | amount_a;
        add_a = (u32)cm_fn_009447e0(value, (u16)eax);
        eax = add_a;
    }
    u16 amount_b = *(volatile u16 *)(object + 0x1au);
    if (amount_b != 0) {
        eax = (eax & 0xffff0000u) | amount_b;
        add_b = (u32)cm_fn_009447e0(value, (u16)eax);
        eax = add_b;
    }
    s16 count = *(volatile s16 *)(object + 0x3eu);
    u32 table = *(volatile u32 *)(object + 0x10u);
    if (count >= 0) {
        s32 remaining = (s32)count + 1;
        do {
            u32 new_stackp = *(volatile u32 *)(object + 0x20u) - 4u;
            u8 enabled_a = *(volatile s16 *)(object + 0x18u) != 0;
            *(volatile u32 *)(object + 0x20u) = new_stackp;
            u32 index = *(volatile u32 *)new_stackp;
            if (enabled_a) {
                u32 base = *(volatile u32 *)table;
                volatile u32 *slot = (volatile u32 *)(base + index * 4u);
                *slot = *slot + add_a;
                u32 flags = *(volatile u32 *)(table + 0x28u);
                *(volatile u8 *)(flags + index) |= 1u;
            }
            if (*(volatile s16 *)(object + 0x1au) != 0) {
                u32 base = *(volatile u32 *)(table + 4u);
                volatile u32 *slot = (volatile u32 *)(base + index * 4u);
                *slot = *slot + add_b;
                u32 flags = *(volatile u32 *)(table + 0x28u);
                *(volatile u8 *)(flags + index) |= 2u;
            }
            --remaining;
        } while (remaining != 0);
    }
    *(volatile u16 *)(object + 0x3eu) = 0;
}
