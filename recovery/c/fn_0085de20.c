#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))

extern unsigned long long CM_CDECL cm_fn_00944e46(u32);
extern unsigned long long CM_CDECL cm_fn_00945667(u32, u32);
extern void CM_THISCALL cm_fn_00417c10(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00945333(u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0085de20(u32 self, u32 first, u32 second)
{
    u8 scratch[0x208];
    u32 table, capacity, count, id, name, extension, line, text, result;
    if (first == 0 || second == 0) {
        name = (u32)scratch + 8;
        extension = (u32)scratch + 0x108;
        line = 0x810;
        goto error;
    }
    table = D(self);
    if (table == 0) {
        table = (u32)cm_fn_00944e46(0x168);
        D(self) = table;
        if (table == 0) {
            name = (u32)scratch + 0x108;
            extension = (u32)scratch + 8;
            line = 0x81b;
            goto error;
        }
        D(self + 8) = 30;
    } else {
        capacity = D(self + 8);
        count = D(self + 4);
        if (count == capacity) {
            table = (u32)cm_fn_00945667(table, (capacity * 3u + 0x5au) * 4u);
            D(self) = table;
            if (table == 0) {
                name = (u32)scratch + 8;
                extension = (u32)scratch + 0x108;
                line = 0x82b;
                goto error;
            }
            D(self + 8) = D(self + 8) + 30u;
        }
    }
    count = D(self + 4);
    table = D(self);
    id = D(first);
    D(table + count * 12u) = id;
    count = D(self + 4);
    table = D(self);
    cm_fn_00417c10(table + count * 12u + 4, (u32)scratch, second);
    count = D(self + 4);
    table = D(self);
    ++count;
    D(self + 4) = count;
    return cm_fn_00945333(table, count, 12, 0x0085e380);

error:
    cm_fn_00944cff(0x00a835b0, 0, 0, name, extension);
    text = cm_fn_0090d130();
    cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, name, extension, line);
    result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
    D(0x00b67a34) = 0;
    return result;
}
