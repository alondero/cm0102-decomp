/* Preferred-image VA 006a1b00, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Both callers overwrite EAX immediately at0069b860/0069b8b8. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_0053b900(u32 person);
extern u8 CM_CDECL cm_fn_00544820(u32 person);
extern unsigned long long CM_THISCALL cm_fn_0077bbe0(void *self, u32 kind, u32 zero);
extern unsigned long long CM_THISCALL cm_fn_00785790(void *self, u32 index, u32 value);
extern u32 CM_THISCALL cm_fn_007858c0(void *self, u32 raw_kind, u32 text, void *cursor);
extern unsigned long long CM_THISCALL cm_fn_007863f0(u32 self, void *record, u32 club);
extern unsigned long long CM_THISCALL cm_fn_00786780(u32 self, void *record);

void CM_STDCALL cm_fn_006a1b00(volatile u32 first, volatile s8 second, volatile s8 third,
                              volatile u32 fourth, volatile s8 fifth)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0xe8];
    u32 previous, record, person, low, high, pointer, value, holder, total, id, receiver;
    u16 current_year, saved_year;
    s8 kind;
    previous = CM_FS(volatile u32, 0);
    frame.state = 0xffffffffu;
    frame.handler = 0x0095cdebu;
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    record = first;
    if (record == 0)
        goto done;
    person = fourth;
    if (person == 0)
        goto done;
    low = D(0x00dd6ec8u);
    high = D(0x00dd6eccu);
    current_year = W(0x00ae2c92u);
    D((u32)work) = low;
    D((u32)work + 4u) = high;
    saved_year = W((u32)work + 2u);
    if (saved_year > current_year)
        goto done;
    if (saved_year == current_year && (s16)(u16)low > (s16)W(0x00ae2c90u))
        goto done;
    kind = second;
    if (kind == 5)
        goto done;
    cm_fn_0077bbe0(work + 8, 6000, 0);
    value = D(record);
    frame.state = 0;
    cm_fn_00785790(work + 8, 0, value);
    pointer = D(record + 4u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 1, value);
    pointer = D(record + 8u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 2, value);
    pointer = D(record + 12u);
    value = D(pointer + 0x33u);
    cm_fn_00785790(work + 8, 3, value);
    cm_fn_00785790(work + 8, 4, (u32)(s32)kind);
    value = D(person);
    cm_fn_00785790(work + 8, 5, value);
    value = (u32)(s32)fifth;
    cm_fn_00785790(work + 8, 7, value);
    holder = D(person + 0xbfu);
    if (holder != 0) {
        pointer = D(holder + 4u);
        value = D(pointer + 0x33u);
        cm_fn_00785790(work + 8, 8, value);
        holder = D(person + 0xbfu);
        pointer = D(holder + 8u);
        value = D(pointer + 0x33u);
        cm_fn_00785790(work + 8, 9, value);
        holder = D(person + 0xbfu);
        pointer = D(holder + 12u);
        value = D(pointer + 0x33u);
    } else {
        cm_fn_00785790(work + 8, 8, 0xffffffffu);
        cm_fn_00785790(work + 8, 9, 0xffffffffu);
        value = 0xffffffffu;
    }
    cm_fn_00785790(work + 8, 10, value);
    value = (u32)(s32)third;
    cm_fn_00785790(work + 8, 11, value);
    total = D(0x00ae2364u);
    id = D(record);
    if ((s32)id >= (s32)(total - 16u)) {
        pointer = D(record + 4u);
        B((u32)work) = 13;
        cm_fn_007858c0(work + 8, 13, pointer, work);
        value = (u32)(s32)(s8)B((u32)work);
        cm_fn_00785790(work + 8, 12, value);
        pointer = D(record + 8u);
        value = D((u32)work);
        cm_fn_007858c0(work + 8, value, pointer, work);
    } else {
        if (cm_fn_00544820(person) == 0)
            goto done;
        value = cm_fn_0053b900(person);
        if (value == 0 && (s16)W(person + 0x80u) <= 0x1c52) {
            person = D(person + 0x53u);
            if (person == 0)
                goto done;
            receiver = D(0x00b7431cu);
            cm_fn_007863f0(receiver, work + 8, person);
            goto done;
        }
    }
    receiver = D(0x00b7431cu);
    cm_fn_00786780(receiver, work + 8);
done:
    CM_FS(volatile u32, 0) = frame.prev;
}
