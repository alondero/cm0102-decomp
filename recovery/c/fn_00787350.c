/* Preferred-image VA 00787350, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern s32 CM_THISCALL cm_fn_00786fe0(u32 self, u32 record, u32 zero);
extern unsigned long long CM_THISCALL cm_fn_00782b70(u32 self, u32 record, u32 state_address, u32 kind, void *output);
extern u32 CM_CDECL cm_fn_00944cff(u32 path, u32 drive, u32 directory, void *name, void *extension);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32 destination, u32 format, ...);
extern void CM_CDECL cm_fn_005e8290(u32 title, u32 message, u32 flags);

u32 CM_THISCALL cm_fn_00787350(u32 self, volatile u32 first, u32 second)
{
    u8 work[0x204];
    u32 record, index, total, table, row, start, position, base, offset, receiver, result, end, version;
    u8 type;
    if (second == 0) {
        record = first;
        return cm_fn_00786fe0(self, record, 0) > 0;
    }
    record = first;
    index = D(record);
    total = D(0x00ae2364u);
    index = index - total + 16u;
    if ((s32)index < 0) {
        cm_fn_00944cff(0x00a693ecu, 0, 0, work + 0x104, work + 4);
        version = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0u, 0x009870e8u, version, work + 0x104, work + 4, 0x241eu);
        cm_fn_005e8290(0x009870e0u, 0x00ae24d0u, 0);
        D(0x00b67a34u) = 0;
        return 0;
    }
    table = D(self + 8u);
    start = D(table + index * 12u);
    row = table + index * 12u;
    if (start == 0xffffffffu)
        return 0;
    position = D(row + 4u) - 1u;
    for (;;) {
        start = D(row);
        if ((s32)position < (s32)start)
            position = start + 99u;
        base = D(self);
        offset = position * 223u;
        receiver = base + offset;
        if (D(receiver + 0xd6u) == 0)
            return 0;
        record = first;
        result = (u32)cm_fn_00782b70(receiver, record, 0x00de1f64u, 2000, work);
        if ((u8)result == 2)
            return 1;
        base = D(self);
        receiver = base + offset;
        type = B(receiver + 4u);
        if (type == 1 && D(receiver + 0xdau) == 0)
            return 1;
        end = D(row + 4u);
        --position;
        --end;
        if (position == end)
            return 0;
    }
}
