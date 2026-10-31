/* Preferred-image VA 007f9520, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   Sole caller at007b64a6 overwrites AL, then full EAX at007b64b4
   or007b64d1 before consuming it. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *self);
extern u32 CM_THISCALL cm_fn_00549f70(void *self, u32 day, u32 month, u32 year, u32 option);
extern u32 CM_THISCALL cm_fn_00417c10(u32 self, void *output, u32 source);
extern u32 CM_CDECL cm_fn_00946464(void *key, u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

void CM_THISCALL cm_fn_007f9520(u32 self, volatile u32 record, volatile u32 supplied)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x224];
    u32 previous, person, club, index, value, count, base, found, date, version, line;
    void *name, *extension;
    u8 status;
    frame.state = 0xffffffffu;
    frame.handler = 0x0096101bu;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work + 8);
    person = record;
    frame.state = 0;
    if (person == 0) {
        name = work + 0x124;
        extension = work + 0x24;
        line = 0x71au;
        goto diagnostic;
    }
    club = supplied;
    if (club == 0) {
        index = (u32)(s32)(s16)W(0x00b77c7au);
        club = D(0x00b74c26u + index * 768u);
        club = D(club + 0x39u);
        if (club == 0) {
            name = work + 0x24;
            extension = work + 0x124;
            line = 0x728u;
            goto diagnostic;
        }
    }
    value = D(person);
    count = D(self + 0x64u);
    D((u32)work + 4u) = value;
    value = D(club);
    base = D(self + 0x5cu);
    D((u32)work) = value;
    found = cm_fn_00946464(work, base, count, 17, 0x007fc270u);
    if (found != 0) {
        date = cm_fn_00549f70(work + 0x14, 27, 4, 1969, 0xffffffffu);
        cm_fn_00417c10(found + 8u, work + 0x1c, date);
        status = B(found + 0x10u);
        B(found + 0x10u) = (u8)(status + 1u);
    }
    goto done;
diagnostic:
    cm_fn_00944cff(0x00a7b094u, 0, 0, name, extension);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, name, extension, line);
    cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
done:
    CM_FS(volatile u32, 0) = frame.prev;
}
