#include "cmre.h"

/* Preferred-image VA 0x0090a540, base 0x00400000, CM0102 3.9.68.
   SHA-256 0c78e7da6d0bcb4603fa179449bbe6d1b6b5e1c627a22c445d14d489bd9bff86. */
#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))
extern u32 CM_CDECL cm_fn_00944e46(u32);
extern u32 CM_CDECL cm_fn_0090cfc0(u32);
extern u32 CM_CDECL cm_fn_00909ec0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern void CM_CDECL cm_fn_005e8290(u32, u32, u32);

u32 CM_THISCALL cm_fn_0090a540(u32 self)
{
    u8 scratch[0x204];
    u32 local = (u32)scratch, allocated, index, base, table, club, first, second;
    u32 offset = 0, scan = 0, total, competition, id, other, selected, value, text;
    u16 year;
    u8 countdown;
    W(self + 0x3e) = 2;
    allocated = cm_fn_00944e46(12);
    D(self + 0xa7) = allocated;
    if (allocated == 0) {
        cm_fn_00944cff(0x00aa20a0, 0, 0, local + 0x104, local + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, local + 0x104, local + 4, 0xc6);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0xffffffffu;
    }
    year = W(self + 0x40);
    index = D(0x009d00a4);
    base = D(0x00ae23b4);
    if ((year & 1u) != 0) {
        D(local) = 0x413;
        D(allocated) = base + index * 581u;
        index = D(0x009d00a8);
        base = D(0x00ae23b4);
        table = D(self + 0xa7);
        D(table + 6) = base + index * 581u;
    } else {
        D(local) = 0x414;
        D(allocated + 6) = base + index * 581u;
        index = D(0x009d00a8);
        base = D(0x00ae23b4);
        table = D(self + 0xa7);
        D(table) = base + index * 581u;
    }
    countdown = (u8)cm_fn_0090cfc0(6);
    total = D(0x00ae235c);
    if ((s32)total > 0) {
        do {
            base = D(0x00ae23b4);
            club = base + offset;
            competition = D(club + 0x57);
            if (competition != 0) {
                id = D(competition);
                other = D(0x009cf590);
                if (id == other) {
                    value = cm_fn_00909ec0(club);
                    selected = D(local);
                    if ((s32)(s16)value == (s32)selected) {
                        if (countdown == 0) {
                            base = D(0x00ae23b4);
                            table = D(self + 0xa7);
                            first = D(table);
                            value = D(base + scan * 581u + 0x69);
                            D(first + 0x69) = value;
                            table = D(self + 0xa7);
                            first = D(table);
                            B(first + 0x6d) = 0;
                            break;
                        }
                        countdown = (u8)(countdown - 1u);
                    }
                }
            }
            total = D(0x00ae235c);
            ++scan;
            offset += 581;
        } while ((s32)scan < (s32)total);
    }
    index = D(0x009cf4f8);
    base = D(0x00ae23a8);
    value = base + index * 290u;
    table = D(self + 0xa7);
    second = D(table + 6);
    D(second + 0x53) = value;
    table = D(self + 0xa7);
    second = D(table + 6);
    first = D(table);
    value = D(second + 0x53);
    D(first + 0x53) = value;
    table = D(self + 0xa7); B(table + 4) = 0;
    table = D(self + 0xa7); B(table + 5) = 0;
    table = D(self + 0xa7); B(table + 10) = 0;
    table = D(self + 0xa7); B(table + 11) = 0;
    return 1;
}
