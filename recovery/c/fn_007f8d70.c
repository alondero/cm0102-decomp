/* Preferred-image VA 007f8d70, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern void CM_THISCALL cm_fn_0054a110(void *self);
extern u32 CM_CDECL cm_fn_00946464(void *key, u32 base, u32 count, u32 size, u32 comparator);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_007f8d70(u32 self, volatile u32 record,
                              volatile u32 supplied, volatile u32 check_date)
{
    volatile struct { u32 prev, handler, state; } frame;
    u8 work[0x21c];
    u32 previous, person, club, base, value, count, found, low, high, result = 0, version;
    u16 year, today_year;
    frame.state = 0xffffffffu;
    frame.handler = 0x00960ffbu;
    previous = CM_FS(volatile u32, 0);
    frame.prev = previous;
    CM_FS(volatile u32, 0) = (u32)&frame;
    cm_fn_0054a110(work + 0x10);
    person = record;
    frame.state = 0;
    if (person == 0) {
        cm_fn_00944cff(0x00a7b094u, 0, 0, work + 0x11c, work + 0x1c);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x11c, work + 0x1c, 0x5dau);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        goto done;
    }
    club = supplied;
    if (club == 0) {
        value = (u32)(s32)(s16)W(0x00b77c7au);
        club = D(0x00b74c26u + value * 768u);
        if (club == 0)
            goto done;
        club = D(club + 0x39u);
        if (club == 0)
            goto done;
    }
    base = D(self + 0x5cu);
    if (base == 0)
        goto done;
    value = D(person);
    D((u32)work + 12u) = value;
    value = D(club);
    count = D(self + 0x64u);
    D((u32)work + 8u) = value;
    found = cm_fn_00946464(work + 8, base, count, 17, 0x007fc270u);
    if (found == 0 || (s8)B(found + 0x10u) >= 3)
        goto done;
    if (check_date != 0) {
        low = D(found + 8u);
        high = D(found + 12u);
        D((u32)work) = low;
        year = W((u32)work + 2u);
        D((u32)work + 4u) = high;
        today_year = W(0x00ae2c92u);
        if (year > today_year)
            goto done;
        if (year == today_year && (s16)low >= (s16)W(0x00ae2c90u))
            goto done;
    }
    result = 1;
done:
    CM_FS(volatile u32, 0) = frame.prev;
    return result;
}
