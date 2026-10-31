/* Preferred-image VA 0079d1f0, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_004b0700(u32 id);
extern s32 CM_CDECL cm_fn_0090cfc0(s32 limit);

u16 CM_THISCALL cm_fn_0079d1f0(u32 self, u32 object, u32 destination)
{
    u32 id, table, pointer, country, region, remaining, entry, wanted, candidate;
    u32 first, count, source, offset, output, value;
    u8 kind, found = 0, byte;
    s32 rows;
    if (object == 0)
        return 0;
    id = D(object);
    table = D(0x00adadfcu);
    pointer = D(table + id * 4u);
    if (pointer == 0)
        return 0;
    kind = B(pointer + 0x42u);
    if (kind == 4 || kind == 2)
        return 0;
    if (cm_fn_004b0700(id) == 0)
        return 0;
    pointer = D(object + 0x5du);
    if (pointer != 0)
        country = D(pointer);
    else {
        country = D(0x00ae2350u);
        country = (u32)cm_fn_0090cfc0((s32)country);
    }
    table = D(self + 0xcu);
    region = table + country * 258u;
    rows = (s16)W(region + 0x10u);
    if (rows > 0) {
        remaining = (u32)rows;
        entry = region + 0x12u;
        first = remaining;
        count = remaining;
        do {
            if (found == 0) {
                candidate = D(entry + 0xcu);
                wanted = D(object);
                if (candidate == wanted) {
                    first = D(entry);
                    count = D(entry + 4u);
                    found = 1;
                }
            }
            entry += 16u;
            remaining -= 1u;
        } while (remaining != 0);
    }
    if (rows <= 0 || found == 0) {
        first = D(region + 8u);
        count = D(region + 4u);
    }
    if ((s32)count > 0) {
        offset = first * 17u;
        output = destination;
        remaining = count;
        do {
            table = D(self);
            source = table + offset;
            offset += 17u;
            value = D(source);
            D(output) = value;
            value = D(source + 4u);
            D(output + 4u) = value;
            value = D(source + 8u);
            D(output + 8u) = value;
            value = D(source + 0xcu);
            D(output + 0xcu) = value;
            byte = B(source + 0x10u);
            B(output + 0x10u) = byte;
            output += 17u;
            remaining -= 1u;
        } while (remaining != 0);
    }
    return (u16)count;
}
