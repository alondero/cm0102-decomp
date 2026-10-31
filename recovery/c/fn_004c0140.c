#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))

extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_CDECL cm_fn_004c0140(u32 first, u32 second, u32 output)
{
    u8 scratch[0x214];
    u32 limit, source, table, capacity, bytes, count, copied, scanned, a, b, c, take, offset, value, text;
    D(output) = 0;
    limit = D(0x00ae237c);
    source = D(0x00ae23d4);
    table = 0;
    count = 0;
    capacity = 0;
    scanned = 0;
    copied = 0;
    bytes = 0;
    if ((s32)limit > 0) {
        do {
            if (D(source + 4) != 0) {
                a = D(source + 0xa);
                take = a == first;
                if (!take) {
                    b = D(source + 0xe);
                    take = b == first;
                    if (!take) {
                        c = D(source + 0x12);
                        take = c == first;
                        if (!take && second != 0)
                            take = a == second || b == second || c == second;
                    }
                }
                if (take) {
                    if (count == capacity) {
                        capacity += 10;
                        bytes += 0x104;
                        table = (u32)cm_fn_00945667(table, bytes);
                        if (table == 0) {
                            cm_fn_00944cff(0x0099ec20, 0, 0, (u32)scratch + 0x114, (u32)scratch + 0x14);
                            text = cm_fn_0090d130();
                            cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x114, (u32)scratch + 0x14, 0x2703);
                            cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
                            D(0x00b67a34) = 0;
                            return 0;
                        }
                    }
                    for (offset = 0; offset != 24; offset += 4) {
                        value = D(source + offset);
                        D(table + copied + offset) = value;
                    }
                    ++count;
                    value = W(source + 24);
                    W(table + copied + 24) = (u16)value;
                    copied += 26;
                }
            }
            limit = D(0x00ae237c);
            ++scanned;
            source += 26;
        } while ((s32)scanned < (s32)limit);
    }
    D(0x00ae0ff8) = first;
    D(0x00ae0ffc) = second;
    cm_fn_00945333(table, count, 26, 0x004c02f0);
    D(output) = table;
    return count;
}
