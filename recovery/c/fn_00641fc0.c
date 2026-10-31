#include "cmre.h"

/* Preferred-image VA 0x00641fc0, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86.
   006830b0 and 006831a0 overwrite ECX before use or forwarding it.
   The two pushes before 006826d0 belong to the later 006831a0 call. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_009452ca(u32);
extern u32 CM_CDECL cm_fn_004afbd0(u32, u32, u32);
extern u32 CM_THISCALL cm_fn_006826d0(u32, u32);
extern u32 CM_STDCALL cm_fn_006830b0(u32, u32, u32);
extern u32 CM_STDCALL cm_fn_006831a0(u32, u32, u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_00641fc0(u32 self)
{
    u8 scratch[0x208];
    u32 local = (u32)scratch, base, id, value, other, table, row, count;
    u32 selected = 0, scan, position, destination, result, text;
    s32 bound;
    (void)self;
    base = D(0x00adadfc);
    id = D(0x009cf578);
    value = D(base + id * 4u);
    id = D(0x009cf57c);
    D(local) = value;
    value = D(base + id * 4u);
    count = D(0x00ae235c);
    D(local + 4) = value;
    table = cm_fn_00944e46(count * 4u);
    if (table == 0) {
        cm_fn_00944cff(0x009ea2a8, 0, 0, local + 0x108, local + 8);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x108, local + 8, 0x419);
        result = cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return result;
    }
    count = D(0x00ae235c);
    row = D(0x00ae23b4);
    scan = 0;
    if ((s32)count <= 0) goto done;
    do {
        value = D(row + 0x57);
        if (value != 0) {
            id = D(value);
            other = D(0x009cf580);
            if (id == other || id == D(0x009cf584) || id == D(0x009cf588)) {
                position = (u32)(s32)(s16)selected;
                ++selected;
                D(table + position * 4u) = row;
            }
        }
        count = D(0x00ae235c);
        ++scan;
        row += 0x245;
    } while ((s32)scan < (s32)count);
    if ((s16)selected < 6) goto done;
    cm_fn_004afbd0(table, selected, 6);
    position = table;
    for (scan = 6; scan != 0; --scan) {
        id = D(0x009cf578);
        base = D(0x00ae23d0);
        value = D(position);
        cm_fn_006830b0(value, base + id * 107u, 1);
        position += 4;
    }
    value = D(local);
    bound = CM_GLOBAL(volatile s16, value + 0x3e);
    scan = 0;
    if (bound <= 0) goto done;
    do {
        value = D(local);
        value = cm_fn_006826d0(value, scan);
        if (B(value + 0x37) == 3) {
            id = D(0x009cf580);
            base = D(0x00ae23d0);
            destination = base + id * 107u;
            value = D(local);
            value = cm_fn_006826d0(value, scan);
            value = D(value);
            cm_fn_006831a0(value, destination, 1);
        }
        value = D(local + 4);
        value = cm_fn_006826d0(value, scan);
        if (B(value + 0x37) == 3) {
            id = D(0x009cf580);
            base = D(0x00ae23d0);
            destination = base + id * 107u;
            value = D(local + 4);
            value = cm_fn_006826d0(value, scan);
            value = D(value);
            cm_fn_006831a0(value, destination, 1);
        }
        ++scan;
    } while ((s32)scan < bound);
done:
    return cm_fn_009452ca(table);
}
