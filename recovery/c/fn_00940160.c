/* Preferred-image VA 0x00940160; image base 0x00400000; pinned target SHA-256
   0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"

extern u32 CM_CDECL cm_fn_009443a0(u32 size, u32 bytes);
extern u32 CM_CDECL cm_fn_00944480(u32 size, u32 old_table, u32 bytes);

u32 CM_CDECL cm_fn_00940160(u32 self)
{
    u32 capacity = *(volatile u32 *)(self + 0x21f4);
    if (capacity == 0) {
        u32 inline_table = self + 0x2170u;
        u32 initial = self + 0x170u;
        *(volatile u32 *)(self + 0x21f4) = 0x20;
        *(volatile u32 *)(self + 0x21f0) = inline_table;
        *(volatile u32 *)(self + 0x21f8) = 0;
        *(volatile u32 *)inline_table = initial;
        *(volatile u32 *)(self + 0x168) = initial;
        *(volatile u32 *)(self + 0x16c) = initial + 0x2000u;
        return initial;
    }

    {
        u32 first = *(volatile u32 *)(self + 0x40);
        u32 second = *(volatile u32 *)(self + 0x34);
        u32 third = *(volatile u32 *)(self + 0x38);
        u32 index;
        u32 bytes;
        u32 table;
        u32 new_capacity;
        u32 allocation;

        first -= *(volatile u32 *)(self + 0x3c);
        first -= second;
        index = *(volatile u32 *)(self + 0x21f8);
        first += third;
        capacity = *(volatile u32 *)(self + 0x21f4);
        bytes = (u32)(((s32)first >> 6) + 0x100);

        ++index;
        *(volatile u32 *)(self + 0x21f8) = index;
        if ((s32)index >= (s32)capacity) {
            table = *(volatile u32 *)(self + 0x21f0);
            new_capacity = index + index;
            *(volatile u32 *)(self + 0x21f4) = new_capacity;
            if (table == self + 0x2170u) {
                u32 size = *(volatile u32 *)(self + 0x2222);
                u32 new_table = cm_fn_009443a0(size, new_capacity << 2);
                u32 offset = 0;
                *(volatile u32 *)(self + 0x21f0) = new_table;
                do {
                    u32 destination = *(volatile u32 *)(self + 0x21f0);
                    u32 value = *(volatile u32 *)(self + 0x2170u + offset);
                    *(volatile u32 *)(destination + offset) = value;
                    offset += 4;
                } while (offset < 0x80);
            } else {
                u32 bytes_for_table = new_capacity << 2;
                u32 size = *(volatile u32 *)(self + 0x2222);
                u32 new_table = cm_fn_00944480(size, table, bytes_for_table);
                *(volatile u32 *)(self + 0x21f0) = new_table;
            }
        }

        {
            u32 size = *(volatile u32 *)(self + 0x2222);
            allocation = cm_fn_009443a0(size, bytes << 3);
        }
        index = *(volatile u32 *)(self + 0x21f8);
        table = *(volatile u32 *)(self + 0x21f0);
        *(volatile u32 *)(table + index * 4u) = allocation;
        *(volatile u32 *)(self + 0x168) = allocation;
        *(volatile u32 *)(self + 0x16c) = allocation + (bytes << 3);
        return allocation;
    }
}
