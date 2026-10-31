/* Preferred-image VA 005abd90, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u8 CM_CDECL cm_fn_0048dd70(u32 first, u32 second);
extern u32 CM_THISCALL cm_fn_005abf10(u32 self, u32 first, u32 second, u32 third, u32 fourth);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32 self);
extern unsigned long long CM_THISCALL cm_fn_0068d0d0(u32 self, u32 source, s16 count);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_005abd90(u32 self, volatile u32 first, volatile u32 second,
                              volatile u32 third, u32 competition, volatile u32 filter,
                              volatile s8 category_argument, volatile u32 output_argument)
{
    u8 work[0x208];
    u32 table, pointer, raw, a, b, c, list, row, output, match, wanted, value, version, result;
    s8 category;
    D((u32)work) = 0;
    if (competition != 0xffffffffu) {
        table = D(0x00ae23d0u);
        pointer = table + competition * 107u;
        B((u32)work + 4u) = cm_fn_0048dd70(pointer, 0);
    } else {
        B((u32)work + 4u) = 0;
    }
    raw = D((u32)work + 4u);
    c = third;
    b = second;
    a = first;
    list = cm_fn_005abf10(self, a, b, c, raw);
    if (list == 0)
        return (u32)W((u32)work);
    value = D(list + 2u);
    D(list + 10u) = value;
    row = cm_fn_0068d1b0(list);
    if (row == 0)
        return (u32)W((u32)work);
    output = output_argument;
    category = category_argument;
    do {
        match = 1;
        if (competition != 0xffffffffu && competition != D(row))
            match = 0;
        wanted = filter;
        if (wanted != 0xffffffffu) {
            value = (u32)(s32)(s16)W(row + 0x34u);
            if (filter != value)
                match = 0;
        }
        if (category == -2 || B(row + 0x42u) == (u8)category) {
            if (match == 1) {
                result = (u32)cm_fn_0068d0d0(output, row, 0x4f);
                if (result == 0)
                    goto diagnostic;
                value = D((u32)work);
                D((u32)work) = value + 1u;
            }
        }
        row = cm_fn_0068d1b0(list);
    } while (row != 0);
    return (u32)W((u32)work);
diagnostic:
    cm_fn_00944cff(0x009cc208u, 0, 0, work + 0x108, work + 8);
    version = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x108, work + 8, 0x634u);
    result = cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
    D(0x00b67a34u) = 0;
    return (result & 0xffff0000u) | W((u32)work);
}
