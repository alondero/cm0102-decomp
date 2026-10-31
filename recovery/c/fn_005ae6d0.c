#include "cmre.h"

#define D(a) CM_GLOBAL(volatile u32, (a))
#define W(a) CM_GLOBAL(volatile u16, (a))
#define B(a) CM_GLOBAL(volatile u8, (a))

extern u8 CM_CDECL cm_fn_0048dd70(u32, u32);
extern u32 CM_THISCALL cm_fn_005abf10(u32, u32, u32, u32, s8);
extern u32 CM_THISCALL cm_fn_0068d1b0(u32);
extern u32 CM_CDECL cm_fn_00944cff(u32, u32, u32, u32, u32);
extern u32 CM_CDECL cm_fn_0090d130(void);
extern u32 CM_CDECL cm_fn_00944c9f(u32, u32, ...);
extern u32 CM_CDECL cm_fn_005e8290(u32, u32, u32);

/* The sole direct caller at VA 0057ff91 tests AL. 005abf10 consumes
   only the low WORDs of its first two stack arguments and the low BYTE
   of its fourth: the incidental upper EAX/ECX/ESI bits pushed here die there. */
u8 CM_STDCALL cm_fn_005ae6d0(u32 date, u32 id)
{
    u8 scratch[0x204];
    u32 base, lane, second, year, object, list, row, kind, text;
    if ((s32)id <= -1)
        return 0;
    base = D(0x00ae23d0);
    B((u32)scratch) = cm_fn_0048dd70(base + id * 107u, 0);
    if (date == 0) {
        cm_fn_00944cff(0x009cc208, 0, 0, (u32)scratch + 0x104, (u32)scratch + 4);
        text = cm_fn_0090d130();
        cm_fn_00944c9f(0x00ae24d0, 0x009870e8, text, (u32)scratch + 0x104, (u32)scratch + 4, 0xe89);
        cm_fn_005e8290(0x009870e0, 0x00ae24d0, 0);
        D(0x00b67a34) = 0;
        return 0;
    }
    kind = D((u32)scratch);
    for (lane = 0; lane != 3; ++lane) {
        second = W(date);
        year = W(date + 2);
        object = D(0x00ae2a58);
        list = cm_fn_005abf10(object, year, second, lane, (s8)kind);
        if (list != 0) {
            row = D(list + 2);
            D(list + 0xa) = row;
            row = cm_fn_0068d1b0(list);
            while (row != 0) {
                if (D(row) == id)
                    return 1;
                row = cm_fn_0068d1b0(list);
            }
        }
    }
    return 0;
}
