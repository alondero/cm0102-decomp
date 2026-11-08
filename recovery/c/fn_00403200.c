#include "cmre.h"
/* Preferred-image VA 0x00403200, base 0x00400000; CM0102 3.9.68
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   EAX/unused EDX/ECX are physical entry registers; only index_slot is a
   stack argument (RET 4). Early AX writes preserve incoming EAX high bits.
   The original rereads the low word of the stack argument after shifting. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

u32 CM_STDCALL __attribute__((regparm(3))) cm_fn_00403200(
    u32 incoming_eax, u32 incoming_edx, u32 self, volatile u32 index_slot)
{
    u32 eax = incoming_eax, table, minimum, item_offset, item_address;
    u32 current, maximum, previous_index, position_address;
    u16 index = (u16)index_slot, count, position, previous;
    (void)incoming_edx;

    if (index == 0xffffu) return eax;
    count = W(self + 0xb74u);
    eax = (eax & 0xffff0000u) | count;
    if ((s16)count >= 0x4b0) return eax;
    if (D(self + 0x18u) & 0x80000u) return eax;

    position = (u16)(count + 1u);
    item_offset = (u32)(s32)(s16)index * 396u;
    table = D(self + 0x208u);
    minimum = D(self + 0x10u);
    W(self + 0xb74u) = position;
    item_address = table + item_offset + 0x24u;
    current = D(item_address);
    if ((s32)current < (s32)minimum) D(self + 0x10u) = current;
    eax = D(item_address);
    maximum = D(self + 0x14u);
    if ((s32)eax > (s32)maximum) D(self + 0x14u) = eax;

    position = (u16)(position - 2u);
    if ((s16)position < 0) {
        W(self + 0x212u) = index;
        return eax;
    }
    for (;;) {
        position_address = self + (u32)(s32)(s16)position * 2u;
        table = D(self + 0x208u);
        previous = W(position_address + 0x212u);
        previous_index = (u32)(s32)(s16)previous;
        current = D(item_offset + table + 0x24u);
        eax = previous_index * 99u;
        maximum = D(table + eax * 4u + 0x24u);
        if ((s32)current >= (s32)maximum) {
            index = (u16)index_slot;
            eax = (eax & 0xffff0000u) | index;
            W(position_address + 0x214u) = index;
            return eax;
        }
        position = (u16)(position - 1u);
        W(position_address + 0x214u) = previous;
        if ((s16)position < 0) break;
    }
    index = (u16)index_slot;
    W(self + 0x212u) = index;
    return eax;
}
