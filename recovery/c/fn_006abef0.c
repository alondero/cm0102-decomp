/* Preferred-image VA 006abef0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Original00557000 reads category/flags as BYTE and counts as WORD;
   unused upper pushed bits are dead. Original00456740 defines full EAX. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern u32 CM_THISCALL cm_fn_00456740(u32 self, u32 low, u32 high);
extern void CM_THISCALL cm_fn_00557000(u32 self, u32 person, s8 category, u8 third, u8 fourth, u16 fifth, u8 sixth, u16 seventh, u8 eighth);
extern void CM_STDCALL cm_fn_006ac060(u32 person, s8 first, s8 second, s16 third, s8 fourth);
extern u32 CM_THISCALL cm_fn_0068d1f0(u32 self, u32 record);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_006abef0(u32 self)
{
    u8 work[0x208];
    u32 initial_list, list, value, row, high, low, person, id, table, cursor, receiver;
    u32 counter, total, version, result;
    u8 eighth, sixth, fourth, third, flag;
    u16 seventh, fifth;
    s8 category;
    D((u32)work) = 0;
    D((u32)work + 4u) = self;
    initial_list = D(self + 0x14u);
    value = D(initial_list + 2u);
    D(initial_list + 10u) = value;
    list = D(self + 0x14u);
    if ((s16)W(list) <= 0)
        return initial_list;
    for (;;) {
        row = cm_fn_0068d1b0(list);
        if (row == 0)
            goto diagnostic;
        high = D(0x00ae2c94u);
        low = D(0x00ae2c90u);
        result = cm_fn_00456740(row + 5u, low, high);
        if (result == 0) {
            id = D(row);
            table = D(0x00ae23bcu);
            person = table + id * 110u;
            flag = B(row + 4u);
            if (flag != 0) {
                cursor = row + 13u;
                do {
                    category = (s8)B(cursor);
                    if (category > -1) {
                        eighth = B(row + 0x18u);
                        seventh = W(row + 0x16u);
                        sixth = B(row + 0x15u);
                        fifth = W(row + 0x13u);
                        fourth = B(row + 0x12u);
                        third = B(row + 0x11u);
                        receiver = D(0x00ae28f0u);
                        id = D(person);
                        cm_fn_00557000(receiver, id, category, third, fourth, fifth, sixth, seventh, eighth);
                    }
                    cursor += 1u;
                } while ((s32)(cursor - row - 13u) < 4);
                self = D((u32)work + 4u);
            }
            eighth = B(row + 0x18u);
            seventh = W(row + 0x16u);
            sixth = B(row + 0x15u);
            flag = B(row + 4u);
            cm_fn_006ac060(person, (s8)flag, (s8)sixth, (s16)seventh, (s8)eighth);
            list = D(self + 0x14u);
            cm_fn_0068d1f0(list, row);
        }
        list = D(self + 0x14u);
        counter = D((u32)work);
        counter += 1u;
        total = (u32)(s32)(s16)W(list);
        D((u32)work) = counter;
        if ((s32)counter >= (s32)total)
            return counter;
    }
diagnostic:
    cm_fn_00944cff(0x009f18a4u, 0, 0, work + 0x108, work + 8);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x57f5u);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return result;
}
