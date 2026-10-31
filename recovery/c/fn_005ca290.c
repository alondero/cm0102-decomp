/* Preferred-image VA 005ca290, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   VA005c6be3 reads only the low byte of the third stack argument. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_THISCALL cm_fn_0054c5b0(u32 date, void *output, u32 days);
extern u32 CM_THISCALL cm_fn_005c6b70(u32 self, u32 date, u32 club, s8 type);
extern u32 CM_CDECL cm_fn_0066f4e0(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005ca290(u32 self, volatile u32 first, volatile u32 second,
                              volatile u32 third, volatile s16 fourth)
{
    u8 work[0x210];
    u32 message, row, pointer, low, high, date, club, version;
    u16 row_year, current_year;
    s8 type;
    s32 width;
    message = third;
    if (message != 0 && fourth < 100) {
        cm_fn_00944cff(0x009cd0a0u, 0, 0, work + 0x110, work + 0x10);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x110, work + 0x10, 0x1575u);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 1;
    }
    row = second;
    if (row == 0)
        goto cancelled;
    pointer = cm_fn_0054c5b0(0x00ae2c90u, work + 8, 2);
    low = D(pointer);
    high = D(pointer + 4u);
    row_year = W(row + 6u);
    D((u32)work + 4u) = high;
    date = row + 4u;
    D((u32)work) = low;
    current_year = W((u32)work + 2u);
    if (current_year > row_year)
        goto cancelled;
    if (current_year == row_year) {
        s16 row_day = (s16)W(date);
        if ((s16)(u16)low > row_day)
            goto cancelled;
    }
    type = (s8)B(row + 0xcu);
    club = first;
    if (cm_fn_005c6b70(self, date, club, type) != 0)
        return 1;
    if (message != 0) {
        width = (s8)B(club + 0x52u);
        cm_fn_0066f4e0(message, 0x009cd188u, width, width, club + 0x38u);
    }
    return 0;
cancelled:
    if (message != 0)
        cm_fn_0066f4e0(message, 0x009cd1d4u);
    return 0;
}
