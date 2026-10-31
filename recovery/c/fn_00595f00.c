/* Preferred-image VA 00595f00, base 00400000; CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#include "cmre.h"
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_STDCALL cm_fn_006830b0(u32 club, u32 nation, u32 flag);
extern void CM_STDCALL cm_fn_006831a0(u32 club, u32 nation, u32 flag);

u32 CM_THISCALL cm_fn_00595f00(u32 self)
{
    u32 array, id, group, table, source, index, record, club, nation, expected, pointer, remaining, result;
    s8 mode;
    (void)self; /* Both original callees overwrite ECX before dependent calls. */
    array = D(0x00adadfcu);
    id = D(0x009cf7f8u);
    group = D(array + id * 4u);
    table = D(group + 0xcu);
    source = D(table + 8u);
    index = 0;
    if ((s16)W(source + 0x3eu) > 0) {
        do {
            table = D(source + 0xa7u);
            record = table + (u32)(s32)(s16)index * 6u;
            if (B(record + 5u) == 1) {
                club = D(record);
                nation = D(group + 4u);
                expected = D(club + 0x57u);
                if (expected != nation)
                    cm_fn_006830b0(club, nation, 1);
            }
            ++index;
        } while ((s16)index < (s16)W(source + 0x3eu));
    }
    mode = -1;
    do {
        if (mode == -1)
            source = group;
        else {
            table = D(group + 0xcu);
            source = D(table + (u32)(s32)mode * 4u);
        }
        index = 0;
        if ((s16)W(source + 0x3eu) > 0) {
            do {
                table = D(source + 0xb1u);
                record = table + (u32)(s32)(s16)index * 59u;
                if (B(record + 0x37u) == 3) {
                    id = D(0x009cf7fcu);
                    club = D(record);
                    nation = D(0x00ae23d0u);
                    cm_fn_006831a0(club, nation + id * 107u, 1);
                }
                ++index;
            } while ((s16)index < (s16)W(source + 0x3eu));
        }
        ++mode;
    } while (mode < 1);
    pointer = group + 0xeeu;
    remaining = 4;
    do {
        id = D(0x009cf7f8u);
        table = D(0x00ae23b4u);
        nation = D(0x00ae23d0u);
        club = D(pointer);
        result = cm_fn_006830b0(table + club * 0x245u, nation + id * 107u, 1);
        pointer += 4;
    } while (--remaining != 0);
    return result;
}
