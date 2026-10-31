#include "cmre.h"

/* cdecl, no stack arguments, returns a 32-bit pointer in EAX (u32).
   Globals: 0x00de42c0 signed count (read once on entry), 0x00de32bc table base
   pointer (an array of element pointers, re-read after the allocation call).
   Element fields by byte offset: 0x00, 0x04, 0x08, 0x0c byte flags (bit mask 0x83),
   0x10 dword, 0x1c dword. A null slot is filled with a new 0x20-byte element
   from cm_fn_00944e46 (its low dword result is stored as the slot value). */
extern unsigned long long CM_CDECL cm_fn_00944e46(u32 size);

#define GLOBAL_COUNT CM_GLOBAL(volatile s32, 0x00de42c0)
#define GLOBAL_TABLE CM_GLOBAL(volatile u32, 0x00de32bc)

u32 CM_CDECL cm_fn_0094b1e0(void)
{
    s32 count;
    s32 index;
    u32 arr;
    u32 cell;
    u32 element;
    u32 result;
    u32 new_ptr;
    u32 table_now;
    u8 alloc;

    result = 0;
    index = 0;
    alloc = 0;

    count = GLOBAL_COUNT;
    if (count > 0) {
        arr = GLOBAL_TABLE;
        cell = arr;
        do {
            element = *(volatile u32 *)cell;
            if (element == 0) {
                alloc = 1;
                break;
            }
            if ((*(volatile u8 *)(element + 0x0c) & 0x83u) == 0) {
                result = *(volatile u32 *)(arr + (u32)index * 4u);
                break;
            }
            index = index + 1;
            cell = cell + 4u;
        } while (index < count);

        if (alloc != 0) {
            new_ptr = (u32)cm_fn_00944e46(0x20u);
            table_now = GLOBAL_TABLE;
            *(volatile u32 *)(table_now + (u32)index * 4u) = new_ptr;
            table_now = GLOBAL_TABLE;
            result = *(volatile u32 *)(table_now + (u32)index * 4u);
        }

        if (result != 0) {
            *(volatile u32 *)(result + 0x10) = *(volatile u32 *)(result + 0x10) | 0xffffffffu;
            *(volatile u32 *)(result + 0x04) = 0u;
            *(volatile u32 *)(result + 0x0c) = 0u;
            *(volatile u32 *)(result + 0x08) = 0u;
            *(volatile u32 *)(result + 0x00) = 0u;
            *(volatile u32 *)(result + 0x1c) = 0u;
        }
    }

    return result;
}
