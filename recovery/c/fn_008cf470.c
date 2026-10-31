/* Preferred-image VA 008cf470, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller00861e0f kills full EAX with LEA at00861e14. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *self);
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 self, void *output, u32 days);
extern u32 CM_THISCALL cm_fn_00417c10(u32 self, void *output, u32 source);
extern u32 CM_THISCALL cm_fn_008e8e50(u32 self, void *date, u32 option);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_008cf470(u32 self, volatile u32 record_argument, volatile u32 output_argument)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x220];
    u32 previous, record, key, cursor, slot = 0, pointer, temporary, output, version;
    u8 index = 0;
    u16 end_year, candidate_year;
    frame.state = 0xffffffffu;
    frame.handler = 0x00963fd6u;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work);
    frame.state = 0;
    cm_fn_0054c5b0(0x00ae2c90u, work + 8, 365);
    record = record_argument;
    B((u32)&frame.state) = 1;
    key = D(record + 0x53u);
    cursor = self + 0x8acu;
    do {
        pointer = D(cursor);
        if (D(pointer + 10u) == key) {
            slot = D(cursor);
            break;
        }
        index = (u8)(index + 1u);
        cursor += 4u;
    } while ((s8)index < 28);
    cm_fn_00417c10((u32)work, work + 0x10, 0x00ae2c90u);
    for (;;) {
        end_year = W((u32)work + 10u);
        candidate_year = W((u32)work + 2u);
        if (end_year < candidate_year)
            goto diagnostic;
        if (end_year == candidate_year) {
            s16 end_day = (s16)W((u32)work + 8u);
            s16 candidate_day = (s16)W((u32)work);
            if (end_day <= candidate_day)
                goto diagnostic;
        }
        if (slot == 0)
            break;
        if (cm_fn_008e8e50(slot, work, 0) != 0)
            break;
        temporary = cm_fn_0054c5b0((u32)work, work + 0x10, 1);
        cm_fn_00417c10((u32)work, work + 0x18, temporary);
    }
    output = output_argument;
    cm_fn_00417c10(output, work + 0x18, (u32)work);
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a8d514u, 0, 0, work + 0x120, work + 0x20);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x120, work + 0x20, 0x32c6u);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
}
